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
