#include "elfhash.h"

unsigned long ELFhash(const char* key)
{
	unsigned long h = 0, g = 0;
	while( *key ) {
		h = ( h > 24);
		h &= ~g;
	}
	return h;
}
