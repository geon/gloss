#include "randf.h"
#include <stdlib.h>

float randf() {
	return ((float)rand()/(float)(RAND_MAX));
}