#include "stdafx.h"
#include "PreDecodeFrameSink.h"


PreDecodeFrameSink::PreDecodeFrameSink(IDecoder * decoder, IFrameSink::wptr renderSink):
	decoder_(decoder),
	renderSink_(renderSink)
{
}

PreDecodeFrameSink::~PreDecodeFrameSink()
{
}

void PreDecodeFrameSink::ProcessFrame(int frameNo, int lengthOfBuffer, boost::shared_array<char> buffer)
{
	boost::shared_array<char> outBuffer(new char[0x100000]);
	int frameSize = decoder_->DecodeFrame(buffer.get(), lengthOfBuffer, outBuffer.get());
	if (auto sp = renderSink_.lock())
	{
		sp->Frame(frameNo, frameSize, outBuffer);
	}
}
