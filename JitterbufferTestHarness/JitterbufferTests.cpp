#include "stdafx.h"
#include "..\JitterBuffer\IJitterbufferImpl.h"
#include <memory>
#include <boost\thread.hpp>
#include "TestDecoderImpl.h"
#include "TestRendererImpl.h"

#define SLEEP(x) boost::this_thread::sleep(boost::posix_time::milliseconds(x));

BOOST_AUTO_TEST_CASE(CanCreateAndDeleteIJitterBufferImpl)
{
	auto jitterBuffer = std::make_shared<IJitterBufferImpl>(nullptr, nullptr);

    BOOST_REQUIRE(jitterBuffer);

}

BOOST_AUTO_TEST_CASE(CanCreateAndDeleteIJitterBufferImplAsIJitterBuffer)
{
	std::shared_ptr<IJitterBuffer> jitterBuffer = std::make_shared<IJitterBufferImpl>(nullptr, nullptr);

    BOOST_REQUIRE(jitterBuffer);
}

BOOST_AUTO_TEST_CASE(CanJitterBufferCanProcessTwoOutOfOrderFrames)
{
	TestRendererImpl renderer;
	TestDecoderImpl decoder;
	auto jitterBuffer = std::make_shared<IJitterBufferImpl>(&decoder, &renderer);
    BOOST_REQUIRE(jitterBuffer);

	char *buffer = "0001-12345678890";
	char *buffer1 = "12345678890abcderf";
	char *buffer2 = "abcderf12345678890";

	char *zbuffer = "0002-z12345678890";
	char *zbuffer1 = "z12345678890abcderf";
	char *zbuffer2 = "zabcderf12345678890";

	jitterBuffer->ReceivePacket( buffer, strlen(buffer), 0, 0, 3);
	SLEEP(5);
	BOOST_REQUIRE( !decoder.size() );
	BOOST_REQUIRE( !renderer.size() );
	jitterBuffer->ReceivePacket( buffer2, strlen(buffer2), 0, 2, 3);
	SLEEP(5);
	BOOST_REQUIRE( !decoder.size() );
	BOOST_REQUIRE( !renderer.size() );

	jitterBuffer->ReceivePacket( zbuffer, strlen(zbuffer), 1, 0, 3);
	SLEEP(5);
	BOOST_REQUIRE( !decoder.size() );
	BOOST_REQUIRE( !renderer.size() );
	jitterBuffer->ReceivePacket( zbuffer2, strlen(zbuffer2), 1, 2, 3);
	SLEEP(5);
	BOOST_REQUIRE( !decoder.size() );
	BOOST_REQUIRE( !renderer.size() );
	jitterBuffer->ReceivePacket( zbuffer1, strlen(zbuffer1), 1, 1, 3);
	SLEEP(5);
	BOOST_REQUIRE( !decoder.size() );
	BOOST_REQUIRE( !renderer.size() );
	jitterBuffer->ReceivePacket( buffer1, strlen(buffer1), 0, 1, 3);
	SLEEP(5);
	BOOST_REQUIRE( decoder.size()==2 );
	BOOST_REQUIRE( renderer.size()==2 );

	std::string zexpectedResult(zbuffer);
	zexpectedResult.append(zbuffer1);
	zexpectedResult.append(zbuffer2);
	BOOST_REQUIRE( decoder.testDelevery(1, zexpectedResult.c_str(), zexpectedResult.length() ));
	{
		auto render = decoder.decodeFrame(zexpectedResult.c_str(),zexpectedResult.length());
		BOOST_REQUIRE( renderer.testDelevery(1, render.second.get(), render.first ));
	}

	std::string expectedResult(buffer);
	expectedResult.append(buffer1);
	expectedResult.append(buffer2);
	BOOST_REQUIRE( decoder.testDelevery(0, expectedResult.c_str(), expectedResult.length() ));
	{
		auto render = decoder.decodeFrame(expectedResult.c_str(),expectedResult.length());
		BOOST_REQUIRE( renderer.testDelevery(0, render.second.get(), render.first ));
	}


}