#include "remu/host/File.h"
#include "remu/loader/Loader.h"
#include "remu/EmulationException.h"

using remu::host::File;
using remu::loader::Target;

namespace {

class ElfFile
{
public:
  ElfFile(File &file)
    : file(file)
  {
    file.readBuffer(ident, EI_NIDENT);

    validateIdent();

    file.setEndian(isLittleEndian() ? File::LITTLE : File::BIG);

    if (is32Bit())
    {
      throw remu::EmulationException("32Bit read not implemented for ELF Header");
    }
    else
    {
      type      = file.read<uint16_t>();
      machine   = file.read<uint16_t>();
      version   = file.read<uint32_t>();
      entry     = file.read<uint64_t>();
      phoff     = file.read<uint64_t>();
      shoff     = file.read<uint64_t>();
      flags     = file.read<uint32_t>();
      ehsize    = file.read<uint16_t>();
      phentsize = file.read<uint16_t>();
      phnum     = file.read<uint16_t>();
      shentsize = file.read<uint16_t>();
      shnum     = file.read<uint16_t>();
      shstrndx  = file.read<uint16_t>();
    }

    if (type != ET_EXEC)
    {
      throw remu::EmulationException("ELF Type was not ET_EXEC:  %d", type);
    }
  }

  void load(Target &target)
  {
    for (uint16_t ii = 0; ii < phnum; ii++)
    {
      ProgramHeader phdr;

      file.seek(phoff + (ii * phentsize));
      if (is32Bit())
      {
        throw remu::EmulationException("32Bit read not implemented for ELF PHeader");
      }
      else
      {
        phdr.type   = file.read<uint32_t>();
        phdr.flags  = file.read<uint32_t>();
        phdr.offset = file.read<uint64_t>();
        phdr.vaddr  = file.read<uint64_t>();
        phdr.paddr  = file.read<uint64_t>();
        phdr.filesz = file.read<uint64_t>();
        phdr.memsz  = file.read<uint64_t>();
        phdr.align  = file.read<uint64_t>();
      }

      if (PT_LOAD == phdr.type)
      {
        uint8_t *buffer = target.bufferForRegion(phdr.paddr, phdr.memsz);

        if (!buffer)
        {
          throw remu::EmulationException("No buffer given for ELF segment %d:  base=%08lx size=%08lx",
                                         ii, phdr.paddr, phdr.memsz);
        }

        file.seek(phdr.offset);
        file.readBuffer(buffer, phdr.filesz);
      }
    }
  }

  uint64_t getEntry() const {
    return entry;
  }

private:
  static const int EI_CLASS   = 4;
  static const int EI_DATA    = 5;
  static const int EI_VERSION = 6;
  static const int EI_NIDENT  = 16;

  static const uint8_t ELFCLASS32 = 1;
  static const uint8_t ELFCLASS64 = 2;

  static const uint8_t ELFDATA2LSB = 1;
  static const uint8_t ELFDATA2MSB = 2;

  static const uint8_t EV_CURRENT = 1;

  static const uint16_t ET_EXEC = 2;

  static const uint32_t PT_LOAD = 1;

  struct ProgramHeader
  {
    uint32_t type;
    uint32_t flags;
    uint64_t offset;
    uint64_t vaddr;
    uint64_t paddr;
    uint64_t filesz;
    uint64_t memsz;
    uint64_t align;
  };

  File &file;

  uint8_t ident[EI_NIDENT];
  uint16_t type;
  uint16_t machine;
  uint32_t version;
  uint64_t entry;
  uint64_t phoff;
  uint64_t shoff;
  uint32_t flags;
  uint16_t ehsize;
  uint16_t phentsize;
  uint16_t phnum;
  uint16_t shentsize;
  uint16_t shnum;
  uint16_t shstrndx;

  bool is32Bit() {
    return ident[EI_CLASS] == ELFCLASS32;
  }

  bool isLittleEndian() {
    return ident[EI_DATA] == ELFDATA2LSB;
  }

  void validateIdent() {
    if (ident[0] != 0x7F || ident[1] != 0x45 || 
        ident[2] != 0x4c || ident[3] != 0x46)
    {
       throw remu::EmulationException("ELF Magic invalid:  %02x %02x %02x %02x",
                                      ident[0], ident[1], ident[2], ident[3]);
    }

    if ((ident[EI_CLASS] != ELFCLASS32) && (ident[EI_CLASS] != ELFCLASS64))
    {
      throw remu::EmulationException("Unknown ELFCLASS %02x", ident[EI_CLASS]);
    }

    if ((ident[EI_DATA] != ELFDATA2LSB) && (ident[EI_DATA] != ELFDATA2MSB))
    {
      throw remu::EmulationException("Unknown ELFDATA %02x", ident[EI_DATA]);
    }

    if (ident[EI_VERSION] != EV_CURRENT)
    {
      throw remu::EmulationException("Unknown ELFVERSION %02x", ident[EI_VERSION]);
    }
  }
};

} /*anonymous namespace*/

namespace remu { namespace loader {

void loadElf(Target &target, const std::string &filename, uint64_t &entry)
{
  File file(filename);

  if (!file.isOpen())
  {
    throw EmulationException("Unable to open ELF file:  %s", filename.c_str());
  }

  ElfFile elf(file);

  elf.load(target);

  entry = elf.getEntry();
}

}} /*namespace remu::loader*/

