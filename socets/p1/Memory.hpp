#ifndef MEMORY_HPP_INCLUDED
#define MEMORY_HPP_INCLUDED

#include <cstring>
#include "Vpointer.h"
#include "Cpointer.h"
#include "Memory.h"

template<class T> Cpointer<T> * Memory::myalloc(int sz)
{
	Cpointer<T> *cp = new Cpointer<T>;
	Vpointer vp(sz, top, cp);
	top = (void *) ((unsigned char *) top + sz);
	vvp.push_back(vp);
	cp->mem = this;
	cp->num = vvp.size() - 1;
	return cp;
}

template<class T> void Memory::myfree(Cpointer<T> * ptr)
{
	std::vector<Vpointer>::iterator p = vvp.begin();
	p += ptr->num;
	p->size = 0;
	p->mem = NULL;
	p->cp = NULL;
	ptr->mem = NULL;
	ptr->num = 0;
	delete ptr;
}

std::ostream &operator << (std::ostream &stream, Memory & mm)
{
	stream << "begin busy addres: " << mm.arr << ", ";
	stream << "begin free address: " << mm.top << "\n";
	stream << "memory elements:\n";

	std::vector<Vpointer>::iterator p;
	for (p = mm.vvp.begin(); p != mm.vvp.end(); p++) {
		if (p->mem) {
			stream << "addres: " << p->mem << ", ";
			stream << "size: " << p->size << ", ";
			stream << "int value: ";
			stream << *((int *)(p->mem));
			stream << "\n";
		} else {
			stream << "free element" << "\n";
		}
	}
stream << "\n";
}

void Memory::compress()
{
	top = arr;
	int i = 0;
	std::vector<Vpointer>::iterator p = vvp.begin();
	std::vector<Vpointer>::iterator q = p;
	for (; p != vvp.end(); p++) {
		if (!(p->mem))
			continue;
		*q = *p;
		((Cpointer<char> *) (q->cp))->num = q - vvp.begin();
		q++;
	}
	vvp.erase(q, vvp.end());

	for (p = vvp.begin(); p != vvp.end(); p++) {
		memcpy(top, p->mem, p->size);
		p->mem = top;
		top = (void *) ((unsigned char *) top + p->size);
	}
}

#endif
