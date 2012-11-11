#include "stdafx.h"
#include "..\JitterBuffer\IJitterbufferImpl.h"
#include <memory>
#include "TestDecoderImpl.h"
#include "TestRendererImpl.h"
#include "JitterBufferTestSource.h"
#include "TestHelpers.h"

BOOST_AUTO_TEST_CASE(BigTest1000Frames)
{
	TestRendererImpl renderer;
	TestDecoderImpl decoder;
	auto jitterBuffer = std::make_shared<IJitterBufferImpl>(&decoder, &renderer);

	int framesToTest = 1000;

	JitterBufferTestSource source(framesToTest);
	source.run(jitterBuffer.get());

	while (renderer.size()<framesToTest)
	{
		SLEEP(20);
	}
}


