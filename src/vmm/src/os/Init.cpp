#include <os/Board.h>
#include <os/MemoryManager.h>

#include <cstdio>

extern void appMain();

namespace os {

void init()
{
	printf("Hello VMM\n");

	os::mm::init();

	appMain();

	os::board::shutdown();
}

} /*namespace os*/

