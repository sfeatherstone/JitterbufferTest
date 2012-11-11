#pragma once
#include "..\JitterBuffer\IFrameSink.h"

class IFrameSinkTestImpl:public IFrameSink
{
	public:
		IFrameSinkTestImpl():frameNo_(-1),lengthOfBuffer_(-1){};
		virtual void Frame(int frameNo, int lengthOfBuffer, boost::shared_array<char> buffer)
		{
			frameNo_=frameNo;
			lengthOfBuffer_=lengthOfBuffer;
			buffer_ = buffer;
		}
		int frameNo_;
		int lengthOfBuffer_;
		boost::shared_array<char> buffer_;
};
