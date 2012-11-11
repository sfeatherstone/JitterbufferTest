#include "stdafx.h"
#include "IFrameSinkImpl.h"


IFrameSinkImpl::IFrameSinkImpl (void):_runLoop(true),nextFrame_(0)
{
	watchingThread_ = std::make_shared<boost::thread>( boost::bind( &IFrameSinkImpl::run, this) );
}


IFrameSinkImpl::~IFrameSinkImpl (void)
{
	_runLoop = false;
	{
		boost::lock_guard<boost::mutex> lock(conditionLock_);
		condition_.notify_one();
	}
	watchingThread_->join();
}


void IFrameSinkImpl::Frame(int frameNo, int lengthOfBuffer, boost::shared_array<char> buffer) 
{
	boost::unique_lock<boost::mutex> lock(conditionLock_);
	frames_[frameNo] = std::make_pair(lengthOfBuffer, buffer);
	if (frameNo==nextFrame_)
	{
		condition_.notify_one();
	}
};

void IFrameSinkImpl::run()
{
	while (_runLoop)
	{
		boost::unique_lock<boost::mutex> lock(conditionLock_);
		auto i = frames_.find(nextFrame_);
		if (i == frames_.end())
		{
			condition_.wait(lock);
		}

		i = frames_.find(nextFrame_);
		if (i != frames_.end())
		{
			ProcessFrame(i->first,i->second.first, i->second.second );
			frames_.erase(i);
			++nextFrame_;
		}
	}
};

