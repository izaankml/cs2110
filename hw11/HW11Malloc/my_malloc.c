/*
 * CS 2110 Fall 2018
 * Author: Izaan Kamal
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

/*Function Headers
 * Here is a place to put all of your function headers
 * Remember to declare them as static
 */
static metadata_t* mergeL(metadata_t* block);
static metadata_t* mergeR(metadata_t* block);
static metadata_t* split_block(metadata_t* block, size_t size);
static void add_to_address_list(metadata_t* add_block);
static void add_to_size_list(metadata_t* add_block);
static void remove_from_address_list(metadata_t* remove_block);
static void remove_from_size_list(metadata_t* remove_block);
static void set_canary(metadata_t* block);

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

/* MALLOC
 * See my_malloc.h for documentation
 */
void *my_malloc(size_t size) {
    if (size == 0) {
        my_malloc_errno = NO_ERROR;
        return NULL;
    }
    size_t actual_size = size + TOTAL_METADATA_SIZE;
    if (actual_size > SBRK_SIZE) {
        my_malloc_errno = SINGLE_REQUEST_TOO_LARGE;
        return NULL;
    }
    metadata_t* node = size_list;
    if (node == NULL) {
        metadata_t* created_node = my_sbrk(SBRK_SIZE);
        if (created_node == NULL) {
            my_malloc_errno = OUT_OF_MEMORY;
            return NULL;
        }
        created_node -> size = SBRK_SIZE;
        set_canary(created_node);
        add_to_size_list(created_node);
        add_to_address_list(created_node);
        node = created_node;
    }
    while (node != NULL) {
        if (node -> size == actual_size) {
            remove_from_size_list(node);
            remove_from_address_list(node);
            set_canary(node);
            my_malloc_errno = NO_ERROR;
            return (node + 1);
        } else if (node -> size >= actual_size + MIN_BLOCK_SIZE) {
            node = split_block(node, actual_size);
        } else if (node -> next_size == NULL) {
            metadata_t* created_node = my_sbrk(SBRK_SIZE);
            if (created_node == NULL) {
                my_malloc_errno = OUT_OF_MEMORY;
                return NULL;
            }
            created_node -> size = SBRK_SIZE;
            add_to_size_list(created_node);
            add_to_address_list(created_node);
            created_node = mergeL(created_node);
            created_node = mergeR(created_node);
            node = created_node;
        } else {
            node = node -> next_size;
        }
    }
	return NULL;
}

static metadata_t* mergeL(metadata_t* merge_block) {
    metadata_t* left = merge_block -> prev_addr;
    if (left != NULL) {
        uint8_t* end = ((uint8_t*) left) + left -> size;
        if (end == (uint8_t *) merge_block) {
            remove_from_address_list(merge_block);
            remove_from_size_list(merge_block);
            remove_from_size_list(left);
            left -> size += merge_block -> size;
            set_canary(left);
            add_to_size_list(left);
            return left;
        }
    }
    return merge_block;
}

static metadata_t* mergeR(metadata_t* merge_block) {
    metadata_t* right = merge_block -> next_addr;
    if (right != NULL) {
        uint8_t* end = ((uint8_t*) merge_block) + merge_block -> size;
        if (end == (uint8_t *) right) {
            remove_from_address_list(right);
            remove_from_size_list(right);
            remove_from_size_list(merge_block);
            merge_block -> size += right -> size;
            set_canary(merge_block);
            add_to_size_list(merge_block);
        }
    }
    return merge_block;
}

static metadata_t* split_block(metadata_t* block, size_t size) {
    remove_from_size_list(block);
    block -> size = block -> size - size;
    set_canary(block);
    add_to_size_list(block);
    metadata_t* new_block = (metadata_t*) ((uint8_t*) block + block -> size);
    new_block -> size = size;
    set_canary(new_block);
    add_to_size_list(new_block);
    add_to_address_list(new_block);
    return new_block;
}

static void add_to_address_list(metadata_t* add_block) {
    metadata_t* node = address_list;
    if (node == NULL) {
        address_list = add_block;
        address_list -> prev_addr = NULL;
        address_list -> next_addr = NULL;
    } else if ((uintptr_t) node > (uintptr_t) add_block) {
        add_block -> next_addr = address_list;
        address_list -> prev_addr = add_block;
        address_list = add_block;
    } else {
        while (node -> next_addr != NULL && (uintptr_t) add_block > (uintptr_t) node) {
            node = node -> next_addr;
        }
        if ((uintptr_t) node < (uintptr_t) add_block) {
            add_block -> prev_addr = node;
            add_block -> next_addr = NULL;
            node -> next_addr = add_block;
        } else {
            add_block -> next_addr = node;
            add_block -> prev_addr = node -> prev_addr;
            node -> prev_addr = add_block;
            add_block -> prev_addr -> next_addr = add_block;
        }
    }
}

