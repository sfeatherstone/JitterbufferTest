#include "stdafx.h"
#include "UnCompressedFrameSink.h"


UnCompressedFrameSink::UnCompressedFrameSink(IRenderer * renderer):
	renderer_(renderer)
{
}

UnCompressedFrameSink::~UnCompressedFrameSink()
{
}


void UnCompressedFrameSink::ProcessFrame(int frameNo, int lengthOfBuffer, boost::shared_array<char> buffer)
{
	renderer_->RenderFrame(buffer.get(),lengthOfBuffer);
}