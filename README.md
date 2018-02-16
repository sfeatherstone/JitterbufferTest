# Video Streaming Test Task - Implement a Jitterbuffer

## Instructions

### Description

The jitterbuffer’s task is to receive packets from the network, each packet containing one fragment of an
encoded video frame, and to then:
* Reassemble these packets into frames - there may be 1-n packets (fragments) in every frame, and they
can be reassembled by simply combining them together in the right order – any extra header for the
packets will have already been stripped.
* Pass these assembled frames to the decoder to decode. Note the decoder can only decode the frames in
the correct order, and with no gaps (frames skipped) - you are guaranteed that every fragment of every
frame will arrive eventually, but may have to wait when some fragments are late.
* Send these decoded frames to the renderer to display.

Packets may be delayed, and even arrive out of order, and it is up to the jitterbuffer to wait until it has all the
packets for the next frame, assemble them into the frame, and then pass to the decoder.

Because some packets may arrive late, you can have situations where you are stuck waiting for some late
packet and thus unable to render anything for say one second, and then suddenly have many frames available
to decode + render. In such a case, you will want to try to 'catch up' (decode+render the queued frames
quickly), but at a reasonable pace, so it is up to the jitterbuffer to control this to try to make what is seen
appear to the user as smoothly as possible. If you want to get fancy, you can try to make the jitterbuffer adapt
the delay to conditions, for example in a situation where packets are often arriving late, you may want to
generally wait longer before decoding frames and passing them to the renderer even when they are available,
so that when one frame is suddenly late, it’s easier to smooth over.

You should decide whether the jitterbuffer should create its own thread from which to do the decoding and
rendering, or should just be driven solely by the thread that is passing it the fragments.

You should provide some form of unit-tests to you used to prove that your solution works - this doesn't need to
work with real video data + decoding + rendering of course.

And you should provide some documentation about how your solution works, why you solved it in the way you
did, and what limitations/issues you see in your solution (and how things could be improved). Indicate if you
identified any trade-offs to be made during implementation, and why you decided to resolve them as you did.
Also, keep track of the time spent on this task, and report how long you spent in doing the different things. Try
to keep the total time you spend on this task to no more than 20 hours, and to come up with the best solution that
is practical within this timeframe.

Please ask us if you have any questions about the specification or requirements.

```
Interfaces
class IDecoder
{
public:
	/*
	Returns the size of the data written to the outputBuffer, will be no more than 1mb.
	*/
	virtual int DecodeFrame(const char* buffer, int length, char* outputBuffer) = 0;
	~IDecoder() {}
};
class IRenderer
{
public:
	/*
	Renders the given buffer. This call will not block for any significant period, and
	the buffer will be copied internally so can be deleted/reused as soon as this call
	is completed.
	*/
	virtual void RenderFrame(const char* buffer, int length) = 0;
	~IRenderer() {}
};
/*
This is the class that you need to implement!
*/
class IJitterBuffer
{
public:
	IJitterBuffer(IDecoder* decoder, IRenderer* renderer);
	/*
	Should copy the given buffer, as it may be deleted/reused immediately following this call.
	@param frameNumber - will start at zero for the call
	@param fragmentNumber – specifies what position this fragment is within the given
	frame – the first fragment number in each frame is number zero
	@param numFragmentsInThisFrame - is guaranteed to be identical for all fragments
	with the same frameNumber
	*/
	virtual void ReceivePacket(
		const char* buffer,
		int length,
		int frameNumber,
		int fragmentNumber,
		int numFragmentsInThisFrame) = 0;
	~IJitterBuffer() {}
};
```

## Response

### Implementation
The functionality was implemented in a static library as having it in static lib helps with unit testing. It could then be compiled into a DLL.

To allow testing of the different functions, my `IJitterFrameImpl` is just the wiring together of the three main classes that do the work.

`FragmentStore` handles the calls to `IJitterBuffer:: ReceivePacket` It stores the fragment in a `UnrenderedFrame` class which is also responsible for signalling all parts received and assembling a continuous buffer from the unordered fragments. This is then passed to the `PreDecodeFrameSink`.