static void add_to_size_list(metadata_t* add_block) {
    metadata_t* node = size_list;
    if (node == NULL) {
        size_list = add_block;
        size_list -> prev_size = NULL;
        size_list -> next_size = NULL;
    } else if (node -> size >= add_block -> size) {
        add_block -> next_size = size_list;
        size_list -> prev_size = add_block;
        size_list = add_block;
    } else {
        while (node -> next_size != NULL && add_block -> size >= node -> size) {
            node = node -> next_size;
        }
        if (node -> size < add_block -> size) {
            add_block -> prev_size = node;
            add_block -> next_size = NULL;
            node -> next_size = add_block;
        } else {
            add_block -> next_size = node;
            add_block -> prev_size = node -> prev_size;
            node -> prev_size = add_block;
            add_block -> prev_size -> next_size = add_block;
        }
    }
}

static void remove_from_address_list(metadata_t* remove_block) {
    if (remove_block == address_list) {
        address_list = remove_block -> next_addr;
        if (address_list != NULL) {
            address_list -> prev_addr = NULL;
        }
    } else {
        remove_block -> prev_addr -> next_addr = remove_block -> next_addr;
        if (remove_block -> next_addr != NULL) {
            remove_block -> next_addr -> prev_addr = remove_block -> prev_addr;
        }
    }
    remove_block -> prev_addr = NULL;
    remove_block -> next_addr = NULL;
}

static void remove_from_size_list(metadata_t* remove_block) {
    if (remove_block == size_list) {
        size_list = remove_block -> next_size;
        if (size_list != NULL) {
            size_list -> prev_size = NULL;
        }
    } else {
        remove_block -> prev_size -> next_size = remove_block -> next_size;
        if (remove_block -> next_size != NULL) {
            remove_block -> next_size -> prev_size = remove_block -> prev_size;
        }
    }
    remove_block -> prev_size = NULL;
    remove_block -> next_size = NULL;
}

static void set_canary(metadata_t* block) {
    block -> canary = ((uintptr_t) block ^ CANARY_MAGIC_NUMBER) + 1;
    unsigned long* end_canary = (unsigned long*) ((uint8_t*) (block) + block -> size - sizeof(unsigned long));
    *end_canary = ((uintptr_t) block ^ CANARY_MAGIC_NUMBER) + 1;

}

/* REALLOC
 * See my_malloc.h for documentation
 */
void *my_realloc(void *ptr, size_t size) {
	metadata_t* mem_pointer;
    if (ptr == NULL) {
        mem_pointer = my_malloc(size);
        return mem_pointer;
    }
    metadata_t* block = ((metadata_t*) ptr) - 1;
    unsigned long* end_canary = (unsigned long*) ((uint8_t*) (block) + block -> size - sizeof(unsigned long));
    if (!(size)) {
        my_free(ptr);
    } else if (*end_canary != block -> canary) {
        mem_pointer = (metadata_t*) ptr;
        my_malloc_errno = CANARY_CORRUPTED;
    } else {
        mem_pointer = my_malloc(size);
        size_t block_size = (size_t) (block -> size);
        if (size < block_size) {
            block_size = size;
        }
        memcpy(mem_pointer, ptr, block_size);
        my_free(ptr);
        return mem_pointer;
    }
    return NULL;
}

/* CALLOC
 * See my_malloc.h for documentation
 */
void *my_calloc(size_t nmemb, size_t size) {
	size_t actual_size = nmemb * size;
    void* calloc_pointer = my_malloc(actual_size);
    if (calloc_pointer) {
        memset(calloc_pointer, 0, actual_size);
    }
    return calloc_pointer;
}

/* FREE
 * See my_malloc.h for documentation
 */
void my_free(void *ptr) {
	if (ptr != NULL) {
        metadata_t* block = ((metadata_t*) ptr) - 1;
        unsigned long* end_canary = (unsigned long*) ((uint8_t*) (block) + block -> size - sizeof(unsigned long));
        if (*end_canary == block -> canary) {
            my_malloc_errno = NO_ERROR;
            add_to_address_list(block);
            add_to_size_list(block);
            block = mergeL(block);
            block = mergeR(block);
        } else {
            my_malloc_errno = CANARY_CORRUPTED;
        }
    } else {
        my_malloc_errno = NO_ERROR;
    }
}
