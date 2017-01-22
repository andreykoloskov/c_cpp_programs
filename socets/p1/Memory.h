#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

#include <iostream>
#include <cstdlib>
#include <vector>
#include "Vpointer.h"
#include "Cpointer.h"

//Memory allocator
class Memory
{
public:
	int size;
	void *arr;
	void *top;
	std::vector<Vpointer> vvp;

	Memory(int sz): size(sz), arr(0), vvp(0)
	{
		arr = malloc(size);
		top = arr;
	}

	~Memory()
	{
		if (arr)
			free(arr);
		arr = NULL;
		top = NULL;
		vvp.clear();
	}

	template<class T> Cpointer<T> * myalloc(int sz);

	template<class T> void myfree(Cpointer<T> * ptr);

	void compress();

	friend std::ostream &operator << (std::ostream &stream, Memory & mm);
};

#endif
