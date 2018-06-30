/* BigAlloc
*   Allows allocation of large chunks of memory even if no continuous chunk is available.
*   Strategy: Allocate smaller chunks, encapsulate the house keeping, expose a convenient interface.
*
* License: MIT
* Copyright (c) 2018 Ivan Rubinson
*
*  Permission is hereby granted, free of charge, to any person obtaining a copy
*  of this software and associated documentation files (the "Software"), to deal
*  in the Software without restriction, including without limitation the rights
*  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*  copies of the Software, and to permit persons to whom the Software is
*  furnished to do so, subject to the following conditions:
*
*  The above copyright notice and this permission notice shall be included in all
*  copies or substantial portions of the Software.
*
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*  SOFTWARE.
*/
#ifndef BIGALLOC_H_INCLUDED
#define BIGALLOC_H_INCLUDED

#include <tuple>
#include <vector>

#ifndef BIGALLOC_NO_CSTDLIB
	#include <cstdlib> // for malloc and free
	template<void* allocator_function(size_t size) = malloc, void deallocator_function(void* block) = free>
#else
	template<void* allocator_function(size_t size), void deallocator_function(void* block)>
#endif
class BigAlloc
{
public:
	class Handle
	{
	public:
		friend BigAlloc;

	private:
		std::vector<std::tuple<char*, const size_t>> blocks;

		void allocate(const size_t length)
		{
			if(length > 0) {
				char* block = (char*) allocator_function(length);

				if(block == nullptr)
				{
					allocate(length / 2);
					allocate(length / 2);
				}
				else
				{
					blocks.push_back(std::tuple<char*, const size_t>(block, length));
				}
			}
		}

		void destroy()
		{
			for (unsigned int i = 0; i < blocks.size(); ++i)
			{
				if (std::get<0>(blocks[i]) != nullptr)
				{
					deallocator_function(std::get<0>(blocks[i]));
					std::get<0>(blocks[i]) = nullptr;
				}
			}
		}

	public:
		char* operator[](const size_t position) // returns nullptr if out of bounds
		{
			size_t sum = 0;
			unsigned int i;
			for(i = 0; i < blocks.size(); ++i)
			{
				if(position < sum + std::get<1>(blocks[i]))
				{
					break;
				}
				sum += std::get<1>(blocks[i]);
			}

			if(i < blocks.size())
			{
				return &(std::get<0>(blocks[i])[position - sum]);
			}

			return nullptr;
		}

		Handle(const size_t length)
		{
			allocate(length);
		}

		Handle() = delete;
	};

	static Handle malloc(size_t length)
	{
		return Handle(length);
	}

	static void free(Handle& handle)
	{
		handle.destroy();
	}

	BigAlloc::BigAlloc() = delete;
	BigAlloc::~BigAlloc() = delete;
};

#endif
