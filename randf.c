#include "randf.h"

float randf() {
	return ((float)rand()/(float)(RAND_MAX));
}