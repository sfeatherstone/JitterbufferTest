#pragma once

#include <windows.h>

#include "..\JitterBuffer\IRenderer.h"
#include "TestOrderedFrameDelevery.h"

class TestRendererImpl : public IRenderer, public TestOrderedFrameDelevery
{
public:
	TestRendererImpl(void);
	~TestRendererImpl(void);

	virtual void RenderFrame(const char* buffer, int length) override
	{
		AddFrame(buffer,length);
		DWORD cbWritten;
		WriteFile(pipe_, buffer, 8, &cbWritten, NULL);
	}
private:
	HANDLE pipe_;
};


