#pragma once

#include "..\JitterBuffer\IDecoder.h"
#include "TestOrderedFrameDelevery.h"

class TestDecoderImpl : public IDecoder, public TestOrderedFrameDelevery
{
public:
	TestDecoderImpl(void);
	~TestDecoderImpl(void);

	virtual int DecodeFrame(const char* buffer, int length, char* outputBuffer) override;

	//Used for testing
	LengthBufferPair decodeFrame(const char* buffer, int length);
private:
	int DecodeFrameImpl(const char* buffer, int length, char* outputBuffer);
};

