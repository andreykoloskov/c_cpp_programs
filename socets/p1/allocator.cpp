#include <iostream>
#include <cstdlib>
#include <vector>
#include "Vpointer.h"
#include "Cpointer.h"
#include "Cpointer.hpp"
#include "Memory.h"
#include "Memory.hpp"

int main(int argc, char **argv)
{
	Memory mem(16 * 1024);
	std::cout << mem;

	Cpointer<int> *a = mem.myalloc<int>(sizeof(int));
	Cpointer<long long> *b = mem.myalloc<long long>(sizeof(long long));
	*a = 4;
	*b = 10;
	std::cout << "a = " <<  *a << std::endl;
	std::cout << "b = " << *b << std::endl;
	std::cout << mem;

	mem.myfree(a);
	std::cout << mem;
	mem.myfree(b);
	std::cout << mem;

	Cpointer<char> *c = mem.myalloc<char>(sizeof(char));
	*c = 'c';
	std::cout << "c = " << *c << std::endl;
	std::cout << mem;

	mem.myfree(c);
	std::cout << mem;

	Cpointer<int> *d = mem.myalloc<int>(sizeof(int));
	Cpointer<long long> *e = mem.myalloc<long long>(sizeof(long long));
	Cpointer<long long> *f = mem.myalloc<long long>(sizeof(long long));
	Cpointer<long long> *g = mem.myalloc<long long>(sizeof(long long));
	*d = 15;
	*e = 30;
	*f = 45;
	*g = 60;
	std::cout << "d,e,f = " << *d << ", " << *e << ", " << *f  << std::endl;
	mem.myfree(e);
	mem.myfree(g);
	std::cout << mem;

	mem.compress();
	std::cout << "d, f = " << *d << ", " << *f << std::endl;
	std::cout << mem;

	mem.myfree(d);
	mem.myfree(f);
	std::cout << mem;

	mem.compress();
	std::cout << mem;

	return 0;
}
