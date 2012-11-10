#include "stdafx.h"
#include "..\JitterBuffer\IJitterbuffer.h"
#include <memory>

BOOST_AUTO_TEST_CASE(CanCreateAndDeleteIJitterBuffer)
{
	auto jitterBuffer = std::make_shared<IJitterBuffer>(nullptr, nullptr);

    BOOST_REQUIRE(jitterBuffer);

}