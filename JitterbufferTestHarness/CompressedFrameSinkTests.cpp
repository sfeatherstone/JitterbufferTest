#include "stdafx.h"
#include "..\JitterBuffer\CompressedFrameSink.h"

#include "IFrameSinkTestImpl.h"
#include "TestDecoderImpl.h"
#include "TestHelpers.h"


BOOST_AUTO_TEST_CASE(CompressedFrameSinkPassesThreeFrames)
{
	TestDecoderImpl decoder;
	auto testSink = std::make_shared<IFrameSinkTestImpl>();
	auto sink = std::make_shared<CompressedFrameSink>(&decoder, testSink);

	char *buffer = "0000-12345678890";
	char *buffer1 = "0001-12345678890";
	char *buffer2 = "0002-12345678890";

	sink->Frame(0, strlen(buffer), BufferToArray(buffer));
	SLEEP(50);
	BOOST_REQUIRE( testSink->frameNo_ == 0 );
	//BOOST_REQUIRE( testSink->buffer_ == BufferToArray(buffer)
	sink->Frame(1, strlen(buffer1), BufferToArray(buffer1));
	SLEEP(50);
	BOOST_REQUIRE( testSink->frameNo_ == 1 );
	sink->Frame(2, strlen(buffer2), BufferToArray(buffer2));
	SLEEP(50);
	BOOST_REQUIRE( testSink->frameNo_ == 2 );

/*	BOOST_REQUIRE( sink->frameNo_ == 0 );
	BOOST_REQUIRE( sink->lengthOfBuffer_ == strlen(buffer) );
	std::string expectedResult(buffer);
	BOOST_REQUIRE(!memcmp(expectedResult.c_str(),sink->buffer_.get(),sink->lengthOfBuffer_));*/
}

BOOST_AUTO_TEST_CASE(CompressedFrameSinkPassesThreeFramesOutOfOrder)
{
	TestDecoderImpl decoder;
	auto testSink = std::make_shared<IFrameSinkTestImpl>();
	auto sink = std::make_shared<CompressedFrameSink>(&decoder, testSink);

	char *buffer = "0000-12345678890";
	char *buffer1 = "0001-12345678890";
	char *buffer2 = "0002-12345678890";

	//BOOST_REQUIRE( testSink->buffer_ == BufferToArray(buffer)
	sink->Frame(1, strlen(buffer1), BufferToArray(buffer1));
	SLEEP(50);
	BOOST_REQUIRE( testSink->frameNo_ == -1 );
	sink->Frame(2, strlen(buffer2), BufferToArray(buffer2));
	SLEEP(50);
	BOOST_REQUIRE( testSink->frameNo_ == -1 );

	sink->Frame(0, strlen(buffer), BufferToArray(buffer));
	SLEEP(50);
	//Race!!! Need better test
	BOOST_REQUIRE( testSink->frameNo_ == 2 );

/*	BOOST_REQUIRE( sink->frameNo_ == 0 );
	BOOST_REQUIRE( sink->lengthOfBuffer_ == strlen(buffer) );
	std::string expectedResult(buffer);
	BOOST_REQUIRE(!memcmp(expectedResult.c_str(),sink->buffer_.get(),sink->lengthOfBuffer_));*/
}