/**
  * @file ml_queue.h
  * @brief 
  * @author malin malinbupt@gmail.com
  * @date 2015年05月09日 星期六 14时55分40秒
  * @version 
  * @note 
  */

#ifndef __QUEUE_INTERNAL_H
#define __QUEUE_INTERNAL_H


#include "ml_config.h"
#include "ml_deque.h"
#include "ml_heap.h"
#include "ml_vector.h"
#include "ml_algobase.h"
#include "ml_function.h"


__ML_BEGIN_NAMESPACE


/**
 * @breif queue
 *
 */
template<typename T, typename Container = deque<T> >
class queue
{
public:
	typedef typename Container::value_type value_type;
	typedef typename Container::size_type size_type;
	typedef typename Container::reference reference;
	typedef typename Container::const_reference const_reference;

	typedef Container Container_type;

public:
	explicit queue(const Container &_c = Container()) : c(_c) {}

	bool empty() const
	{
		return c.empty();
	}

	size_type size() const
	{
		return c.size();
	}

	reference front()
	{
		return c.front();
	}

	const_reference front() const
	{
		return c.front();
	}

	reference back()
	{
		return c.back();
	}

	const_reference back() const
	{
		return c.back();
	}

	void push(const_reference x)
	{
		c.push_back(x);
	}

	void pop()
	{
		c.pop_front();
	}

	void swap(queue &rhs)
	{
		c.swap(rhs.c);
	}


private:
	Container c;

};


template<typename T, typename Container = deque<T> >
void swap(queue<T, Container> &x, queue<T, Container> &y)
{
	x.swap(y);
}


template<typename T, typename Container = deque<T> >
void operator == (const queue<T, Container> &x, const queue<T, Container> &y)
{
	return x.c == y.c;
}

template<typename T, typename Container = deque<T> >
void operator != (const queue<T, Container> &x, const queue<T, Container> &y)
{
	return !(x == y);
}

template<typename T, typename Container = deque<T> >
void operator < (const queue<T, Container> &x, const queue<T, Container> &y)
{
	return x.c < y.c;
}

template<typename T, typename Container = deque<T> >
void operator <= (const queue<T, Container> &x, const queue<T, Container> &y)
{
	return !(y < x);
}

template<typename T, typename Container = deque<T> >
void operator > (const queue<T, Container> &x, const queue<T, Container> &y)
{
	return y < x;
}

template<typename T, typename Container = deque<T> >
void operator >= (const queue<T, Container> &x, const queue<T, Container> &y)
{
	return !(x < y);
}




/**
 * @brief priority_queue
 *
 */
template<typename T, typename Container = vector<T>, typename Compare = less<typename Container::value_type> >
class priority_queue
{
public:
	typedef typename Container::size_type		size_type;
	typedef typename Container::value_type		value_type;
	typedef typename Container::reference		reference;
	typedef typename Container::const_reference const_reference;

	typedef Container container_type;

public:
	explicit priority_queue(const Compare &_comp = Compare(), const Container &_c = Container()) : c(_c), comp(_comp) {}

	template<typename InputIterator>
	priority_queue(InputIterator first, InputIterator last,
			const Compare &_comp = Compare(), const Container &_c = Container()) : c(_c), comp(_comp)
	{
		c.insert(c.end(), first, last);
		make_heap(c.begin(), c.end());
	}

	size_type size() const
	{
		return c.size();
	}

	bool empty() const
	{
		return c.empty();
	}

	const_reference top() const
	{
		return c.front();
	}

	void push(const_reference x)
	{
		try
		{
			c.push_back(x);
			push_heap(c.begin(), c.end());
		}
		catch (...)
		{
			c.clear();
			throw;
		}
	}

	void pop()
	{
		try
		{
			pop_heap(c.begin(), c.end());
			c.pop_back();
		}
		catch (...)
		{
			c.clear();
			throw;
		}
	}

	void swap(priority_queue &rhs)
	{
		ml::swap(c, rhs.c);
		ml::swap(comp, rhs.comp);
	}


private:
	Container c;
	Compare comp;

};


template<typename T, typename Container = vector<T>, typename Compare = less<typename Container::value_type> >
void swap(priority_queue<T, Container, Compare> &x, priority_queue<T, Container, Compare> &y)
{
	x.swap(y);
}




__ML_END_NAMESPACE


#endif // __QUEUE_INTERNAL_H
