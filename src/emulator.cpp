#include "bitbang/nes.h"
#include "common.h"
#include "emulationexception.h"
#include <sys/time.h>

namespace remu {

Emulator::Emulator(const EmulatorOptions &opt)
  : terminated( false )
  , image( opt.image )
  , mem_size( opt.mem_size )
  , start_addr( opt.start_addr )
  , graphics( opt.graphics )
  , usage( opt.usage )
  , quiet( opt.quiet )
  , nes_enabled( opt.nes_enabled )
  , system_timer_base( getTime() * 1000 )
{ }

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

