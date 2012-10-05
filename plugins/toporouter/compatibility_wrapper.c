#include "toporouter.h"

void Message(char *c,...) {
        va_list ap;
	va_start(ap, c); /* Initialize the va_list */
	printf(c, ap); /* Call printf */
	va_end(ap); /* Cleanup the va_list */
}
