#pragma once

#include "IFrameSinkImpl.h"
#include "IRenderer.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <chrono>

class PreRenderFrameSink : public IFrameSinkImpl
{
public:
	PreRenderFrameSink(IRenderer * renderer);
	~PreRenderFrameSink(void);

protected:
	virtual void ProcessFrame(int frameNo, int lengthOfBuffer, boost::shared_array<char> buffer) override;

private:
	IRenderer * renderer_;
	std::chrono::time_point<std::chrono::system_clock> nextFrameRenderedDue_;
	std::chrono::microseconds	idealTimePeriod_;
	std::chrono::microseconds	currentTimePeriod_;
};

