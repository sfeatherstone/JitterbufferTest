#pragma once
#include <vector>
#include <boost\thread\mutex.hpp>
#include "common.h"

class UnrenderedFrame
{
public:
	typedef std::shared_ptr<UnrenderedFrame> sptr;

	UnrenderedFrame(int numFragmentsInThisFrame) : numFragmentsInThisFrame_(numFragmentsInThisFrame),
		fragmentVector_(numFragmentsInThisFrame),storedFragmentsInThisFrame_(0)
	{

	}

	//Returns true when frame is full (has all parts)
	bool addFragment(int fragmentNumber, const char* buffer, int length)
	{
		boost::shared_array<char> bufferCopy(new char[length]);
		memcpy(bufferCopy.get(),buffer,length);

		fragmentVector_[fragmentNumber] = std::make_pair(length, bufferCopy);
		//Protect the counter. Fragment should be OK because we preallocated.
		//boost::mutex::scoped_lock lock(dataMutex_);
		++storedFragmentsInThisFrame_;
		return storedFragmentsInThisFrame_>=numFragmentsInThisFrame_;
	};

	LengthBufferPair getFrame() const
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
	
private:
	typedef std::vector<LengthBufferPair> FragmentVector;
	FragmentVector fragmentVector_;
	int numFragmentsInThisFrame_;
	int storedFragmentsInThisFrame_;
	//boost::mutex dataMutex_;



};
