#ifndef REMU_BMC2835_RPIEMULATOR_H
#define REMU_BCM2835_RPIEMULATOR_H

#include "emulator.h"

namespace remu {

class RPiEmulator : public Emulator
{
public:
  RPiEmulator(const EmulatorOptions &opt)
    : Emulator(opt)
  { }
};

} /*namespace remu*/

#endif /*REMU_BCM2835_RPIEMULATOR_H*/

