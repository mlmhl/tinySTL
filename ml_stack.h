/**
  * @file ml_stack.h
  * @brief 
  * @author malin malinbupt@gmail.com
  * @date 2015年05月09日 星期六 11时22分44秒
  * @version 
  * @note 
  */

#ifndef __STACK_INTERNAL_H
#define __STACK_INTERNAL_H


#include "ml_config.h"
#include "ml_deque.h"

__ML_BEGIN_NAMESPACE


template<typename T, typename Container = deque<T> >
class stack
{
public:
	typedef typename Container::value_type value_type;
	typedef typename Container::size_type size_type;
	typedef typename Container::reference reference;
	typedef typename Container::const_reference const_reference;

	typedef Container Container_type;

	explicit stack(Container _c = Container()) : c(_c) {}

	bool empty() const
	{
		return c.empty();
	}

	size_type size() const
	{
		return c.size();
	}

	reference top()
	{
		return c.back();
	}

	const_reference top() const
	{
		return c.back();
	}

	void push(const_reference x)
	{
		c.push_back(x);
	}

	void pop()
	{
		c.pop_back();
	}

	void swap(stack &rhs)
	{
		c.swap(rhs.c);
	}


private:
	Container c;

};


template<typename T, typename Container = deque<T> >
void swap(stack<T, Container> &x, stack<T, Container> &y)
{
	x.swap(y);
}


template<typename T, typename Container = deque<T> >
void operator == (const stack<T, Container> &x, const stack<T, Container> &y)
{
	return x.c == y.c;
}

template<typename T, typename Container = deque<T> >
void operator != (const stack<T, Container> &x, const stack<T, Container> &y)
{
	return !(x == y);
}

template<typename T, typename Container = deque<T> >
void operator < (const stack<T, Container> &x, const stack<T, Container> &y)
{
	return x.c < y.c;
}

template<typename T, typename Container = deque<T> >
void operator <= (const stack<T, Container> &x, const stack<T, Container> &y)
{
	return !(y < x);
}

template<typename T, typename Container = deque<T> >
void operator > (const stack<T, Container> &x, const stack<T, Container> &y)
{
	return y < x;
}

template<typename T, typename Container = deque<T> >
void operator >= (const stack<T, Container> &x, const stack<T, Container> &y)
{
	return !(x < y);
}




__ML_END_NAMESPACE


#endif // __STACK_INTERNAL_H
