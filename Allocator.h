#pragma once

#define MEM_SIZE 1024

#include <stdio.h>

typedef int size_t ;
typedef enum { false, true } bool;


int * global_mem;

void *mem_alloc(size_t size);

void *mem_realloc(void *addr, size_t size);

void mem_free(void *addr);

void test();

int get_head_byte(int num);

int get_state_of_block(int num);

int get_address_of_block(int num);

int get_size_of_block(int num);

int get_free_in_block(int num);
