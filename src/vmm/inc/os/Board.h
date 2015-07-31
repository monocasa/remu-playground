#ifndef REMU_VMM_OS_BOARD_H
#define REMU_VMM_OS_BOARD_H

#include <os/Types.h>

namespace os { namespace board {

void putc(char c);

void shutdown();

uint64_t high_performance_timer();

ssize_t read_env_blob(const char *argname, void *buffer, size_t size);

}} /*namespace os::board*/

#endif /*REMU_VMM_OS_BOARD_H*/

