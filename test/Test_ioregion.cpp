#include "ioregion.h"

#include <gtest/gtest.h>

class TestIoRegion : public remu::IoRegion
{
public:
  TestIoRegion(uint32_t base, uint32_t length)
  {
    this->base = base;
    this->length = length;
  }

  uint64_t readIo(uint64_t offset, unsigned int size) override final {
    (void)offset;
    (void)size;
    return 0xAAAAAAAAAAAAAAAAULL;
  }

  void writeIo(uint64_t offset, uint64_t val, unsigned int size) override final {
    (void)offset;
    (void)val;
    (void)size;
  }
};

TEST(IoMap, Init)
{
  TestIoRegion region1(0x100, 0x20);
  TestIoRegion region2(0x10, 0x40);
  remu::IoMap iomap;

  iomap.addRegion(&region1);
  iomap.addRegion(&region2);

  EXPECT_EQ(nullptr,  iomap.getRegionForAddr(0x0FF));
  EXPECT_EQ(&region1, iomap.getRegionForAddr(0x100));
  EXPECT_EQ(&region1, iomap.getRegionForAddr(0x11F));
  EXPECT_EQ(nullptr,  iomap.getRegionForAddr(0x120));

  EXPECT_EQ(nullptr,  iomap.getRegionForAddr(0x0F));
  EXPECT_EQ(&region2, iomap.getRegionForAddr(0x10));
  EXPECT_EQ(&region2, iomap.getRegionForAddr(0x4F));
  EXPECT_EQ(nullptr,  iomap.getRegionForAddr(0x50));
}

