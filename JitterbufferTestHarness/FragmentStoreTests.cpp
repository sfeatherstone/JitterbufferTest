#include "stdafx.h"
#include "..\JitterBuffer\FragmentStore.h"

class IFrameSinkTestImpl:public IFrameSink
{
	public:
		IFrameSinkTestImpl():frameNo_(-1),lengthOfBuffer_(-1){};
		virtual void Frame(int frameNo, int lengthOfBuffer, boost::shared_array<char> buffer)
		{
			frameNo_=frameNo;
			lengthOfBuffer_=lengthOfBuffer;
			buffer_ = buffer;
		}
		int frameNo_;
		int lengthOfBuffer_;
		boost::shared_array<char> buffer_;
};

BOOST_AUTO_TEST_CASE(FragmentStoreCanStoreSingleItem)
{
	auto sink = std::make_shared<IFrameSinkTestImpl>();
	FragmentStore fs(sink);

	char *buffer = "12345678890";

	fs.ReceivePacket(buffer, strlen(buffer), 0, 0, 1);
	BOOST_REQUIRE( sink->frameNo_ == 0 );
	BOOST_REQUIRE( sink->lengthOfBuffer_ == strlen(buffer) );
	std::string expectedResult(buffer);
	BOOST_REQUIRE(!memcmp(expectedResult.c_str(),sink->buffer_.get(),sink->lengthOfBuffer_));
}

BOOST_AUTO_TEST_CASE(FragmentStoreCanStoreManyItems)
{
	char *buffer = "12345678890";
	char *buffer1 = "12345678890abcderf";
	char *buffer2 = "abcderf12345678890";

	auto sink = std::make_shared<IFrameSinkTestImpl>();
	FragmentStore fs(sink);

	fs.ReceivePacket( buffer, strlen(buffer), 0, 0, 3);
	BOOST_REQUIRE( sink->frameNo_ == -1);
	fs.ReceivePacket( buffer2, strlen(buffer2), 0, 2, 3);
	BOOST_REQUIRE( sink->frameNo_ == -1);
	fs.ReceivePacket( buffer1, strlen(buffer1), 0, 1, 3);

	std::string expectedResult(buffer);
	expectedResult.append(buffer1);
	expectedResult.append(buffer2);
	BOOST_REQUIRE( sink->frameNo_ == 0 );
	BOOST_REQUIRE( sink->lengthOfBuffer_ == expectedResult.length() );
	BOOST_REQUIRE(!memcmp(expectedResult.c_str(),sink->buffer_.get(),sink->lengthOfBuffer_));
}

BOOST_AUTO_TEST_CASE(FragmentStoreCanStoreManyUnorderedFrames)
{
	char *buffer = "12345678890";
	char *buffer1 = "12345678890abcderf";
	char *buffer2 = "abcderf12345678890";

	char *zbuffer = "z12345678890";
	char *zbuffer1 = "z12345678890abcderf";
	char *zbuffer2 = "zabcderf12345678890";

	auto sink = std::make_shared<IFrameSinkTestImpl>();
	FragmentStore fs(sink);

	fs.ReceivePacket( buffer, strlen(buffer), 0, 0, 3);
	BOOST_REQUIRE( sink->frameNo_ == -1);
	fs.ReceivePacket( buffer2, strlen(buffer2), 0, 2, 3);
	BOOST_REQUIRE( sink->frameNo_ == -1);

	fs.ReceivePacket( zbuffer, strlen(zbuffer), 1, 0, 3);
	BOOST_REQUIRE( sink->frameNo_ == -1);
	fs.ReceivePacket( zbuffer2, strlen(zbuffer2), 1, 2, 3);
	BOOST_REQUIRE( sink->frameNo_ == -1);
	fs.ReceivePacket( zbuffer1, strlen(zbuffer1), 1, 1, 3);

	std::string zexpectedResult(zbuffer);
	zexpectedResult.append(zbuffer1);
	zexpectedResult.append(zbuffer2);
	BOOST_REQUIRE( sink->frameNo_ == 1 );
	BOOST_REQUIRE( sink->lengthOfBuffer_ == zexpectedResult.length() );
	BOOST_REQUIRE(!memcmp(zexpectedResult.c_str(),sink->buffer_.get(),sink->lengthOfBuffer_));

	fs.ReceivePacket( buffer1, strlen(buffer1), 0, 1, 3);
	std::string expectedResult(buffer);
	expectedResult.append(buffer1);
	expectedResult.append(buffer2);
	BOOST_REQUIRE( sink->frameNo_ == 0 );
	BOOST_REQUIRE( sink->lengthOfBuffer_ == expectedResult.length() );
	BOOST_REQUIRE(!memcmp(expectedResult.c_str(),sink->buffer_.get(),sink->lengthOfBuffer_));
}

