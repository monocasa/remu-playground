#ifndef REMU_LOADER_ELFLOADER_H
#define REMU_LOADER_ELFLOADER_H

#include <cstdint>
#include <cstdlib>
#include <string>

namespace remu { namespace loader {

class Target
{
public:
  virtual uint8_t* bufferForRegion(uint64_t base_addr, size_t size) = 0;
};

void loadElf(Target &target, const std::string &filename, uint64_t &entry);

}} /*namespace remu::loader*/

#endif /*REMU_LOADER_ELFLOADER_H*/

