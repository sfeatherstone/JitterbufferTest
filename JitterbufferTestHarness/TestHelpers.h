#pragma once

#include <boost\thread.hpp>
#define SLEEP(x) boost::this_thread::sleep(boost::posix_time::milliseconds(x));

#include <boost\shared_array.hpp>

inline boost::shared_array<char> BufferToArray(const char * buffer)
{
	boost::shared_array<char> ret(new char[strlen(buffer)]);
	memcpy(ret.get(),buffer,strlen(buffer));
	return ret;
}

#include "..\Jitterbuffer\Common.h"

inline LengthBufferPair BufferToLengthBufferPair(const char * buffer)
{
	return std::make_pair(strlen(buffer),BufferToArray(buffer));
}


