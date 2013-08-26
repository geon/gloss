#ifndef CONTAINER_H
#define CONTAINER_H

#include <stdlib.h>
#include <assert.h>
#include <string.h>


#define containerForeach(valueType, iterator, container) for (valueType *iterator = container.values; iterator < container.values + container.numValues; ++iterator)


#define declareContainerInternal(containerType, valueType, constructorTypeName, methodPrefix) \
typedef struct { \
	valueType *values; \
	int numValues; \
	int	capacity; \
} containerType; \
containerType make##constructorTypeName(const int capacity); \
valueType * methodPrefix##AddValue(containerType *container, const valueType value); \
void methodPrefix##AddValues(containerType *container, const containerType values); \
void methodPrefix##Clear(containerType *container); \
void methodPrefix##Destroy(containerType *container); \


#define defineContainerInternal(containerType, valueType, constructorTypeName, methodPrefix) \
containerType make##constructorTypeName(const int capacity) { \
 \
	return (containerType) {malloc(sizeof(valueType) * capacity), 0, capacity}; \
} \
valueType * methodPrefix##AddValue(containerType *container, const valueType value) { \
	if (container->capacity > container->numValues) { \
		container->values[container->numValues] = value; \
		++ container->numValues; \
		return &container->values[container->numValues - 1]; \
	} else { \
		/* TODO: Change this to grow the container dynamically. */ \
		assert(0); \
	} \
} \
void methodPrefix##AddValues(containerType *container, const containerType values) { \
	if (container->capacity >= container->numValues + values.numValues) { \
		memcpy(container->values, values.values, sizeof(valueType) * values.numValues); \
		container->numValues += values.numValues; \
	} else { \
		/* TODO: Change this to grow the container dynamically. */ \
		assert(0); \
	} \
} \
void methodPrefix##Clear(containerType *container) { \
	container->numValues = 0; \
} \
void methodPrefix##Destroy(containerType *container) { \
 \
	free(container->values); \
 \
	container->numValues = 0; \
	container->capacity = 0; \
} \


#define declareContainer(type, lowerCaseType) \
declareContainerInternal(type##Container, type, type##Container, lowerCaseType##Container) \


#define defineContainer(type, lowerCaseType) \
defineContainerInternal(type##Container, type, type##Container, lowerCaseType##Container) \


#define declarePointerContainer(type, lowerCaseType) \
declareContainerInternal(type##PointerContainer, type *, type##PointerContainer, lowerCaseType##PointerContainer) \


#define definePointerContainer(type, lowerCaseType) \
defineContainerInternal(type##PointerContainer, type *, type##PointerContainer, lowerCaseType##PointerContainer) \


#endif // CONTAINER_H
