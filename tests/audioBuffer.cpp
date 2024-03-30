#include "src/audioBuffer.hpp"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

TEST_CASE("AudioBuffer")
{
	using namespace mcl;

	static const int BUFFER_SIZE = 4096;

	AudioBuffer buffer;
	buffer.alloc(BUFFER_SIZE, 2);

	/* Fill it with fake data 0...BUFFERSIZE-1 */
	buffer.forEachFrame([](float* channels, int numFrame) {
		channels[0] = static_cast<float>(numFrame);
		channels[1] = static_cast<float>(numFrame);
	});

	SECTION("test allocation")
	{
		SECTION("test mono")
		{
			buffer.alloc(BUFFER_SIZE, 1);
			REQUIRE(buffer.countFrames() == BUFFER_SIZE);
			REQUIRE(buffer.countSamples() == BUFFER_SIZE);
			REQUIRE(buffer.countChannels() == 1);
		}

		SECTION("test stereo")
		{
			REQUIRE(buffer.countFrames() == BUFFER_SIZE);
			REQUIRE(buffer.countSamples() == BUFFER_SIZE * 2);
			REQUIRE(buffer.countChannels() == 2);
		}

		buffer.free();

		REQUIRE(buffer.countFrames() == 0);
		REQUIRE(buffer.countSamples() == 0);
		REQUIRE(buffer.countChannels() == 0);
	}

	SECTION("test clear all")
	{
		buffer.clear();
		for (int i = 0; i < buffer.countFrames(); i++)
			for (int k = 0; k < buffer.countChannels(); k++)
				REQUIRE(buffer[i][k] == 0.0f);
		buffer.free();
	}

	SECTION("test clear range")
	{
		for (int i = 0; i < buffer.countFrames(); i++)
			for (int k = 0; k < buffer.countChannels(); k++)
				buffer[i][k] = 1.0f;

		buffer.clear(5, 6);

		for (int k = 0; k < buffer.countChannels(); k++)
			REQUIRE(buffer[5][k] == 0.0f);

		buffer.free();
	}

	SECTION("test copy")
	{
		AudioBuffer other(BUFFER_SIZE, 2);

		SECTION("test full copy")
		{
			other.set(buffer, 1.0f);

			REQUIRE(other[0][0] == 0.0f);
			REQUIRE(other[16][0] == 16.0f);
			REQUIRE(other[128][0] == 128.0f);
			REQUIRE(other[1024][0] == 1024.0f);
			REQUIRE(other[BUFFER_SIZE - 1][0] == static_cast<float>(BUFFER_SIZE - 1));
		}
	}
}
