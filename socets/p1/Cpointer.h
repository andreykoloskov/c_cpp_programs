#ifndef CPOINTER_H_INCLUDED
#define CPOINTER_H_INCLUDED

#include <iostream>

//External pointer
template<class T> class Cpointer
{
public:
	void *mem;
	int num;

	Cpointer(void *m = NULL, int n = 0): mem(m), num(n) {}

	~Cpointer() {}

	Cpointer<T> operator = (T ch);

	template<class P>
	friend std::ostream &operator << (std::ostream &stream, Cpointer<P> cp);
};

#endif
