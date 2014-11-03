#include "os/Board.h"
#include "os/Types.h"

namespace os { namespace board {

namespace kvmvmm {

enum class HypercallType : uint16_t
{
	EXIT = 0,
	PUTC = 1,
};

void hypercall_i(HypercallType call, int arg)
{
	asm volatile ( "outl %0, %1" : : "a"(arg), "Nd"(call) );
}

} /*namespace kvmvmm (in os::board)*/

using kvmvmm::HypercallType;
using kvmvmm::hypercall_i;

void putc(char c)
{
	hypercall_i(HypercallType::PUTC, c);
}

}} /*namespace os::board*/

