#include "stdafx.h"
#include "UnrenderedFrame.h"

UnrenderedFrame::UnrenderedFrame(int numFragmentsInThisFrame) : 
	numFragmentsInThisFrame_(numFragmentsInThisFrame),
	fragmentVector_(numFragmentsInThisFrame),
	storedFragmentsInThisFrame_(0)
	{
	}

//Returns true when frame is full (has all parts)
bool UnrenderedFrame::addFragment(int fragmentNumber, const char* buffer, int length)
{
	boost::shared_array<char> bufferCopy(new char[length]);
	memcpy(bufferCopy.get(),buffer,length);

	fragmentVector_[fragmentNumber] = std::make_pair(length, bufferCopy);
	//We don't need to protect the counter, because we have a lock in FragmentStore class
	++storedFragmentsInThisFrame_;
	return storedFragmentsInThisFrame_>=numFragmentsInThisFrame_;
};

//Assemble the frame into one buffer
LengthBufferPair UnrenderedFrame::getFrame() const
{
	if (storedFragmentsInThisFrame_>=numFragmentsInThisFrame_)
	{
		int totalLength = 0;
		for(auto f : fragmentVector_)
		{
			totalLength+=f.first;
		}
		boost::shared_array<char> array(new char[totalLength]);
		int offset = 0;
		for(auto f : fragmentVector_)
		{
			memcpy(array.get()+offset, f.second.get(), f.first);
			offset+=f.first;
		}
		return LengthBufferPair(totalLength, array);
	}
	return LengthBufferPair(-1,boost::shared_array<char>());
}
