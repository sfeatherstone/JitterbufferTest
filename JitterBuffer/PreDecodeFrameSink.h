#pragma once

#include "IFrameSinkImpl.h"
#include "IDecoder.h"
#include <boost/date_time/posix_time/posix_time.hpp>


class PreDecodeFrameSink : public IFrameSinkImpl
{
public:
	PreDecodeFrameSink(IDecoder * decoder, IFrameSink::wptr renderSink);
	~PreDecodeFrameSink(void);

protected:
	virtual void ProcessFrame(int frameNo, int lengthOfBuffer, boost::shared_array<char> buffer) override;
private:
	IDecoder * decoder_;	
	IFrameSink::wptr renderSink_;
	//boost::posix_time::ptime 
};

