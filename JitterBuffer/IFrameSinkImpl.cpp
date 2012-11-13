#include "stdafx.h"
#include "IFrameSinkImpl.h"
#include <queue>

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
	// Lock, store, notify other thread
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
		std::queue<std::pair<int,LengthBufferPair>> readyForProcessesingFrames;

		{
			//Lock access to frames_
			boost::unique_lock<boost::mutex> lock(conditionLock_);

			auto i = frames_.find(nextFrame_);

			if (i == frames_.end())
			{
				//Release lock and wait for signal.
				condition_.wait(lock);
			}

			//Move all complete in order frames to queue
			i = frames_.find(nextFrame_);
			if (i != frames_.end())
			{
				ProcessFrame(i->first,i->second.first, i->second.second );
				frames_.erase(i);
				++nextFrame_;
			}

		}

		//This can be blocked, but the lock is now released to the sink can receive frames again
		while (readyForProcessesingFrames.size() && _runLoop)
		{
			ProcessFrame(readyForProcessesingFrames.front().first,
				readyForProcessesingFrames.front().second.first, 
				readyForProcessesingFrames.front().second.second );
			readyForProcessesingFrames.pop();
		}

	}
};

