#pragma once

#include "IFrameSink.h"
#include "common.h"
#include <boost/thread.hpp>


class IFrameSinkImpl : public IFrameSink
{
public:
	IFrameSinkImpl(void);
	~IFrameSinkImpl(void);

	virtual void Frame(int frameNo, int lengthOfBuffer, boost::shared_array<char> buffer) override;

	void run();

protected:
	virtual void ProcessFrame(int frameNo, int lengthOfBuffer, boost::shared_array<char> buffer) = 0;

private:
	typedef std::map<int, LengthBufferPair> FrameStore;

	FrameStore frames_;
	int nextFrame_;
	bool _runLoop;

	std::shared_ptr<boost::thread> watchingThread_;
	boost::mutex conditionLock_;
	boost::condition_variable condition_;
};

