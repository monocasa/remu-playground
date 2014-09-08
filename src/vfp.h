#ifndef REMU_VFP_H
#define REMU_VFP_H

namespace remu {

class Vfp
{
public:
  Emulator* emu;

  /* Registers */
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
  } reg;
};

void vfp_init(Vfp*, Emulator*);
void vfp_destroy(Vfp *);
void vfp_dump(Vfp *);

void vfp_data_proc(Vfp*, op_coproc_data_proc_t* instr);
void vfp_data_transfer(Vfp*, op_coproc_data_transfer_t* instr);
void vfp_reg_transfer(Vfp*, op_coproc_reg_transfer_t* instr);

} /*namespace remu*/

#endif /* REMU_VFP_H */

