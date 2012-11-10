#pragma once

class IDecoder
{
public:
	/*
	Returns the size of the data written to the outputBuffer, will be no more than 1mb.
	*/
	virtual int DecodeFrame(const char* buffer, int length, char* outputBuffer) = 0;
	~IDecoder() {}
};
