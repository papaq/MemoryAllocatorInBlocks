#include "Allocator.h"

shift_left(int *what, int n)
{
	*what = *what << n;
}

int pow(int what, int topow)
{
	if (topow == 1)
	{
		return what;
	}

	return pow(what, topow - 1) *what;
}

int take_n_bits(int of_what, int start_bit, int n_bit)
{
	return of_what >> (sizeof(int)*8 - n_bit - start_bit) & (pow(2, n_bit+1)-1);
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
	return num >> 15 & 255;
}

int get_size_of_block(int num)
{
	return num >> 7 & 255;
}

int get_free_in_block(int num)
{
	return num >> 4 & 7;
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
			if (get_head_byte(*current_start_point) == 1)
			{
				current_start_point = j + get_size_of_block(*current_start_point);
				break;
			}

			curr_size += 4;

			if (curr_size >= size)
			{
				return current_start_point;
			}

			j++;
		}
	} while (current_start_point < global_mem);

	return NULL;
}

void *mem_alloc(size_t size)
{
	// Looking for size + 1 because of header 1 *sizeof(int) bytes
	int *ptr_to_free = find_free_space(size + 1);

	if (!ptr_to_free)
	{
		return NULL;
	}

	int header = make_header(true, ptr_to_free+1, size);
	*ptr_to_free = header;

	return NULL;
}

void *mem_realloc(void *addr, size_t size)
{
	return 6;
}

void mem_free(void *addr)
{
	if (addr < global_mem || addr > global_mem + MEM_SIZE - 4)
	{
		return;
	}

	int *header = (int *)addr - 1;

	*header = 0;
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
	shift_left(&header, 8);
	header += ((int)address - (int)global_mem) / 4;

	// Add size/4
	shift_left(&header, 8);
	int free = 4 - size % 4;
	header += size / 4 + (free == 0 ? 0 : 1);

	// Add free space in this block
	shift_left(&header, 3);
	header += free;

	// Final shift
	shift_left(&header, 4);

	return header;
}

void test()
{
	//printf("%d", pow(2, 5));
	//printf("%d\n", take_n_bits(2863309845, 20, 7));
	//printf("%d\n", get_first_byte(78227881));
	//printf("%d\n", get_state_of_block(2854922922));


	int header = make_header(true, global_mem + 48, 50);

	printf("%d\n\n", header);

	printf("head = %d\n", get_head_byte(header));
	printf("flag = %d\n", get_state_of_block(header));
	printf("addr = %d\n", get_address_of_block(header));
	printf("size = %d\n", get_size_of_block(header));
	printf("free = %d\n", get_free_in_block(header));

	
}

