#ifndef CPOINTER_HPP_INCLUDED
#define CPOINTER_HPP_INCLUDED

#include "Vpointer.h"
#include "Cpointer.h"
#include "Memory.h"

template<class T>
Cpointer<T> Cpointer<T>::operator = (T ch)
{
	*((T *) (((Memory *) mem)->vvp[num]).mem) = ch;
	return *this;
}

template<class P>
std::ostream &operator << (std::ostream &stream, Cpointer<P> cp)
{
	std::vector<Vpointer>::iterator p = ((Memory *) (cp.mem))->vvp.begin();
	p += cp.num;
	stream << *((P *) (p->mem));
	return stream;
}

#endif
