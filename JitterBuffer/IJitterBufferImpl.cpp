#include "stdafx.h"
#include "IJitterBufferImpl.h"
#include "PreDecodeFrameSink.h"
#include "PreRenderFrameSink.h"


IJitterBufferImpl::IJitterBufferImpl(IDecoder* decoder, IRenderer* renderer)
{
	PreRenderFrameSink_ = std::make_shared<PreRenderFrameSink>(renderer);
	PreDecodeFrameSink_ = std::make_shared<PreDecodeFrameSink>(decoder, PreRenderFrameSink_);
	fragmentStore_ = std::make_shared<FragmentStore>(PreDecodeFrameSink_);
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