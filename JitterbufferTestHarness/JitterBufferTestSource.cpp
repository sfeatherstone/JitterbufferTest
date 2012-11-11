#include "JitterBufferTestSource.h"
#include "TestHelpers.h"
#include <thread>
#include <random>
#include <iostream>
#include <boost/format.hpp>


typedef std::mt19937 Myeng; 
typedef std::binomial_distribution<int, double> Mydist; 

JitterBufferTestSource::~JitterBufferTestSource(void)
{
}

void JitterBufferTestSource::run(IJitterBuffer * buffer)
{
	std::gamma_distribution<double> variance(2.0,2.0);
	Myeng eng;
	long long maxrandomValue=0;
	std::chrono::microseconds delay = std::chrono::microseconds(0);
	for(int frameNo = 0; frameNo < frames_;frameNo++)
	{
		int fragments = (frameNo % 7) + 1;
		for(int frag = 0; frag < fragments; frag++)
		{
			long long randomValue = (long long)(15000.0 * variance(eng));
			maxrandomValue = std::max(randomValue, maxrandomValue);
			std::chrono::microseconds packetDelay = delay + std::chrono::microseconds(randomValue);
			deleveryMap_.insert(std::make_pair(packetDelay, std::make_pair(frameNo,frag)));
		}
		delay+=std::chrono::microseconds(33333);
	}

	std::cout << maxrandomValue;

	std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
	auto i = deleveryMap_.begin();
	while (i!=deleveryMap_.end())
	{
		std::chrono::time_point<std::chrono::system_clock> sleepTill  = now + i->first;
		std::this_thread::sleep_until(sleepTill);
		std::string payload( (boost::format("%06d%02dHello World - Welcome to my video data") 
			% i->second.first % i->second.second).str());
		buffer->ReceivePacket(payload.c_str(), payload.length(), i->second.first, i->second.second, (i->second.first % 7) + 1);
		i = deleveryMap_.erase(i);
	}
};

LengthBufferPair JitterBufferTestSource::getFrame(int frameNo)
{
	std::string total;

	int fragments = (frameNo % 7) + 1;
	for(int frag = 0; frag < fragments; frag++)
	{
		total.append( (boost::format("%06d%02dHello World - Welcome to my video data") 
			% frameNo % frag).str());
	}

	return BufferToLengthBufferPair(total.c_str());

};