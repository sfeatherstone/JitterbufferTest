#include "stdafx.h"
#include "..\JitterBuffer\UnrenderedFrame.h"

BOOST_AUTO_TEST_CASE(UnrenderedFrameCanStoreSingleItem)
{
	char *buffer = "12345678890";
	UnrenderedFrame fs(1);

	BOOST_REQUIRE(fs.addFragment(0, buffer, strlen(buffer)));
}

BOOST_AUTO_TEST_CASE(UnrenderedFrameCanStoreManyItems)
{
	char *buffer = "12345678890";
	char *buffer2 = "12345678890abcderf";
	char *buffer3 = "abcderf12345678890";
	UnrenderedFrame fs(3);

	BOOST_REQUIRE(!fs.addFragment(0, buffer, strlen(buffer)));
	BOOST_REQUIRE(fs.getFrame().first == -1);
	BOOST_REQUIRE(!fs.addFragment(1, buffer2, strlen(buffer2)));
	BOOST_REQUIRE(fs.getFrame().first == -1);
	BOOST_REQUIRE(fs.addFragment(2, buffer3, strlen(buffer3)));
	auto result = fs.getFrame();
	BOOST_REQUIRE(result.first == strlen(buffer) + strlen(buffer2) + strlen(buffer3));
	std::string expectedResult(buffer);
	expectedResult.append(buffer2);
	expectedResult.append(buffer3);
	BOOST_REQUIRE(!memcmp(expectedResult.c_str(),result.second.get(),result.first));
}

BOOST_AUTO_TEST_CASE(UnrenderedFrameCanStoreManyUnorderedItems)
{
	char *buffer = "12345678890";
	char *buffer2 = "12345678890abcderf";
	char *buffer3 = "abcderf12345678890";
	UnrenderedFrame fs(3);

	BOOST_REQUIRE(!fs.addFragment(1, buffer2, strlen(buffer2)));
	BOOST_REQUIRE(fs.getFrame().first == -1);
	BOOST_REQUIRE(!fs.addFragment(2, buffer3, strlen(buffer3)));
	BOOST_REQUIRE(fs.getFrame().first == -1);
	BOOST_REQUIRE(fs.addFragment(0, buffer, strlen(buffer)));
	auto result = fs.getFrame();
	BOOST_REQUIRE(result.first == strlen(buffer) + strlen(buffer2) + strlen(buffer3));
	std::string expectedResult(buffer);
	expectedResult.append(buffer2);
	expectedResult.append(buffer3);
	BOOST_REQUIRE(!memcmp(expectedResult.c_str(),result.second.get(),result.first));
}
