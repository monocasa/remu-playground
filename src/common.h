#ifndef REMU_COMMON_H
#define REMU_COMMON_H

/* Standard C */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <getopt.h>
#include <setjmp.h>
#include <assert.h>
#include <time.h>
#include <math.h>

/* SDL */
#include <SDL/SDL.h>

/* Useful macros */
#ifdef __GNUC__
#  define UNUSED(x) UNUSED_ ## x __attribute__((__unused__))
#  define UNUSED_FUNCTION(x) __attribute__((__unused__)) UNUSED_ ## x
#else
#  define UNUSED(x) UNUSED_ ## x
#  define UNUSED_FUNCTION(x) UNUSED_ ## x
#endif

/* Emulator forward declaration */
class Emulator;

/* Modules */
#include "memory.h"
#include "opcode.h"
#include "vfp.h"
#include "cpu.h"
#include "nes.h"
#include "bcm2835/gpio.h"
#include "bcm2835/mbox.h"
#include "bcm2835/framebuffer.h"
#include "bcm2835/peripheral.h"

/* Emulator */
#include "emulator.h"

#endif /*REMU_COMMON_H */

