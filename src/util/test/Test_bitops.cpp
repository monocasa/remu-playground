#include <util/bitops.h>

#include <gtest/gtest.h>

#include <cstdint>

using namespace remu::util::bitops;

TEST(bitops, byteSwap16)
{
	uint16_t value = byteSwap<uint16_t>(0xFEDC);
	EXPECT_EQ(0xDCFE, value);
}

TEST(bitops, byteSwap32)
{
	uint32_t value = byteSwap<uint32_t>(0xFEDCBA98);
	EXPECT_EQ(0x98BADCFE, value);
}

TEST(bitops, byteSwap64)
{
	uint64_t value = byteSwap<uint64_t>(0xFEDCBA9876543210UL);
	EXPECT_EQ(0x1032547698BADCFEUL, value);
}

TEST(bitops, rotateLeft)
{
	uint32_t value = rotateLeft<uint32_t>(0xAAAAAAAA, 1);
	EXPECT_EQ(0x55555555, value);

	value = rotateLeft<uint32_t>(0xFF000000, 8);
	EXPECT_EQ(0xFF, value);
}

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

