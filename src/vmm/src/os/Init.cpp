#include "os/Board.h"
#include "os/Log.h"

extern void appMain();

namespace os {

void init()
{
	log("Hello VMM\n");

	appMain();
}

} /*namespace os*/

