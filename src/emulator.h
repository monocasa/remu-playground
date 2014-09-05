#ifndef REMU_EMULATOR_H
#define REMU_EMULATOR_H

struct EmulatorOptions
{
  EmulatorOptions()
	: image( nullptr )
    , mem_size( 0 )
    , start_addr( 0 )
    , graphics( 0 )
    , usage( 0 )
    , quiet( 0 )
    , nes_enabled( 0 )
    , gpio_test_offset( 0 )
  { }

  const char   *image;

  size_t        mem_size;
  uint32_t      start_addr;
  int           graphics;
  int           usage;
  int           quiet;
  int           nes_enabled;
  int           gpio_test_offset;
};

/**
 * Emulator state
 */
class Emulator
{
public:
  Emulator(const EmulatorOptions &opt);

  virtual ~Emulator();

  int           terminated;

  /* Kernel image */
  const char   *image;

  /* Error handling */
  jmp_buf       err_jmp;
  char*         err_msg;

  /* Arguments */
  size_t        mem_size;
  uint32_t      start_addr;
  int           graphics;
  int           usage;
  int           quiet;
  int           nes_enabled;
  int           gpio_test_offset;

  /* Modules */
  framebuffer_t fb;
  memory_t      memory;
  cpu_t         cpu;
  gpio_t        gpio;
  mbox_t        mbox;
  peripheral_t  pr;
  vfp_t         vfp;
  nes_t         nes;

  /* System Timer */
  uint64_t      system_timer_base;

  /* Refresh */
  uint64_t      last_refresh;
};

void emulator_init(Emulator* );
int emulator_is_running(Emulator* );
uint64_t emulator_get_time();
uint64_t emulator_get_system_timer(Emulator*);
void emulator_tick(Emulator* );
void emulator_info(Emulator*, const char *, ...);
void emulator_error(Emulator*, const char *, ...);
void emulator_fatal(Emulator*, const char *, ...) __attribute__((noreturn));
void emulator_dump(Emulator* );
void emulator_load(Emulator* , const char*);

#endif /* REMU_EMULATOR_H */