`PreDecodeFrameSink` stores the frame until the next frame in sequence is delivered. It is then decoded using `IDecoder` and sent to `PreRenderFrameSink`.

`PreRenderFrameSink` again stores the frame and delays the frame (sleeps the Rendering thread) and then passes it to `IRenderer`. Both `PreDecodeFrameSink` and `PreRenderFrameSink` use their own thread empty their stores of frames. A condition variable is used to signal between the thread adding frames to the worker thread .

The unit tests where written to test the individual parts before fitting them together. A final test simulates 1000 frames being sent at the rate of 30fps but with a random delay on top of that. I used a gamma random distribution (http://en.wikipedia.org/wiki/Gamma_distribution) with a shape parameter of 2.0 and scale of 2.0. Research (http://www.ieee-infocom.org/2004/Papers/37_4.PDF & http://www.t-vips.com/sites/default/files/whitepapers/Whitepaper_QoS.pdf ) seemed to support packet lateness distribution. I also borrowed a C# animation project (a clock face) that I used to get a feel for smoothness of playout. Communication between the two was done with a simple named pipe.

I trialled with an idea of speeding up or slowing down frames to achieve the result, but in order to do this I would need to get ahead of the playout, which would not be acceptable in a realtime communication application. I found this to be a dead end.

The easiest solution would be to always delay by the “most delayed packet” that has been received. I felt a better solution would be to delay by the “most delayed packet” for the last n seconds. If you find that your delay is too great (due to a long delay that has passed), start reducing the delay in order to catch up to what the new maximum delay is. This should be smoother than just jumping forward.

Notable test classes:
`TestDecoderImpl`. Implements `IDecoder`. Doubles length of data and shifts any lowercase alphas to uppercase.

`TestRendererImpl` Implements `IRenderer`. Like `TestDecoderImpl` it will store incoming frames for testing against expected results. It also opens a named pipe and writes the first 8 bytes to that pipe. This was used to drive a C# animation application to test perceived smoothness.

`IFrameSinkTestImpl` Stores all frames received so that they can be compared with expected delivery.

`JitterBufferTestSource` Will send n amount of fragments (1-7 per frame) with a delay of framenumber * 1/30 sec + random delay to `IJitterBuffer`.

#### Breakdown of task
* Initial 30fps Implementation including supporting unit tests : 6hrs (Estimate 3hrs)
* Enhanced system testing: 2hrs 20mins (Estimate 2hrs)
* Implement frame speedup/catchup 2hrs 20mins (Estimate 2hrs)
* Implement variable delay 3hrs (Estimate 2hrs)
* Cleanup and documentation 1hrs 30mins (Estimate 1hr)

#### Limitations
I did not code for packets that do not arrive. This would cause this implementation to stall.<br>
I do not feel that my tests fully tested concurrency issues.<br>
I don’t have access to real world examples of packet delivery. I have estimated what delays would be like.<br>
I did feel that the `PreRenderFrameSink` needed to block the decoder, (if it had more than a second of frames buffered), to prevent the process from having to hold too many uncompressed frames in memory. But after working with the algorithm for smoothing playout, it seemed the problem would always be not having enough frames to render in a timely fashion.<br>
I used the heap for storage and did not attempt any kind of memory pooling. It would be wise to examine with a memory profiler if there is a problem before expending effort in trying to solve it .<br>

#### Observations
Writing unit tests as I created my interfaces allow the capturing of bugs early on (I forgot to copy the contents of memory buffer more than once), before I attempted to piece all the parts together.<br>
I decided to use VS2012 and where possible Standard template Library over Boost. The `<chrono>` class was more complex, with less examples available than their boost posix time equivalent. Ultimately I reverted to the boost implementation.<br>
VS2012 compiling C++, was too slow on my aging machine. The P4 I am using takes almost 5 minutes to do a full rebuild.
Phase one was not taxing in terms of the code that I wrote, but just took much longer than I expected.
I found once I had my store of lateness measurement, that increasing it from 5 seconds to 10 seconds reduced the jerkiness of the playback better.<br>
Although part of the algorithm starts to speed up the delivery of frames when very late frames haven’t been encountered for a while, it will always revert to the average of 30/fps.<br>