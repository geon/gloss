#ifndef ALLOCATOR_H
#define ALLOCATOR_H


#define declareAllocator(type) \
type * allocate##type(type data); \

#define defineAllocator(type) \
type * allocate##type(type data) { \
	type *object = malloc(sizeof(type)); \
	*object = data; \
	return object; \
} \


#endif // ALLOCATOR_H