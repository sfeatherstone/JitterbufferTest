#pragma once
#include <memory>
#include <boost\shared_array.hpp>

class IFrameSink
{
	public:
		typedef std::weak_ptr<IFrameSink> wptr;
		virtual ~IFrameSink(){};
		virtual void Frame(int frameNo, int lengthOfBuffer, boost::shared_array<char> buffer) = 0;
};
