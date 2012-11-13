#include "stdafx.h"
#include "..\JitterBuffer\PreDecodeFrameSink.h"
#include "TestDecoderImpl.h"
#include "IFrameSinkTestImpl.h"
#include "TestHelpers.h"

BOOST_AUTO_TEST_CASE(PreDecodedFrameSinkPassesThreeFrames)
{
	TestDecoderImpl renderer;
	auto testSink = std::make_shared<IFrameSinkTestImpl>();
	auto sink = std::make_shared<PreDecodeFrameSink>(&renderer,testSink);

	char *buffer = "0000-12345678890";
	char *buffer1 = "0001-12345678890";
	char *buffer2 = "0002-12345678890";

	sink->Frame(0, strlen(buffer), BufferToArray(buffer));
	SLEEP(50);
	BOOST_REQUIRE( renderer.size() == 1 );
	//BOOST_REQUIRE( testSink->buffer_ == BufferToArray(buffer)
	sink->Frame(1, strlen(buffer1), BufferToArray(buffer1));
	SLEEP(50);
	BOOST_REQUIRE( renderer.size() == 2 );
	sink->Frame(2, strlen(buffer2), BufferToArray(buffer2));
	SLEEP(50);
	BOOST_REQUIRE( renderer.size() == 3 );

}

BOOST_AUTO_TEST_CASE(PreDecodedFrameSinkPassesThreeFramesOutOfOrder)
{
	TestDecoderImpl renderer;
	auto testSink = std::make_shared<IFrameSinkTestImpl>();
	auto sink = std::make_shared<PreDecodeFrameSink>(&renderer,testSink);

	char *buffer = "0000-12345678890";
	char *buffer1 = "0001-12345678890";
	char *buffer2 = "0002-12345678890";

	//BOOST_REQUIRE( testSink->buffer_ == BufferToArray(buffer)
	sink->Frame(1, strlen(buffer1), BufferToArray(buffer1));
	SLEEP(50);
	BOOST_REQUIRE( renderer.size() == 0 );
	sink->Frame(2, strlen(buffer2), BufferToArray(buffer2));
	SLEEP(50);
	BOOST_REQUIRE( renderer.size() == 0 );

	sink->Frame(0, strlen(buffer), BufferToArray(buffer));
	SLEEP(50);
	//Race!!! Need better test
	BOOST_REQUIRE( renderer.size() == 3 );

}
/*
BOOST_AUTO_TEST_CASE(PreRenderFrameSinkFastTestForNonBlocking)
{
	TestDecoderImpl renderer;
	auto testSink = std::make_shared<IFrameSinkTestImpl>(40);
	auto sink = std::make_shared<PreDecodeFrameSink>(&renderer,testSink);

	char *buffer = "0000-12345678890";
	for (int i = 0;i<1000;i++)
	{
		sink->Frame(i, strlen(buffer), BufferToArray(buffer));
	}
	std::cout << " "<< testSink->allFrames_.size();
}*/