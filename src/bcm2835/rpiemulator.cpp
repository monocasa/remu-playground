#include "bcm2835/rpiemulator.h"
#include "emulationexception.h"

namespace remu {

RPiEmulator::RPiStubRegion::RPiStubRegion(uint32_t base, uint32_t length, Memory &mem)
  : StubRegion(base, length)
  , mem(mem)
{
  mem.addRegion(this);
}

RPiEmulator::RPiStubRegion::~RPiStubRegion()
{
  mem.removeRegion(this);
}

/**
 * Loads a binary image into memory
 */
void RPiEmulator::load()
{
  FILE *finput;
  size_t file_size;
  void* memory_start = memory.getDramArrayBase() + opt.start_addr;

  /* Throw error if file unopenable. */
  if (!(finput = fopen(opt.image, "rb")))
  {
    error("Cannot open file '%s'", opt.image);
  }

  fseek(finput, 0L, SEEK_END);
  file_size = ftell(finput);
  fseek(finput, 0L, SEEK_SET);

  /* Check for buffer overflow */
  if(opt.start_addr + file_size > opt.mem_size)
  {
    throw EmulationException("Not enough memory for kernel");
  }

  /* Copy instructions into memory and error if incomplete */
  if (fread(memory_start, 1, file_size, finput) != file_size)
  {
    error("Could not read entire file '%s'", opt.image);
  }

  fclose(finput);
}

void RPiEmulator::execute()
{
  while (isRunning())
  {
    tick();
  }
}

/**
 * Prints out the state of the emulator
 */
void RPiEmulator::dump()
{
  cpu_dump(&cpu);
  memory.dump();
}

void RPiEmulator::tick()
{
  cpu_tick(&cpu);

  /* When graphics are emulated, we execute a screen refresh after 34ms has
   * passed (30 frames per second) */
  const uint32_t frame_time = 20;

  const uint64_t now = getTime();
  if ((now - last_refresh) > frame_time)
  {
    fb.tick();
    last_refresh = now;
  }
}

} /*namespace remu*/
 
