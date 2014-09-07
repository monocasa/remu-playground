#ifndef REMU_BMC2835_RPIEMULATOR_H
#define REMU_BCM2835_RPIEMULATOR_H

#include "emulator.h"

class RPiEmulator : public Emulator
{
public:
  RPiEmulator(const EmulatorOptions &opt)
    : Emulator(opt)
  { }
};

#endif /*REMU_BCM2835_RPIEMULATOR_H*/

