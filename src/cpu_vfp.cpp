#include "common.h"

#include "cpu.h"
#include "emulationexception.h"
#include "emulator.h"

namespace remu {

/**
 * Initialises the vector floating point coprocessor
 * @param vfp VFP context
 * @param emu Emulator
 */
void
vfp_init(Cpu *cpu)
{
  /* Initialise registers */
  memset(&cpu->vfp.s, 0, sizeof(cpu->vfp.s));
  memset(&cpu->vfp.fpscr, 0, sizeof(cpu->vfp.fpscr));
  memset(&cpu->vfp.fpexc, 0, sizeof(cpu->vfp.fpexc));

  /* TODO: Initialise FPSID */
  cpu->vfp.fpsid = 0;
}

/**
 * Prints the state of the VFP coprocessor
 */
void
vfp_dump(Cpu *cpu)
{
  int i;
  union
  {
    float    f;
    uint32_t u;
  } reg;

  for (i = 0; i < 32; ++i)
  {
    reg.u = cpu->vfp.s[i];
    printf("s%02d: %f\n", i, reg.f);
  }
}

/**
 * Compare two floats and set FPSCR flags
 * @param vfp VFP context
 * @param a First value
 * @param b Second value
 * @param mode Set to 0 if no exceptions on quiet NaN's, 1 otherwise
 */
void
dp_fcmps(Cpu *cpu, float a, float b, uint32_t UNUSED(mode))
{
  /* Determine if a and b are unordered (NaNs) */
  if ((a != a) || (b != b))
  {
    /* NZCV = 0011 */
    cpu->vfp.fpscr.b.flags = 0x3;
  }
  else if (a == b)
  {
    /* NZCV = 0110 */
    cpu->vfp.fpscr.b.flags = 0x6;
  }
  else if (a < b)
  {
    /* NZCV = 1000 */
    cpu->vfp.fpscr.b.flags = 0x8;
  }
  else
  {
    /* NZCV = 0010 */
    cpu->vfp.fpscr.b.flags = 0x2;
  }
}

/**
 * Handles a data processing instruction
 * @param vfp VFP context
 * @param instr Instruction
 */
void
vfp_data_proc(Cpu *cpu, op_coproc_data_proc_t* instr)
{
  uint32_t opcode, Fd, Fn, Fm;

  /* Layout: CP_opcode = [p|D|q|r], CP = [N|s|M] */

  /* Extract Opcode - Layout: [p|q|r|s] */
  opcode = ((instr->CP_opcode >> 3) & 0x1) << 3;
  opcode |= ((instr->CP_opcode) & 0x3) << 1;
  opcode |= (instr->CP >> 1) & 0x1;

  /* Extract registers */
  /* Fd - Layout: [CRd|D] */
  Fd = instr->CRd << 1;
  Fd |= (instr->CP_opcode >> 2) & 0x1;

  /* Fn - Layout: [CRn|N] */
  Fn = instr->CRn << 1;
  Fn |= (instr->CP >> 2) & 0x1;

  /* Fm - Layout: [CRm|M] */
  Fm = instr->CRm << 1;
  Fm |= instr->CP & 0x1;

  /* Read data from the registers */
  union
  {
    uint32_t u;
    int32_t i;
    float f;
  } in, im, o;

  if (opcode != 0xf)
  {
    in.u = cpu->vfp.s[Fn];
  }
  else
  {
    in.u = 0;
  }

  im.u = cpu->vfp.s[Fm];
  o.u = cpu->vfp.s[Fd];

  /* Dispatch instruction */
  switch (opcode)
  {
    case 0x0: /* FMACS */
    {
      o.f = o.f + (in.f * im.f);
      break;
    }
    case 0x1: /* FNMACS */
    {
      o.f = o.f - (in.f * im.f);
      break;
    }
    case 0x2: /* FMSCS */
    {
      o.f = -o.f + (in.f * im.f);
      break;
    }
    case 0x3: /* FNMSCS */
    {
      o.f = -o.f - (in.f * im.f);
      break;
    }
    case 0x4: /* FMULS */
    {
      o.f = in.f * im.f;
      break;
    }
    case 0x5: /* FNMULS */
    {
      o.f = -(in.f * im.f);
      break;
    }
    case 0x6: /* FADDS */
    {
      o.f = in.f + im.f;
      break;
    }
    case 0x7: /* FSUBS */
    {
      o.f = in.f - im.f;
      break;
    }
    case 0x8: /* FDIVS */
    {
      o.f = in.f / im.f;
      break;
    }
    case 0xf:
    {
      /* Extension instructions */
      switch (Fn)
      {
        case 0x0: /* FCMPS */
        {
          o.u = im.u;
          break;
        }
        case 0x1: /* FCPYS */
        {
          o.f = fabs(im.f);
          break;
        }
        case 0x2: /* FABSS */
        {
          o.f = -im.f;
          break;
        }
        case 0x3: /* FSQRTS */
        {
          o.f = sqrtf(im.f);
          break;
        }
        case 0x8: /* FCMPS */
        {
          dp_fcmps(cpu, o.f, im.f, 0);
          break;
        }
        case 0x9: /* FCMPES */
        {
          dp_fcmps(cpu, o.f, im.f, 1);
          break;
        }
        case 0xa: /* FCMPZS */
        {
          dp_fcmps(cpu, o.f, 0.0f, 0);
          break;
        }
        case 0xb: /* FCMPEZS */
        {
          dp_fcmps(cpu, o.f, 0.0f, 1);
          break;
        }
        case 0x10: /* FUITOS */
        {
          o.f = (float)im.u;
          break;
        }
        case 0x11: /* FSITOS */
        {
          o.f = (float)im.i;
          break;
        }
        case 0x18: /* FTOUIS */
        {
          o.u = (uint32_t)roundf(im.f);
          break;
        }
        case 0x19: /* FTOUIZS */
        {
          o.u = (uint32_t)truncf(im.f);
          break;
        }
        case 0x1a: /* FTOSIS */
        {
          o.i = (int32_t)roundf(im.f);
          break;
        }
        case 0x1b: /* FTOSIZS */
        {
          o.i = (int32_t)truncf(im.f);
          break;
        }
        default:
        {
          cpu->emu->error( "Undefined VFP extension data proc instruction");
        }
      }
      break;
    }
    default:
    {
      throw EmulationException("Undefined VFP data proc instruction");
    }
  }

  /* Write output back to Fd */
  cpu->vfp.s[Fd] = o.u;
}

/**
 * Single Data Transfer
 *
 * @param vfp      VFP coprocessor state
 * @param Fd       Destination register
 * @param Rn       Base register
 * @param offfset  Offset from base
 * @param l        Load/store
 */
static inline void
dt_single_data_transfer(Cpu *cpu, uint32_t Fd, uint32_t Rn, uint32_t offset,
                         uint32_t l)
{
  uint32_t base = cpu_read_register(cpu, Rn) + (offset << 2);

  /* If l == 1 then load otherwise store */
  if (l)
  {
    cpu->vfp.s[Fd] = cpu->memory->readDwordLe(base);
  }
  else
  {
    cpu->memory->writeDwordLe(base, cpu->vfp.s[Fd]);
  }
}

/**
 * Multiple data transfer
 *
 * @param vfp      VFP coprocessor state
 * @param Fd       First float register
 * @param offset   Number of registers to load
 * @param l        Load/store
 * @param mode     0 - unindexed, 1 - increment, 2 - decrement
 */
static inline void
dt_multiple_data_transfer(Cpu *cpu, uint32_t Fd, uint32_t Rn, uint32_t offset,
                          uint32_t l, uint32_t mode)
{
  uint32_t i;
  uint32_t base;

  base = cpu_read_register(cpu, Rn) & 0xfffffffc;

  /* In mode 2 - decrement base and write back to Rn */
  if (mode == 2)
  {
    base -= offset << 2;
    cpu_write_register(cpu, Rn, base);
  }

  /* If l == 1 then load otherwise store */
  if (l)
  {
    /* Load registers from Fd to Fd + offset from memory */
    for (i = 0; i < offset; ++i)
    {
      cpu->vfp.s[Fd + i] =
        cpu->memory->readDwordLe(base + (i << 2));
    }
  }
  else
  {
    /* Write registers from Fd to Fd + offset to memory */
    for (i = 0; i < offset; ++i)
    {
      cpu->memory->writeDwordLe(base + (i << 2), cpu->vfp.s[Fd + i]);
    }
  }

  /* In mode 1 - increment base and write back to Rn */
  if (mode == 1)
  {
    cpu_write_register(cpu, Rn, base + (offset << 2));
  }
}

/**
 * Handles a data transfer instruction
 * @param vfp VFP context
 * @param instr Instruction
 */
void
vfp_data_transfer(Cpu *cpu, op_coproc_data_transfer_t* instr)
{
  uint32_t opcode, Fd, Rn;

  /* Extract Opcode - Layout: [p|u|w] */
  opcode = (instr->p << 2) | (instr->u << 1) | instr->w;

  /* Fd - Layout: [CRd|D] where D is stored in n */
  Fd = instr->CRd << 1 | instr->n;

  /* Rn */
  Rn = instr->CRn;

  /* Dispatch instruction */
  switch (opcode)
  {
    case 0x2:
    {
      /* FLDMS or FSTMS (unindexed) */
      dt_multiple_data_transfer(cpu, Fd, Rn, instr->offset, instr->l, 0);
      break;
    }
    case 0x3:
    {
      /* FLDMS or FSTMS (increment) */
      dt_multiple_data_transfer(cpu, Fd, Rn, instr->offset, instr->l, 1);
      break;
    }
    case 0x4:
    {
      /* FLDS or FSTS (negative offset) */
      dt_single_data_transfer(cpu, Fd, Rn, -instr->offset, instr->l);
      break;
    }
    case 0x5:
    {
      /* FLDMS or FSTMS (decrement) */
      dt_multiple_data_transfer(cpu, Fd, Rn, instr->offset, instr->l, 2);
      break;
    }
    case 0x6:
    {
      /* FLDS or FSTS (positive offset) */
      dt_single_data_transfer(cpu, Fd, Rn, instr->offset, instr->l);
      break;
    }
    default:
    {
      throw EmulationException("Unimplemented VFP data transfer instruction");
    }
  }
}

/**
 * Register Transfer sub-instructions
 */
static inline void
rt_reg_transfer(Cpu *cpu, uint32_t Fn, uint32_t Rd, uint32_t l)
{
  if (l)
  {
    cpu_write_register(cpu, Rd, cpu->vfp.s[Fn]);
  }
  else
  {
    cpu->vfp.s[Fn] = cpu_read_register(cpu, Rd);
  }
}

static inline void
rt_status_reg_transfer(Cpu *cpu, uint32_t Fn, uint32_t Rd, uint32_t l)
{
  uint32_t value = 0;
  if (l)
  {
    /* Read system register */
    switch (Fn)
    {
      case 0x0:
      {
        /* FPSID */
        value = cpu->vfp.fpsid;
        break;
      }
      case 0x2:
      {
        /* FPSCR */
        value = cpu->vfp.fpscr.r;
        break;
      }
      case 0x10:
      {
        /* FPEXC */
        value = cpu->vfp.fpexc.r;
        break;
      }
      default:
      {
        throw EmulationException("Unrecognised VFP system register");
      }
    }

    /* If Rd == 15 and Fn == 2 then this instruction is FMSTAT */
    if (Rd == 15)
    {
      if (Fn == 0x2)
      {
        cpu->cpsr.r &= 0x0fffffff;
        cpu->cpsr.r |= value & 0xf0000000;
      }
      else
      {
        throw EmulationException("Cannot copy to r15");
      }
    }
    else
    {
      /* Write to destination register */
      cpu_write_register(cpu, Rd, value);
    }
  }
  else
  {
    /* Read source register */
    value = cpu_read_register(cpu, Rd);

    /* Write to system register */
    switch (Fn)
    {
      case 0x0:
      {
        /* FPSID */
        cpu->vfp.fpsid = value;
        break;
      }
      case 0x2:
      {
        /* FPSCR */
        cpu->vfp.fpscr.r = value;
        break;
      }
      case 0x10:
      {
        /* FPEXC */
        cpu->vfp.fpexc.r = value;
        break;
      }
      default:
      {
        throw EmulationException("Unrecognised VFP system register");
      }
    }
  }
}

/**
 * Handles a register transfer instruction
 * @param vfp VFP context
 * @param instr Instruction
 */
void
vfp_reg_transfer(Cpu *cpu, op_coproc_reg_transfer_t* instr)
{
  uint32_t opcode, Rd, Fn;

  /* Opcode */
  opcode = instr->CP_opcode;

  /**
   * Extract registers
   */

  /* Rd */
  Rd = instr->Rd;

  /* Layout: CP = [N|0|0] */
  /* Fn - Layout: [CRn|N] */
  Fn = instr->CRn << 1;
  Fn |= (instr->CP >> 2) & 0x1;

  /* Dispatch instruction */
  switch (opcode)
  {
    case 0x0:
    {
      /* FMSR and FMRS */
      rt_reg_transfer(cpu, Fn, Rd, instr->l);
      break;
    }
    case 0x7:
    {
      /* FMXR, FMRX and FMSTAT */
      rt_status_reg_transfer(cpu, Fn, Rd, instr->l);
      break;
    }
    default:
    {
      throw EmulationException("Unimplemented VFP reg transfer instruction");
    }
  }
}

} /*namespace remu*/

