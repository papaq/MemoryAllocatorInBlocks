// MemoryAllocator.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <stdlib.h>
#include "Allocator.h"
#include "Output.h"


int main()
{
	global_mem = malloc(MEM_SIZE);

	/*
	for (int i = 1; i <= MEM_SIZE / 4; i ++)
	{
		mem_free(global_mem + i);
	}
	*/

	mem_alloc(23);

	mem_dump();


	free(global_mem);
    return 0;
}

