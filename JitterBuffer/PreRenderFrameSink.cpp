#include "stdafx.h"
#include "PreRenderFrameSink.h"
#include <boost\thread.hpp>
#include <thread>
#include <chrono>


PreRenderFrameSink::PreRenderFrameSink(IRenderer * renderer):
	renderer_(renderer)
{
	idealTimePeriod_ = boost::posix_time::microseconds(33333);
	secondsToRecordForLongestDelay_ = 10;
}

PreRenderFrameSink::~PreRenderFrameSink()
{
}


void PreRenderFrameSink::ProcessFrame(int frameNo, int lengthOfBuffer, boost::shared_array<char> buffer)
{
	//Get now just once
	auto now = boost::posix_time::microsec_clock::universal_time();
	if (frameNo == 0)
	{
		firstFrameRendered_  = now;
	}

	//Record time that this frame should be played out as duration from first frame (frames * (1/30)sec) + longestDelay
	boost::posix_time::time_duration expectedDelay = boost::posix_time::microseconds(idealTimePeriod_.total_microseconds() * frameNo) + currentLagDelay_;

	//Record delay from first frame to now
	boost::posix_time::time_duration currentDelay = now - firstFrameRendered_;

	//Get difference between two. How much later are we than we expected to be (but keep it positive)
	boost::posix_time::time_duration overdueDelayToRecord = 
		std::max(boost::posix_time::time_duration(),currentDelay-expectedDelay);

	//Add the delay to the our running maximum delay
	recentDelays_.push_front(overdueDelayToRecord);
	if (recentDelays_.size()>(30*secondsToRecordForLongestDelay_))
	{
		recentDelays_.pop_back();
	}

	//Find Longest delay
	boost::posix_time::time_duration longestDuration;
	for (auto duration : recentDelays_)
	{
		longestDuration = std::max(longestDuration, duration);
	}

	//If the longest delay is now smaller than than what is stored in our recentDelays_, start slowly shortening the delay
	if (currentLagDelay_ > longestDuration)
	{
		//Shrinks slowly
		currentLagDelay_ -= boost::posix_time::milliseconds(5);
	}
	else
	{
		//Grows fast
/*		if (currentLagDelay_ < longestDuration)
		{
			std::cout << "late:" << boost::posix_time::to_simple_string(currentLagDelay_) 
				<< "->"<<boost::posix_time::to_simple_string(longestDuration);
		}*/
		currentLagDelay_ = longestDuration;
	}

	//Are we still early?
	if (currentDelay < expectedDelay)
	{
		//Delay amount of time to take us to expected time
		boost::this_thread::sleep(expectedDelay-currentDelay);
	}
	
	renderer_->RenderFrame(buffer.get(),lengthOfBuffer);
}