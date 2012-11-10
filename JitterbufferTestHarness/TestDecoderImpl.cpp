#include "TestDecoderImpl.h"


TestDecoderImpl::TestDecoderImpl(void)
{
}


TestDecoderImpl::~TestDecoderImpl(void)
{
}

int TestDecoderImpl::DecodeFrame(const char* buffer, int length, char* outputBuffer)
{
	//Make it a bit different
//	boost::shared_array<char> buff(new char[in.first*2]);
	AddFrame(buffer,length);
	return DecodeFrameImpl(buffer, length, outputBuffer);
};

int TestDecoderImpl::DecodeFrameImpl(const char* buffer, int length, char* outputBuffer)
{
	memcpy(outputBuffer,buffer,length);
	memcpy(outputBuffer+length, buffer,length);
	for (int i = 0; i < length * 2; i++)
	{
		if (::isalpha(outputBuffer[i]))
		{
			outputBuffer[i] = ::toupper(outputBuffer[i]);
		}
	}
	return length*2;
}


LengthBufferPair TestDecoderImpl::decodeFrame(const char* buffer, int length)
{
	boost::shared_array<char> buff(new char[length*2]);
	DecodeFrameImpl(buffer, length, buff.get());
	return std::make_pair(length*2, buff);
};
