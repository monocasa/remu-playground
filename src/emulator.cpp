#include "common.h"
#include <sys/time.h>

Emulator::Emulator(const EmulatorOptions &opt)
  : err_msg( nullptr )
  , memory(this, opt.mem_size)
  , mbox(this)
  , terminated( false )
  , image( opt.image )
  , mem_size( opt.mem_size )
  , start_addr( opt.start_addr )
  , graphics( opt.graphics )
  , usage( opt.usage )
  , quiet( opt.quiet )
  , nes_enabled( opt.nes_enabled )
  , gpio_test_offset( opt.gpio_test_offset )
  , system_timer_base( getTime() * 1000 )
  , last_refresh( 0 )
{
  memset(&cpu,    0, sizeof(cpu));
  memset(&vfp,    0, sizeof(vfp));
}

Emulator::~Emulator()
{
  fb_destroy(&fb);
  delete gpio;
  delete pr;
  delete nes;
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
  gpio = new Gpio(*this, memory);
  fb_init(&fb, this, &memory, gpio);
  pr = new Peripheral(*this);

  if( nes_enabled ) {
    nes = new Nes(*this, *gpio, &fb);
  }
}

/**
 * Loads a binary image into memory
 */
void Emulator::load()
{
  FILE *finput;
  size_t file_size;
  void* memory_start = memory.data + start_addr;

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
    fatal("Not enough memory for kernel");
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
      fb_tick(&fb);
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
  int size = 100;
  int n;
  char* str = NULL;
  va_list ap;

  if (quiet)
  {
    return;
  }

  str = (char*)malloc(size);
  assert(str);

  while (1)
  {
    va_start(ap, fmt);
    n = vsnprintf(str, size, fmt, ap);
    va_end(ap);

    if (-1 < n && n < size)
    {
      break;
    }

    size = n > -1 ? (n + 1) : (size << 1);
    str = (char*)realloc(err_msg, size);
    assert(str);
  }

  printf("Info: %s\n", str);
}

/**
 * Prints an error message and exits if the strict flag is set
 *
 * @param fmt Printf-like format string
 */
void Emulator::error(const char * fmt, ...)
{
  int size = 100, n;
  char* tmp = NULL;
  va_list ap;

  if (quiet)
  {
    return;
  }

  err_msg = (char*)malloc(size);
  assert(err_msg);

  while (1)
  {
    va_start(ap, fmt);
    n = vsnprintf(err_msg, size, fmt, ap);
    va_end(ap);

    if (-1 < n && n < size)
    {
      break;
    }

    size = n > -1 ? (n + 1) : (size << 1);
    tmp = (char*)realloc(err_msg, size);
    assert(tmp);

    err_msg = tmp;
  }

  printf("Error: %s\n", err_msg);
}

/**
 * Kills the emulator, printing a message to stdout
 *
 * @param fmt Printf-like format string
 */
void Emulator::fatal(const char * fmt, ...)
{
  int size = 100, n;
  char * tmp;
  va_list ap;

  err_msg = (char*)malloc(size);
  assert(err_msg);

  while (1)
  {
    va_start(ap, fmt);
    n = vsnprintf(err_msg, size, fmt, ap);
    va_end(ap);

    if (-1 < n && n < size)
    {
      break;
    }

    size = n > -1 ? (n + 1) : (size << 1);
    if (!(tmp = (char*)realloc(err_msg, size)))
    {
      free(err_msg);
      break;
    }

    err_msg = tmp;
  }

  longjmp(err_jmp, 1);
}

