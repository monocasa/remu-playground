#include <util/bitops.h>

#include <gtest/gtest.h>

#include <cstdint>

using namespace remu::util::bitops;

TEST(bitops, rotateRight)
{
	uint32_t value = rotateRight<uint32_t>(0x55555555, 1);
	EXPECT_EQ(0xAAAAAAAA, value);

	value = rotateRight<uint32_t>(0xFF, 8);
	EXPECT_EQ(0xFF000000, value);
}

TEST(bitops, signExtend)
{
	int32_t value = signExtend<int32_t, 2>(0x02);
	EXPECT_EQ(-2, value);
}

