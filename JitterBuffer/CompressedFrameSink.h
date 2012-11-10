#pragma once

#include "IFrameSinkImpl.h"
#include "IDecoder.h"
#include <boost/date_time/posix_time/posix_time.hpp>


class CompressedFrameSink : public IFrameSinkImpl
{
public:
	CompressedFrameSink(IDecoder * decoder, IFrameSink::wptr renderSink);
	~CompressedFrameSink(void);

protected:
	virtual void ProcessFrame(int frameNo, int lengthOfBuffer, boost::shared_array<char> buffer) override;
private:
	IDecoder * decoder_;	
	IFrameSink::wptr renderSink_;
	//boost::posix_time::ptime 
};

