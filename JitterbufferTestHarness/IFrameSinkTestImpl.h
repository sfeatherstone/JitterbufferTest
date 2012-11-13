#pragma once
#include "..\JitterBuffer\IFrameSink.h"
#include "..\JitterBuffer\common.h"
#include "TestHelpers.h"

class IFrameSinkTestImpl:public IFrameSink
{
	public:
		IFrameSinkTestImpl(int sleep = 0):frameNo_(-1),lengthOfBuffer_(-1),sleep_(sleep){};
		virtual void Frame(int frameNo, int lengthOfBuffer, boost::shared_array<char> buffer)
		{
			SLEEP(sleep_);
			frameNo_=frameNo;
			lengthOfBuffer_=lengthOfBuffer;
			buffer_ = buffer;
			allFrames_[frameNo]= std::make_pair(lengthOfBuffer_, buffer_);
		}

		std::map<int, LengthBufferPair> allFrames_;
		int frameNo_;
		int lengthOfBuffer_;
		boost::shared_array<char> buffer_;
		int sleep_;
};
