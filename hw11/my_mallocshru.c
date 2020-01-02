/*
 * CS 2110 Fall 2018
 * Author:
 */

/* we need this for uintptr_t */
#include <stdint.h>
/* we need this for memcpy/memset */
#include <string.h>
/* we need this to print out stuff*/
#include <stdio.h>
/* we need this for the metadata_t struct and my_malloc_err enum definitions */
#include "my_malloc.h"
/* include this for any boolean methods */
#include <stdbool.h>


#define POINTER_TO_MEMORY(block) (block + 1)
#define POINTER_TO_BLOCK(pointer) (((metadata_t *) pointer) - 1)
#define END_OF_BLOCK(block) (((uint8_t *) block) + block -> size)
#define TAIL_CANARY(block) *((unsigned long *) (END_OF_BLOCK(block) - sizeof(unsigned long)))

/*Function Headers
 * Here is a place to put all of your function headers
 * Remember to declare them as static
 */

static void setCanaries(metadata_t *block);
static void addressRemove(metadata_t *removed);
static void sizeRemove(metadata_t *removed);
static void addressAdd(metadata_t *added);
static void sizeAdd(metadata_t *added);
static metadata_t *checkMergeLeft(metadata_t *block);
static metadata_t *checkMergeRight(metadata_t *block);
static metadata_t *checkMergeBlock(metadata_t *block);
static metadata_t *split(metadata_t *block, size_t size);

/* Our freelist structure - our freelist is represented as two doubly linked lists
 * the address_list orders the free blocks in ascending address
 * the size_list orders the free blocks by size
 */

metadata_t *address_list;
metadata_t *size_list;

/* Set on every invocation of my_malloc()/my_free()/my_realloc()/
 * my_calloc() to indicate success or the type of failure. See
 * the definition of the my_malloc_err enum in my_malloc.h for details.
 * Similar to errno(3).
 */
enum my_malloc_err my_malloc_errno;

static void setCanaries(metadata_t *block) {
	unsigned long canary =  ((uintptr_t)block ^ CANARY_MAGIC_NUMBER) + 1;
	block -> canary = canary;
	TAIL_CANARY(block) = canary;
}

static void addressRemove(metadata_t *removed) {
	if (address_list == removed) {
		address_list = removed -> next_addr;
		if (address_list != NULL) {
			address_list -> prev_addr = NULL;
		}
	} else {
		removed -> prev_addr -> next_addr = removed -> next_addr;
		if (removed -> next_addr != NULL) {
			removed -> next_addr -> prev_addr = removed -> prev_addr;
		}
	}
	removed -> prev_addr = NULL;
	removed -> next_addr = NULL;
}

static void sizeRemove(metadata_t *removed) {
	if (size_list == removed) {
		size_list = removed -> next_size;
		if (size_list != NULL) {
			size_list -> prev_size = NULL;
		}
	} else {
		removed -> prev_size -> next_size = removed -> next_size;
		if (removed -> next_size != NULL) {
			removed -> next_size -> prev_size = removed -> prev_size;
		}
	}
	removed -> prev_size = NULL;
	removed -> next_size = NULL;
}

static void addressAdd(metadata_t *added) {
	metadata_t *current = address_list;
	if (current == NULL) {
		address_list = added;
		address_list -> prev_addr = NULL;
		address_list -> next_addr = NULL;
		return;
	} else if (current > added) {
		added -> next_addr = address_list;
		address_list -> prev_addr = added;
		address_list = added;
		return;
	}
	while ((current < added) && (current -> next_addr) != NULL) {
		current = current -> next_addr;
	}

	if (current < added) {
		added -> prev_addr = current;
		added -> next_addr = NULL;
		current -> next_addr = added;
	} else {
		added -> prev_addr = current -> prev_addr;
		added -> next_addr = current;
		added -> prev_addr -> next_addr = added;
		current -> prev_addr = added;
	}
}

static void sizeAdd(metadata_t *added) {
	metadata_t *current = size_list;
	if (current == NULL) {
		size_list = added;
		size_list -> prev_size = NULL;
		size_list -> next_size = NULL;
		return;
	} else if (current -> size >= added -> size) {
		added -> next_size = size_list;
		size_list -> prev_size = added;
		size_list = added;
		return;
	}
	while ((current -> size < added -> size) && (current -> next_size) != NULL) {
		current = current -> next_size;
	}

	if (current -> size < added -> size) {
		added -> prev_size = current;
		added -> next_size = NULL;
		current -> next_size = added;
	} else {
		added -> prev_size = current -> prev_size;
		added -> next_size = current;
		added -> prev_size -> next_size = added;
		current -> prev_size = added;
	}
}

