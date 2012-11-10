#pragma once

class IRenderer
{
public:
	/*
	Renders the given buffer. This call will not block for any significant period, and 
	the buffer will be copied internally so can be deleted/reused as soon as this call 
	is completed.
	*/
	virtual void RenderFrame(const char* buffer, int length) = 0;
	~IRenderer() {}
};