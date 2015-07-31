#include <os/Board.h>
#include <os/Types.h>

namespace os { namespace board {

namespace kvmvmm {

enum class HypercallType : uint16_t
{
	EXIT          = 0,
	PUTC          = 1,
	READ_ENV_BLOB = 2,
};

extern void hypercall_v_i(HypercallType call, int arg);

extern void hypercall_v_v(HypercallType call);

extern ssize_t hypercall_S(HypercallType call, const char *str, void *buffer, size_t size);

} /*namespace kvmvmm (in os::board)*/

using kvmvmm::HypercallType;

void putc(char c)
{
	hypercall_v_i(HypercallType::PUTC, c);
}

void shutdown()
{
	hypercall_v_v(HypercallType::EXIT);
}

uint64_t high_performance_timer()
{
	uint64_t a, d;
	asm volatile ( "rdtsc" : "=a"(a), "=d"(d) );
	return (d << 32) | a;
}

ssize_t read_env_blob(const char *argname, void *buffer, size_t size)
{
	return hypercall_S(HypercallType::READ_ENV_BLOB, argname, buffer, size);
}

}} /*namespace os::board*/

