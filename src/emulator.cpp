#include "bitbang/nes.h"
#include "common.h"
#include "emulationexception.h"
#include <sys/time.h>

namespace remu {

Emulator::Emulator(const EmulatorOptions &opt)
  : memory(this, opt.mem_size)
  , fb(opt.mem_size)
  , mbox(this, memory)
  , terminated( false )
  , image( opt.image )
  , mem_size( opt.mem_size )
  , start_addr( opt.start_addr )
  , graphics( opt.graphics )
  , usage( opt.usage )
  , quiet( opt.quiet )
  , nes_enabled( opt.nes_enabled )
  , system_timer_base( getTime() * 1000 )
  , last_refresh( 0 )
{
  memset(&cpu,    0, sizeof(cpu));
  memset(&vfp,    0, sizeof(vfp));
}

Emulator::~Emulator()
{
  fb_destroy(&fb);
  cpu_destroy(&cpu);
  vfp_destroy(&vfp);
}

/**
 * Initialises the emulator
 */
void Emulator::init()
{
  cpu_init(&cpu, this);
  vfp_init(&vfp, this);
  fb_init(&fb, this, &memory);
}

/**
 * Loads a binary image into memory
 */
void Emulator::load()
{
  FILE *finput;
  size_t file_size;
  void* memory_start = memory.getDramArrayBase() + start_addr;

  /* Throw error if file unopenable. */
  if (!(finput = fopen(image, "rb")))
  {
    error("Cannot open file '%s'", image);
  }

  fseek(finput, 0L, SEEK_END);
  file_size = ftell(finput);
  fseek(finput, 0L, SEEK_SET);

  /* Check for buffer overflow */
  if(start_addr + file_size > mem_size)
  {
    throw EmulationException("Not enough memory for kernel");
  }

  /* Copy instructions into memory and error if incomplete */
  if (fread(memory_start, 1, file_size, finput) != file_size)
  {
    error("Could not read entire file '%s'", image);
  }

  fclose(finput);
}

/**
 * Checks whether the emulator is still active
 */
bool Emulator::isRunning() const
{
  return !terminated;
}

/**
 * Get the current time in milliseconds
 */
uint64_t Emulator::getTime() const
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
}

/**
 * Get the value of the system timer
 */
uint64_t Emulator::getSystemTimer() const
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  uint64_t us = (tv.tv_sec) * 1000000ULL + tv.tv_usec;
  return us - system_timer_base;
}

/**
 * Executes a single step
 */
void Emulator::tick()
{
  cpu_tick(&cpu);

  /* When graphics are emulated, we execute a screen refresh after 34ms has
   * passed (30 frames per second) */
  uint32_t frame_time = 20;
  if (graphics)
  {
    uint64_t now = getTime();
    if ((now - last_refresh) > frame_time)
    {
      fb.tick();
      last_refresh = now;
    }
  }
}

/**
 * Prints out the state of the emulator
 */
void Emulator::dump()
{
  cpu_dump(&cpu);
  memory.dump();
}

/**
 * Prints a useful info message.
 *
 * @param fmt Printf-like format string
 */
void Emulator::info(const char * fmt, ...)
{
  fprintf(stderr, "Info: ");
  va_list va;
  va_start(va, fmt);
  vfprintf(stderr, fmt, va);
  fprintf(stderr, "\n");
  va_end(va);
}

/**
 * Prints an error message and exits if the strict flag is set
 *
 * @param fmt Printf-like format string
 */
void Emulator::error(const char * fmt, ...)
{
  fprintf(stderr, "Error: ");
  va_list va;
  va_start(va, fmt);
  vfprintf(stderr, fmt, va);
  fprintf(stderr, "\n");
  va_end(va);
}

} /*namespace remu */

