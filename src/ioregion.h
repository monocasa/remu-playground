#ifndef REMU_IOTREE_H
#define REMU_IOTREE_H

#include <cstdint>
#include <list>

class IoRegion
{
public:
  virtual uint64_t readIo(uint64_t offset, unsigned int size) = 0;
  virtual void writeIo(uint64_t offset, uint64_t val, unsigned int size) = 0;

  uint32_t base;
  uint32_t length;
};

/* TODO: THM: switch this to a more appropriate data structure once I start caring */
class IoMap
{
public:
  IoRegion* getRegionForAddr(uint32_t addr) {
    for( auto region : regions ) {
      if( region->base <= addr && ((region->base + region->length) > addr) ) {
        return region;
      }
    }
    return nullptr;
  }

  void addRegion(IoRegion *region) {
    regions.push_back(region);
  }

  void removeRegion(IoRegion *region) {
    regions.remove(region);
  }

private:
  std::list<IoRegion*> regions;
};

#endif /*REMU_IOTREE_H*/

