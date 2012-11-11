#include "stdafx.h"
#include "UnCompressedFrameSink.h"
#include <boost\thread.hpp>
#include <thread>


UnCompressedFrameSink::UnCompressedFrameSink(IRenderer * renderer):
	renderer_(renderer)
{
	nextFrameRenderedDue_ = std::chrono::system_clock::now();
	idealTimePeriod_ = currentTimePeriod_ = std::chrono::microseconds(33333);
}

UnCompressedFrameSink::~UnCompressedFrameSink()
{
}


void UnCompressedFrameSink::ProcessFrame(int frameNo, int lengthOfBuffer, boost::shared_array<char> buffer)
{
	auto now = std::chrono::system_clock::now();
	std::this_thread::sleep_until(nextFrameRenderedDue_);
	//if (now < nextFrameRenderedDue_)
	{
		renderer_->RenderFrame(buffer.get(),lengthOfBuffer);
		nextFrameRenderedDue_ = now + currentTimePeriod_;
	}
}