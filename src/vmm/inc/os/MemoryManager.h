#ifndef REMU_VMM_OS_MEMORYMANAGER_H
#define REMU_VMM_OS_MEMORYMANAGER_H

#include <cstdint>

namespace os { namespace intm {

struct Context;

}} /*namespace os::intm*/

namespace os { namespace mm {

void init();

void* allocate_page();

void set_lower_pml3(void *pml3, uint64_t virt_base);

void on_page_fault( os::intm::Context *ctx );

void *sbrk(intptr_t);

uint64_t phys_for_virt( const void *virt );

}} /*namespace os::mm*/

#endif /*REMU_VMM_OS_MEMORYMANAGER_H*/

