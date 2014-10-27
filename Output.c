#include "Output.h"

void new_line(int *current_pointer)
{
	if (0 == (current_pointer - global_mem) % 16)
	{
		printf("\n");
	}
}

void mem_dump()
{
	int * ptr = global_mem;
	while (ptr < global_mem + MEM_SIZE/4)
	{
		if (1 != get_head_byte(*ptr))
		{
			printf("%c%c%c%c", FREESPC, FREESPC, FREESPC, FREESPC);
			ptr++;

			new_line(ptr);

			continue;
		}

		int size = get_size_of_block(*ptr);
		int free = get_free_in_block(*ptr);

		printf("%c%c%c%c", HEADER, HEADER, HEADER, HEADER);
		ptr++;

		new_line(ptr);

		for (int i = 0; i < size - 1; i++)
		{
			printf("%c%c%c%c", BUSYBL, BUSYBL, BUSYBL, BUSYBL);
			ptr++;
			
			new_line(ptr);
		}

		for (int i = 0; i < 4 - free; i++)
		{
			printf("%c", BUSYBL);
		}

		for (int i = 0; i < free; i++)
		{
			printf("%c", EMPTYBL);
		}

		ptr++;

		new_line(ptr);
	}

	printf("\n");
}

void print_line(char * line)
{
	for (int i = 0; i < strlen(line)/sizeof(char); i++)
	{
		printf("%c", line[i]);
	}
	printf("\n\n");
}

