#include <util/bitops.h>

#include <gtest/gtest.h>

#include <cstdint>

using namespace remu::util::bitops;

TEST(bitops, signExtend)
{
	int32_t value = signExtend<int32_t, 2>(0x02);
	EXPECT_EQ(-2, value);
}

