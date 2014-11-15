#ifndef REMU_VMM_OS_MEMORYMANAGER_H
#define REMU_VMM_OS_MEMORYMANAGER_H

namespace os { namespace mm {

void init();

void* allocate_page();

void set_lower_pml3(void *pml3, uint64_t virt_base);

}} /*namespace os::mm*/

#endif /*REMU_VMM_OS_MEMORYMANAGER_H*/

