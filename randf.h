#ifndef _randf_
#define _randf_

#include <stdlib.h>

float randf() {
	return ((float)rand()/(float)(RAND_MAX));
}

#endif