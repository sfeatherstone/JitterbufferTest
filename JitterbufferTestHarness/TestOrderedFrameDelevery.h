#pragma once

#include "..\JitterBuffer\Common.h"
#include <vector>

class TestOrderedFrameDelevery
{
public:
	TestOrderedFrameDelevery(void);
	~TestOrderedFrameDelevery(void);

	void AddFrame(const char * buffer, int length);
	int size() const;
	bool testDelevery(int frameNo,const char * buffer, int length) const;


protected:
	std::vector<LengthBufferPair>	framesDelevered_;
};

