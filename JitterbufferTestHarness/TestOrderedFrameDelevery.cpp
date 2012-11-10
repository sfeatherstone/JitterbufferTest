#include "TestOrderedFrameDelevery.h"


TestOrderedFrameDelevery::TestOrderedFrameDelevery(void)
{
}


TestOrderedFrameDelevery::~TestOrderedFrameDelevery(void)
{
}

void TestOrderedFrameDelevery::AddFrame(const char * buffer, int length)
{
	boost::shared_array<char> buff(new char[length]);
	memcpy(buff.get(),buffer,length);
	framesDelevered_.push_back(std::make_pair(length, buff));
};

int TestOrderedFrameDelevery::size() const
{
	return framesDelevered_.size();
};

bool TestOrderedFrameDelevery::testDelevery(int frameNo,const char * buffer, int length) const
{
	if (framesDelevered_.size()<frameNo)
		return false;
	if (framesDelevered_[frameNo].first!=length)
		return false;
	return !memcmp(framesDelevered_[frameNo].second.get(),buffer,length);
};