static metadata_t *checkMergeLeft(metadata_t *block) {
	metadata_t *left = block -> prev_addr;
	if (left == NULL) {
		return block;
	}
	if (END_OF_BLOCK(left) == (uint8_t *) block) {
		addressRemove(block);
		sizeRemove(block);
		sizeRemove(left);
		left -> size = left -> size + block -> size;
		setCanaries(left);
		sizeAdd(left);
		return left;
	}
	return block;
}

static metadata_t *checkMergeRight(metadata_t *block) {
	metadata_t *right = block -> next_addr;
	if (right == NULL) {
		return block;
	}
	if (END_OF_BLOCK(block) == (uint8_t *) right) {
		addressRemove(right);
		sizeRemove(right);
		sizeRemove(block);
		block -> size = block -> size + right -> size;
		setCanaries(block);
		sizeAdd(block);
	}
	return block;
}

static metadata_t *checkMergeBlock(metadata_t *block) {
	addressAdd(block);
	sizeAdd(block);
	block = checkMergeLeft(block);
	block = checkMergeRight(block);
	return block;
}

static metadata_t *split(metadata_t *block, size_t size) {
	sizeRemove(block);
	block -> size = block -> size - size;
	setCanaries(block);
	sizeAdd(block);
	metadata_t *newBlock = (metadata_t *) END_OF_BLOCK(block);
	newBlock -> size = size;
	setCanaries(newBlock);
	addressAdd(newBlock);
	sizeAdd(newBlock);
	return newBlock;
}

/* MALLOC
 * See my_malloc.h for documentation
 */
void *my_malloc(size_t size) {
    size_t requiredMemory = size + TOTAL_METADATA_SIZE;
    if (requiredMemory > SBRK_SIZE) {
    	my_malloc_errno = SINGLE_REQUEST_TOO_LARGE;
    	return NULL;
    } else if (size == 0) {
    	my_malloc_errno = NO_ERROR;
    	return NULL;
    }

    metadata_t *block = size_list;
    if (block == NULL) {
    	metadata_t *newBlock = my_sbrk(SBRK_SIZE);
    		if (newBlock == NULL) {
    			my_malloc_errno = OUT_OF_MEMORY;
    			return NULL;
    		}
    		newBlock -> size = SBRK_SIZE;
    		setCanaries(newBlock);
    		addressAdd(newBlock);
    		sizeAdd(newBlock);
    		block = newBlock;
    }
    while (block != NULL) {
    	if (block -> size == requiredMemory) {
    		addressRemove(block);
    		sizeRemove(block);
    		setCanaries(block);
    		my_malloc_errno = NO_ERROR;
    		return POINTER_TO_MEMORY(block);
    	} else if (block -> size >= requiredMemory + MIN_BLOCK_SIZE) {
    		block = split(block, requiredMemory);
    		continue;
    	} else if (block -> next_size == NULL) {
    		metadata_t *newBlock = my_sbrk(SBRK_SIZE);
    		if (newBlock == NULL) {
    			my_malloc_errno = OUT_OF_MEMORY;
    			return NULL;
    		}
    		newBlock -> size = SBRK_SIZE;
    		block = checkMergeBlock(newBlock);
    		continue;
    	}
    	block = block -> next_size;
    }
    return NULL;
}

/* REALLOC
 * See my_malloc.h for documentation
 */
void *my_realloc(void *ptr, size_t size) {
	metadata_t *memoryPointer;
	my_malloc_errno = NO_ERROR;
	if (ptr == NULL) {
		memoryPointer = my_malloc(size);
	} else if (size == 0) {
		my_free(ptr);
		return NULL;
	} else if (TAIL_CANARY(POINTER_TO_BLOCK(ptr)) != POINTER_TO_BLOCK(ptr) -> canary) {
		memoryPointer = ptr;
		my_malloc_errno = CANARY_CORRUPTED;
		return NULL;
	} else {
		memoryPointer = my_malloc(size);
		size_t toCopy = (size_t) (POINTER_TO_BLOCK(ptr) -> size);
		if (size < toCopy) {
			toCopy = size;
		}
		memcpy(memoryPointer, ptr, toCopy);
		my_free(ptr);
	}
	return memoryPointer;
}

/* CALLOC
 * See my_malloc.h for documentation
 */
void *my_calloc(size_t nmemb, size_t size) {
	size_t totalSize = nmemb * size;
	metadata_t *memoryPointer = my_malloc(totalSize);
	if (memoryPointer == NULL) {
		return NULL;
	}
	memset(memoryPointer, 0, totalSize);
	return memoryPointer;
}

/* FREE
 * See my_malloc.h for documentation
 */
void my_free(void *ptr) {
	if (ptr == NULL) {
		my_malloc_errno = NO_ERROR;
		return;
	}
	metadata_t *block = POINTER_TO_BLOCK(ptr);
	if (TAIL_CANARY(block) != block -> canary) {
		my_malloc_errno = CANARY_CORRUPTED;
		return;
	} else {
		my_malloc_errno = NO_ERROR;
		checkMergeBlock(block);
	}
}
