#ifndef CONTAINER_H
#define CONTAINER_H

#include <stdlib.h>
#include <assert.h>

#define ContainerType(type) struct { \
	type *values; \
	int numValues; \
	int capacity; \
}

#define containerForeach(type, itterator, container) for (type *itterator = container.values; itterator < container.values + container.numValues; ++itterator)


#define declareContainer(type, lowercaseType) \
typedef ContainerType(type) type##Container; \
type##Container make##type##Container(const int capacity); \
type * lowercaseType##ContainerAddValue(type##Container *container, const type value); \
void lowercaseType##ContainerAddValues(type##Container *container, const type##Container values); \
void lowercaseType##ContainerClear(type##Container *container); \
void lowercaseType##ContainerDestroy(type##Container *container); \


#define defineContainer(type, lowercaseType) \
type##Container make##type##Container(const int capacity) { \
 \
    return (type##Container) {malloc(sizeof(type) * capacity), 0, capacity}; \
} \
type * lowercaseType##ContainerAddValue(type##Container *container, const type value) { \
	if (container->capacity > container->numValues) { \
		container->values[container->numValues] = value; \
		++ container->numValues; \
		return &container->values[container->numValues - 1]; \
	} else { \
		/* TODO: Change this to grow the container dynamically. */ \
		assert(0); \
	} \
} \
void lowercaseType##ContainerAddValues(type##Container *container, const type##Container values) { \
	/* TODO: Actually implement. */ \
	assert(0); \
} \
void lowercaseType##ContainerClear(type##Container *container) { \
	container->numValues = 0; \
} \
void lowercaseType##ContainerDestroy(type##Container *container) { \
 \
	free(container->values); \
 \
	container->numValues = 0; \
	container->capacity = 0; \
} \


#endif // CONTAINER_H
