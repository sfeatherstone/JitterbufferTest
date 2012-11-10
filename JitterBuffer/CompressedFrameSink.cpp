#include "stdafx.h"
#include "CompressedFrameSink.h"


CompressedFrameSink::CompressedFrameSink(IDecoder * decoder, IFrameSink::wptr renderSink):
	decoder_(decoder),
	renderSink_(renderSink)
{
}

CompressedFrameSink::~CompressedFrameSink()
{
}

void CompressedFrameSink::ProcessFrame(int frameNo, int lengthOfBuffer, boost::shared_array<char> buffer)
{
	boost::shared_array<char> outBuffer(new char[0x100000]);
	int frameSize = decoder_->DecodeFrame(buffer.get(), lengthOfBuffer, outBuffer.get());
	if (auto sp = renderSink_.lock())
	{
		sp->Frame(frameNo, frameSize, outBuffer);
	}
}
