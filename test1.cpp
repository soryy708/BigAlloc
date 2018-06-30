#define BIGALLOC_NO_CSTDLIB // We intend to use custom `malloc` and `free`
#include "bigalloc.h"
#include "StackHeap/stackheap.h" // a git submodule, check it out: https://github.com/soryy708/StackHeap
#include <iostream>

char StackHeap<64>::memory[64];
char StackHeap<64>::block_directory[64];
char StackHeap<64>::unused_block_id;

void dump(const char* beginning);
char* fragmentTheMemory();

int main()
{
	StackHeap<64>::init();
	char* memory_beginning = fragmentTheMemory();

	auto big_block = BigAlloc<StackHeap<64>::malloc, StackHeap<64>::free>::malloc(32);
	for(unsigned int i = 0; i < 32; ++i)
	{
		*big_block[i] = 97;
	}

	dump(memory_beginning);
	// Output should be 16 times A
	//             then 16 times a - first chunk of BigAlloc
	//             then 16 times C
	//             then 16 times a - second chunk of BigAlloc

	BigAlloc<StackHeap<64>::malloc, StackHeap<64>::free>::free(big_block);

	// Just making sure that freeing was successful:
	char* c1 = (char*) StackHeap<64>::malloc(16);
	char* c2 = (char*) StackHeap<64>::malloc(16);
	if(c1 != nullptr && c2 != nullptr)
	{
		std::cout << "BigAlloc deallocation was successful!" << std::endl;
	}
	else
	{
		std::cout << "BigAlloc failed to deallocate its resources :(" << std::endl;
	}

	return 0;
}

char* fragmentTheMemory()
{
	char* c1 = (char*) StackHeap<64>::malloc(16);
	char* c2 = (char*) StackHeap<64>::malloc(16);
	char* c3 = (char*) StackHeap<64>::malloc(16);
	char* c4 = (char*) StackHeap<64>::malloc(16);

	// Lets fill the different blocks with something to make them easy to identify
	for(unsigned int i = 0; i < 16; ++i)
	{
		c1[i] = 65;
		c2[i] = 66;
		c3[i] = 67;
		c4[i] = 68;
	}

	StackHeap<64>::free(c2);
	StackHeap<64>::free(c4);
	// intentionally leaking c1 and c3

	return c1; // returning the beginning
}

void dump(const char* beginning)
{
	std::cout << "Address\t\tValue" << std::endl << std::endl;

	for (unsigned int i = 0; i < 64; i++)
	{
		const unsigned int* val_as_int = (unsigned int*) *(int*)(beginning + i);

		// Print the address
		std::cout << (int*)(beginning + i);

		std::cout << " - ";

		// Print the value as character
		std::cout << (char) *(beginning + i);

		std::cout << "\t";
		if(!((i+1) % 8))
			std::cout << std::endl;
	}
	std::cout << std::endl;
}
