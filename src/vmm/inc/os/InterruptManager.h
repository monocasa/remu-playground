#ifndef REMU_VMM_OS_INTERRUPTMANAGER_H
#define REMU_VMM_OS_INTERRUPTMANAGER_H

#include <cstdint>

namespace os { namespace intm {

struct Context
{
	uint64_t r15, r14, r13, r12;
	uint64_t r11, r10,  r9,  r8;
	uint64_t rbp, rdi, rsi;
	uint64_t rdx, rcx, rbx, rax;
	uint64_t intNum;
	uint32_t errorCode;
	uint32_t rsvd;
	uint64_t rip;
	uint8_t  cs;
	uint8_t  cs_rsvd[7];
	uint64_t rflags;
	uint64_t rsp;
	uint8_t  ss;
	uint8_t  ss_rsvd[7];

	void print();
};

void init();

}} /*namespace os::intm*/

#endif /*REMU_VMM_OS_INTERRUPTMANAGER_H*/

