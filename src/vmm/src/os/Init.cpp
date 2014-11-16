#include <os/Board.h>
#include <os/Log.h>
#include <os/MemoryManager.h>

extern void appMain();

namespace os {

void init()
{
	log("Hello VMM\n");

	os::mm::init();

	appMain();

	os::board::shutdown();
}

} /*namespace os*/

