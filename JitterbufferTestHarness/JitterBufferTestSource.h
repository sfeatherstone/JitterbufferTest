#pragma once

#include <map>
#include <chrono>
#include "..\Jitterbuffer\IJitterBuffer.h"

class JitterBufferTestSource
{
public:
	JitterBufferTestSource(int frames):frames_(frames){};
	~JitterBufferTestSource(void);

	void run(IJitterBuffer * buffer);

	//get frame

	int frames_;
	typedef std::pair<int,int> FrameFragmentPair;
	typedef std::multimap<std::chrono::microseconds, FrameFragmentPair> DeleveryMap;
	DeleveryMap deleveryMap_;

};

