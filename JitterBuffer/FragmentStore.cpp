#include "stdafx.h"
#include "FragmentStore.h"

FragmentStore::FragmentStore(IFrameSink::wptr sink):frameSink_(sink)
{
};

void FragmentStore::ReceivePacket(
		const char* buffer, 
		int length, 
		int frameNumber, 
		int fragmentNumber, 
		int numFragmentsInThisFrame)
{
	UnrenderedFrame::sptr frame;

	//Lock up the store. This will prevent anything else from accessing the Unrendered frames, so they don't need a lock.
	boost::mutex::scoped_lock lock(storeMutex_);
	auto i = store_.find(frameNumber);
	if (i !=store_.end())
	{
		frame = i->second;
	}
	else
	{
		frame = std::make_shared<UnrenderedFrame>(numFragmentsInThisFrame);
		store_[frameNumber] = frame;
	}
	
	if (frame->addFragment(fragmentNumber,buffer,length))
	{
		auto complete = frame->getFrame();
		if (complete.first == -1)
		{
			//panic - we should not be complete and get a duff frame
			return;
		}
		if (auto sp = frameSink_.lock())
		{
			sp->Frame(frameNumber, complete.first, complete.second);
		}
		//Remove frame.
		store_.erase(frameNumber);
	}
};
