#ifndef REMU_CPU_H
#define REMU_CPU_H

#include "arm/opcode.h"

#include <cstdint>

namespace remu {
class Emulator;
class Memory;
} /*namespace remu*/

namespace remu { namespace arm {

/**
 * ARM condition codes
 */
typedef enum
{
  CC_EQ = 0x00,
  CC_NE = 0x01,
  CC_CS = 0x02,
  CC_CC = 0x03,
  CC_MI = 0x04,
  CC_PL = 0x05,
  CC_VS = 0x06,
  CC_VC = 0x07,
  CC_HI = 0x08,
  CC_LS = 0x09,
  CC_GE = 0x0a,
  CC_LT = 0x0b,
  CC_GT = 0x0c,
  CC_LE = 0x0d,
  CC_AL = 0x0e
} armCond_t;

/**
 * ARM registers
 */
typedef enum
{
  R0  = 0x0,
  R1  = 0x1,
  R2  = 0x2,
  R3  = 0x3,
  R4  = 0x4,
  R5  = 0x5,
  R6  = 0x6,
  R7  = 0x7,
  R8  = 0x8,
  R9  = 0x9,
  R10 = 0xA,
  R11 = 0xB,
  R12 = 0xC,
  SP  = 0xD,
  LR  = 0xE,
  PC  = 0xF
} armReg_t;

/**
 * ARM operating modes
 */
typedef enum
{
  MODE_USR = 0x10,
  MODE_FIQ = 0x11,
  MODE_IRQ = 0x12,
  MODE_SVC = 0x13,
  MODE_ABT = 0x17,
  MODE_UND = 0x1B,
  MODE_SYS = 0x1F
} armMode_t;

/**
 * CPU data - registers, flags etc
 */
class Cpu
{
public:
  Cpu(remu::Emulator *emu, remu::Memory *memory, uint32_t start_addr);

  void tick();
  void dump();

  remu::Emulator  *emu;
  remu::Memory    *memory;

  /* User mode registers */
  union
  {
    int32_t r[16];
    struct
    {
      int32_t r0;
      int32_t r1;
      int32_t r2;
      int32_t r3;
      int32_t r4;
      int32_t r5;
      int32_t r6;
      int32_t r7;
      int32_t r8;
      int32_t r9;
      int32_t r10;
      int32_t r11;
      int32_t r12;
      int32_t sp;
      int32_t lr;
      int32_t pc;
    } reg;
  } r_usr;

  /* Banked FIQ */
  union
  {
    int32_t r[7];
    struct
    {
      int32_t r8;
      int32_t r9;
      int32_t r10;
      int32_t r11;
      int32_t r12;
      int32_t r13;
      int32_t r14;
    } reg;
  } r_fiq;

  /* Banked SVC */
  union
  {
    int32_t r[2];
    struct
    {
      int32_t r13;
      int32_t r14;
    } reg;
  } r_svc;

  /* Banked UND */
  union
  {
    int32_t r[2];
    struct
    {
      int32_t r13;
      int32_t r14;
    } reg;
  } r_und;

  /* Banked ABT */
  union
  {
    int32_t r[2];
    struct
    {
      int32_t r13;
      int32_t r14;
    } reg;
  } r_abt;

  /* Banked IRQ */
  union
  {
    int32_t r[2];
    struct
    {
      int32_t r13;
      int32_t r14;
    } reg;
  } r_irq;

  /* Saved program status register */
  struct
  {
    uint32_t svc;
    uint32_t abt;
    uint32_t und;
    uint32_t irq;
    uint32_t fiq;
  } spsr;

  /* Current program status register */
  union
  {
    uint32_t r;
    struct
    {
      uint32_t m:5;
      uint32_t t:1;
      uint32_t f:1;
      uint32_t i:1;
      uint32_t  :20;
      uint32_t v:1;
      uint32_t c:1;
      uint32_t z:1;
      uint32_t n:1;
    } b;
  } cpsr;

  /* VFP Registers */
  struct
  {
    /* General Purpose */
    uint32_t s[32];

    /* FPSID */
    uint32_t fpsid;

    /* FPSCR */
    union
    {
      uint32_t r;
      struct
      {
        uint32_t ioc    : 1;
        uint32_t dzc    : 1;
        uint32_t ofc    : 1;
        uint32_t ufc    : 1;
        uint32_t ixc    : 1;
        uint32_t        : 3;
        uint32_t ioe    : 1;
        uint32_t dze    : 1;
        uint32_t ofe    : 1;
        uint32_t ufe    : 1;
        uint32_t ixe    : 1;
        uint32_t        : 3;
        uint32_t len    : 3;
        uint32_t        : 1;
        uint32_t stride : 2;
        uint32_t rmode  : 2;
        uint32_t fz     : 1;
        uint32_t        : 3;
        uint32_t flags  : 4;
      } b;
    } fpscr;

    /* FPEXC */
    union
    {
      uint32_t r;
      struct
      {
        uint32_t        : 30;
        uint32_t en     : 1;
        uint32_t ex     : 1;
      } b;
    } fpexc;
  } vfp;

  uint32_t readRegister(int reg) const;
  void writeRegister(int reg, uint32_t value);

  void vfpInit();
  void vfpDump() const;

  void vfpDataProc(op_coproc_data_proc_t* instr);
  void vfpDataTransfer(op_coproc_data_transfer_t* instr);
  void vfpRegTransfer(op_coproc_reg_transfer_t* instr);
};

}} /*namespace remu::arm*/

#endif /* REMU_CPU_H */

