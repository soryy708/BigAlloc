# BigAlloc

## Description
Allocate big things without caring about fragmentation.

Allows allocation of large chunks of memory even if no continuous chunk is available.

Strategy: Allocate smaller chunks, encapsulate the house keeping, expose a convenient interface.

## Usage
    #include <bigalloc.h>
    #include <iostream>
    
    int main()
    {
        const size_t size = 1024 * 1024 * 1024; // something large, you get the point
        
        // Allocation:
        BigAlloc<>::Handle big_block = BigAlloc<>::malloc(size);
        
        if(big_block != nullptr)
        {
            // Writing to memory:
            *big_block[128] = 32;
            
            // Reading from memory:
            std::cout << *big_block[128];
            
            // Deallocation:
            BigAlloc<>::free(big_block);
        }
        
        return 0;
    }

## License
The software is licensed under the MIT license. See file: [LICENSE](https://github.com/soryy708/BigAlloc/blob/master/LICENSE).

## How to contribute?
The contribution process is as usual with open source.
1. You fork this (or other, forked) repository
2. You make your changes
3. You open a pull request
4. Your pull request is reviewed
5. Optional: Changes are requested to the pull request
6. You make the neccesary changes (skip this step if 5 wasn't done)
7. Optional: Go to step 4
8. Your fork is merged

When doing your development and testing I recommend using [StackHeap](https://github.com/soryy708/StackHeap) to allocate memory on the stack instead of the heap, in a sequential and deterministic fashion.
This makes it trivial to keep track of and visualize the state of the memory.
See example: [test1.cpp](https://github.com/soryy708/BigAlloc/blob/master/test1.cpp).
