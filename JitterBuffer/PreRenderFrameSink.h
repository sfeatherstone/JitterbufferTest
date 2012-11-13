#pragma once

#include "IFrameSinkImpl.h"
#include "IRenderer.h"
#include <boost/date_time/posix_time/posix_time.hpp>

//This recieves Frames from the PreDecodeSink it then sends them to the IRenderer implementation
//This is the class that delays the calls to render to attemp to make the output smooth.
class PreRenderFrameSink : public IFrameSinkImpl
{
public:
	PreRenderFrameSink(IRenderer * renderer);
	~PreRenderFrameSink(void);

protected:
	virtual void ProcessFrame(int frameNo, int lengthOfBuffer, boost::shared_array<char> buffer) override;

private:
	IRenderer * renderer_;

	int secondsToRecordForLongestDelay_;
	//Times used to help implemt smooth rendering
	boost::posix_time::ptime firstFrameRendered_;
	std::deque<boost::posix_time::time_duration> recentDelays_; //Finite store of delays

	boost::posix_time::time_duration	idealTimePeriod_;  //== 1/30 second
	boost::posix_time::time_duration	currentLagDelay_;  // current extra delay required (due to late packets)
};

