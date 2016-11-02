#include "Allocator.h"

shift_left(int *what, int n)
{
	*what = *what << n;
}

int get_head_byte(int num)
{
	return num >> 24;
}

int get_state_of_block(int num)
{
	return num >> 23 & 1;
}

int get_address_of_block(int num)
{
	return num >> 13 & 1023;
}

int get_size_of_block(int num)
{
	return num >> 3 & 1023;
}

int get_free_in_block(int num)
{
	return num & 3;
}

int *find_free_space(int size)
{
	int *current_start_point = global_mem;

	do
	{
		if (size > global_mem + MEM_SIZE - current_start_point)
		{
			return NULL;
		}
		
		int curr_size = 0;

		int *j = current_start_point;
		while (j < MEM_SIZE + global_mem)
		{
			if (get_head_byte(*(current_start_point + curr_size / 4)) == 1)
			{
				// + 1, because we suppose the block to start after the header 4 bytes
				current_start_point = j + 
					get_size_of_block(*(current_start_point + curr_size / 4)) + 1;
				break;
			}

			curr_size += 4;

			if (curr_size >= size)
			{
				return current_start_point;
			}

			j++;
		}
	} while (current_start_point < global_mem + MEM_SIZE);

	return NULL;
}

void *mem_alloc(size_t size)
{
	// Looking for size + 1 *sizeof(int) because of header
	int *ptr_to_free = find_free_space(size + 1 * sizeof(int));

	if (!ptr_to_free)
	{
		return NULL;
	}

	int header = make_header(true, ptr_to_free+1, size);
	*ptr_to_free = header;

	return ptr_to_free + 1;
}

void *mem_realloc(void *addr, size_t size)
{
	if (addr < global_mem + 1 || addr > global_mem + MEM_SIZE - 4)
	{
		return;
	}

	int header = *((int*)addr - 1);
	int size_of_block = get_size_of_block(header)*4;
	int size_of_busy_block = size_of_block - get_free_in_block(header);

	if (size == size_of_busy_block)
	{
		return addr;
	}

	if (size <= size_of_block)
	{
		int new_header = make_header(true, (int*)addr, size);
		int *head_ptr = (int*)addr - 1;
		*head_ptr = new_header;
		return addr;
	}

	void *new_block = mem_alloc(size);
	if (new_block == NULL)
	{
		return NULL;
	}
	mem_free(addr);

	return new_block;
}

void mem_free(void *addr)
{
	if (addr < global_mem || addr > global_mem + MEM_SIZE - 4)
	{
		return;
	}

	int *head_ptr = (int *)addr - 1;

	*head_ptr = 0;
}

int make_header(bool busy, int *address, int size)
{
	if (size < 1
		|| size > MEM_SIZE - 1
		|| 4 > (int)address - (int)global_mem
		|| MEM_SIZE < (int)address - (int)global_mem + 4
		|| 0 != ((int)address - (int)global_mem) % 4)
	{
		return -1;
	}

	// 1 in the first byte of header points to its origin
	int header = 1;

	// Add flag busy/free
	shift_left(&header, 1);
	header += busy;

	// Add local address/4
	shift_left(&header, 10);
	header += ((int)address - (int)global_mem) / 4;

	// Add size/4
	shift_left(&header, 10);
	int free = 0 == size % 4 ? 0 : 4 - size % 4;
	header += size / 4 + (free == 0 ? 0 : 1);

	// Add free space in this block
	shift_left(&header, 3);
	header += free;

	return header;
}
