#include "stdafx.h"
#include "IJitterBufferImpl.h"
#include "CompressedFrameSink.h"
#include "UnCompressedFrameSink.h"


IJitterBufferImpl::IJitterBufferImpl(IDecoder* decoder, IRenderer* renderer)
{
	unCompressedFrameSink_ = std::make_shared<UnCompressedFrameSink>(renderer);
	compressedFrameSink_ = std::make_shared<CompressedFrameSink>(decoder, unCompressedFrameSink_);
	fragmentStore_ = std::make_shared<FragmentStore>(compressedFrameSink_);
}

void IJitterBufferImpl::ReceivePacket(
		const char* buffer, 
		int length, 
		int frameNumber, 
		int fragmentNumber, 
		int numFragmentsInThisFrame)
{
	fragmentStore_->ReceivePacket(buffer, length, frameNumber, fragmentNumber, numFragmentsInThisFrame);
}