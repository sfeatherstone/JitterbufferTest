#pragma once

#include "IJitterBuffer.h"
#include "UnrenderedFrame.h"
#include "IFrameSink.h"

class FragmentStore 
{
public:
	FragmentStore(IFrameSink::wptr sink);

	void ReceivePacket(
		const char* buffer, 
		int length, 
		int frameNumber, 
		int fragmentNumber, 
		int numFragmentsInThisFrame);

	~FragmentStore() {}
private:
	typedef std::map<int,UnrenderedFrame::sptr> UnrenderedFrameStore;
	UnrenderedFrameStore store_;
	boost::mutex storeMutex_;
	IFrameSink::wptr frameSink_;
};
