#include "stdafx.h"
#include "PreRenderFrameSink.h"
#include <boost\thread.hpp>
#include <thread>
#include <chrono>


PreRenderFrameSink::PreRenderFrameSink(IRenderer * renderer):
	renderer_(renderer)
{
	nextFrameRenderedDue_ = std::chrono::system_clock::now();
	idealTimePeriod_ = currentTimePeriod_ = std::chrono::microseconds(33333);
}

PreRenderFrameSink::~PreRenderFrameSink()
{
}


void PreRenderFrameSink::ProcessFrame(int frameNo, int lengthOfBuffer, boost::shared_array<char> buffer)
{
	auto now = std::chrono::system_clock::now();
	if (now > nextFrameRenderedDue_)
	{
		//Change delay
		typedef std::chrono::duration<std::chrono::system_clock::rep, std::chrono::system_clock::period> PeriodType;
		auto delay = now - nextFrameRenderedDue_ ;
		//What do we need to create the delay to get it back 2 30fps in 5 secs?
		auto delayCount = delay.count();// * PeriodType::period::num/PeriodType::period::den;
		auto fiveSecsCount = 5 * (PeriodType::period::den / PeriodType::period::num);
		auto timeTofix = fiveSecsCount - delayCount;
		auto countsPerFrameDelay = timeTofix / (30 * 5);
		//Limit to 60fps
		countsPerFrameDelay = std::max(static_cast<long long>((PeriodType::period::den / PeriodType::period::num)/60), countsPerFrameDelay);
		currentTimePeriod_ = std::chrono::duration_cast<std::chrono::microseconds>(PeriodType(countsPerFrameDelay));
		std::cout << " fps:" << (PeriodType::period::den / PeriodType::period::num)/countsPerFrameDelay<< " delay:"<<delayCount << " ttf:"<< timeTofix;
	}
	else
	{
		std::this_thread::sleep_until(nextFrameRenderedDue_);
	}

	renderer_->RenderFrame(buffer.get(),lengthOfBuffer);
	nextFrameRenderedDue_ = now + currentTimePeriod_;
}