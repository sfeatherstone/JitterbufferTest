#pragma once
#include <vector>
#include <boost\thread\mutex.hpp>
#include "common.h"

class UnrenderedFrame
{
public:
	typedef std::shared_ptr<UnrenderedFrame> sptr;

	UnrenderedFrame(int numFragmentsInThisFrame);

	//Returns true when frame is full (has all parts)
	bool addFragment(int fragmentNumber, const char* buffer, int length);
	LengthBufferPair getFrame() const;
	
private:
	typedef std::vector<LengthBufferPair> FragmentVector;
	FragmentVector fragmentVector_;
	int numFragmentsInThisFrame_;
	int storedFragmentsInThisFrame_;
};
