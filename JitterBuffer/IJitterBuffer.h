#pragma once

#include "IDecoder.h"
#include "IRenderer.h"

class IJitterBuffer
{
public:
	//IJitterBuffer(IDecoder* decoder, IRenderer* renderer);

	/*
	Should copy the given buffer, as it may be deleted/reused immediately following this call.
	@param frameNumber - will start at zero for the call
	@param fragmentNumber – specifies what position this fragment is within the given 
	frame – the first fragment number in each frame is number zero
	@param numFragmentsInThisFrame - is guaranteed to be identical for all fragments
	with the same frameNumber
	*/
	virtual void ReceivePacket(
		const char* buffer, 
		int length, 
		int frameNumber, 
		int fragmentNumber, 
		int numFragmentsInThisFrame) = 0;

	~IJitterBuffer() {}
};
