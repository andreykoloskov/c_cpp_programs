#ifndef VPOINTER_H_INCLUDED
#define VPOINTER_H_INCLUDED

#include <iostream>

//Internal pointer in array
class Vpointer
{
public:
	int size;
	void *mem;
	void *cp;

	Vpointer(int sz = 0, void *mm = NULL, void *c = NULL):
		size(sz), mem(mm), cp(c) {}

	~Vpointer() {}
};

#endif
