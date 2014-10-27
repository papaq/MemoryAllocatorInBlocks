// MemoryAllocator.c : Defines the entry point for the console application.
//

#include <stdio.h>
#include <stdlib.h>
#include "Allocator.h"
#include "Output.h"


int main()
{
	global_mem = malloc(MEM_SIZE);
	
	char *line = "a = mem_alloc(4)\nb = mem_alloc(10)\nc = mem_alloc(16)";
	print_line(line);
	void * a = mem_alloc(4);
	void * b = mem_alloc(10);
	void * c = mem_alloc(16);

	mem_dump();

	line = "mem_free(b)";
	print_line(line);

	mem_free(b);
	mem_dump();

	line = "mem_realloc(c, 8)";
	print_line(line);

	void *new_c = mem_realloc(c, 8);
	mem_dump();

	line = "mem_realloc(a, 21)";
	print_line(line);

	void *new_a = mem_realloc(a, 21);
	mem_dump();

	free(global_mem);
    return 0;
}

