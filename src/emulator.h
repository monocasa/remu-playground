#ifndef REMU_EMULATOR_H
#define REMU_EMULATOR_H

#include "ui.h"

#include <cstdint>
#include <cstdlib>

namespace remu {

namespace bitbang {
class Nes;
} /*namespace bitbang*/

struct EmulatorOptions
{
  EmulatorOptions()
	: image( nullptr )
    , mem_size( 0 )
    , start_addr( 0 )
    , usage( 0 )
    , quiet( 0 )
    , nes_enabled( 0 )
    , gpio_test_offset( 0 )
  { }

  const char   *image;

  size_t        mem_size;
  uint32_t      start_addr;
  int           usage;
  int           quiet;
  int           nes_enabled;
  int           gpio_test_offset;
};

/**
 * Emulator state
 */
class Emulator : public Ui
{
public:
  Emulator(const EmulatorOptions &opt);

  virtual ~Emulator() = default;

  bool isRunning() const;
  uint64_t getSystemTimer() const;

  void info(const char *fmt, ...) override final;
  void error(const char *fmt, ...) override final;

  virtual void execute() = 0;
  virtual void dump() = 0;

  void terminate() {
    terminated = true;
  }

protected:
  uint64_t getTime() const;

private:
  int           terminated;

  /* Kernel image */
  const char   *image;

  /* Arguments */
  size_t        mem_size;
  uint32_t      start_addr;
  int           usage;
  int           quiet;
  int           nes_enabled;

  /* System Timer */
  uint64_t      system_timer_base;
};

} /*namespace remu*/

#endif /* REMU_EMULATOR_H */

