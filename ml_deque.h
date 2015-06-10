/**
  * @file ml_deque.h
  * @brief 
  * @author malin malinbupt@gmail.com
  * @date 2015年04月30日 星期四 10时45分18秒
  * @version 
  * @note 
  */

#ifndef __DEQUE_INTERNAL_H
#define __DEQUE_INTERNAL_H


#include "ml_alloc.h"
#include "ml_config.h"
#include "ml_algobase.h"
#include "ml_construct.h"
#include "ml_iterator_base.h"
#include "ml_uninitialized.h"


__ML_BEGIN_NAMESPACE


static const size_t BYTE = 512; // buf size(byte)
static const size_t initial_map_size = 8; // initial map's size

// return the number of elements can be holded by the standard buf
inline size_t __deque_buf_size(size_t size)
{
	return size < BYTE ? BYTE / size : 1;
}


template<typename T>
struct __deque_iterator
{
	typedef T						   value_type;
	typedef value_type*				   pointer;
	typedef value_type&				   reference;
	typedef ptrdiff_t				   difference_type;
	typedef random_access_iterator_tag iterator_category;

	typedef T** map_pointer;
	typedef __deque_iterator self;


	inline static size_t buf_size()
	{
		return __deque_buf_size(sizeof(T));
	}

	__deque_iterator() : cur(0), first(0), last(0), node(0) {}

	__deque_iterator(map_pointer _node, pointer _cur) : node(_node), cur(_cur), first(*_node), last(first + buf_size()) {}


	reference operator * () const
	{
		return *cur;
	}

	pointer operator -> () const
	{
		return &(operator*());
	}

	self& operator ++ ()
	{
		++cur;
		if (cur == last)
		{
			set_node(node + 1);
			cur = first;
		}

		return *this;
	}

	self operator ++ (int)
	{
		self tmp = *this;
		++*this;
		return tmp;
	}

	self& operator -- ()
	{
		if (cur == first)
		{
			set_node(node - 1);
			cur = last;
		}
		--cur;

		return *this;
	}

	self operator -- (int)
	{
		self tmp = *this;
		--*this;
		return tmp;
	}

	difference_type operator - (const self &rhs) const
	{
		return (node - rhs.node - 1) * buf_size() + (cur - first) + (rhs.last - rhs.cur);
	}

	self& operator += (difference_type n)
	{
		difference_type offset = cur - first + n;

		if (0 <= offset && offset < buf_size())
			cur += n;
		else
		{
			difference_type node_offset = offset >= 0 ? offset / buf_size() : (offset + 1) / buf_size() - 1;
			set_node(node + node_offset);
			cur = first + (offset - node_offset * buf_size());
		}

		return *this;
	}

	const self operator + (difference_type n) const
	{
		self tmp = *this;
		tmp += n;
		return tmp;
	}

	self& operator -= (difference_type n)
	{
		return operator+=(-n);
	}

	const self operator - (difference_type n) const
	{
		self tmp = *this;
		tmp -= n;
		return tmp;
	}


	void set_node(map_pointer new_node)
	{
		node = new_node;
		first = *node;
		last = first + buf_size();
	}


	pointer cur;
	pointer first;
	pointer last;
	map_pointer node;
};


// relational operator

template<typename T>
bool operator == (const __deque_iterator<T> &lhs, const __deque_iterator<T> &rhs)
{
	return lhs.cur == rhs.cur;
}

template<typename T>
bool operator != (const __deque_iterator<T> &lhs, const __deque_iterator<T> &rhs)
{
	return !(lhs == rhs);
}

// random_access_iterator must support for operator <
template<typename T>
bool operator < (const __deque_iterator<T> &lhs, const __deque_iterator<T> &rhs)
{
	return lhs.node < rhs.node || (lhs.node == rhs.node && lhs.cur < rhs.cur);
}




template<typename T, typename Alloc = alloc>
class deque
{
public:
	typedef T				   value_type;
	typedef value_type*		   pointer;
	typedef value_type&		   reference;
	typedef const value_type*  const_pointer;
	typedef const value_type&  const_reference;
	typedef size_t			   size_type;
	typedef ptrdiff_t		   difference_type;

	typedef pointer* map_pointer;
	typedef __deque_iterator<T> iterator;


public:
	// construct
	explicit deque(const Alloc &a = Alloc()) : start(), finish(), map(0), map_size(0)
	{
		create_map(0);
	}

	explicit deque(size_type n, const_reference x = value_type(), const Alloc &a = Alloc())
	{
		fill_initialize(n, x);
	}

	template<typename InputIterator>
	deque(InputIterator first, InputIterator last)
	{
		// dispatched according to the iterator category
		range_initialize_dispatch(first, last, iterator_category(first));
	}

	// copy construct
	deque(const deque &rhs)
	{
		create_map(rhs.size());
		uninitialized_copy(rhs.begin(), rhs.end(), start);
	}

	// assignment operator
	deque& operator = (const deque &rhs)
	{
		if (this != &rhs)
		{
			size_type len = size();
			if (len >= rhs.size())
				erase(copy(rhs.begin(), rhs.end(), start), finish);
			else
			{
				iterator mid = rhs.begin() + len;
				copy(rhs.begin(), mid, start);
				insert(finish, mid, rhs.end());
			}
		}

		return *this;
	}

	// destruct
	~deque()
	{
		if (!map) return;
		destroy(start, finish);
		destroy_nodes(start.node, finish.node);
		destroy_map();
	}




	// unmutable funcition
	iterator begin() const
	{
		return start;
	}

	iterator end() const
	{
		return finish;
	}

	size_type size() const
	{
		return finish - start;
	}

	size_type max_size() const
	{
		return size_type(-1);
	}

	bool empty() const
	{
		return size() == 0;
	}

	const_reference front() const
	{
		return *start;
	}

	const_reference back() const
	{
		return *(finish - 1);
	}

	const_reference operator [] (size_type n) const
	{
		return *(start + n);
	}

	const_reference at(size_type n) const
	{
		if (n >= size()) throw out_of_range("deque's subscript is out of range");

		return operator[](n);
	}




	// mutable function
	
	reference front()
	{
		return *start;
	}

	reference back()
	{
		return *(finish - 1);
	}

	reference operator [] (size_type n)
	{
		return *(start + n);
	}

	reference at(size_type n)
	{
		if (n >= size()) throw out_of_range("deque's subscript is out of range");
		
		return operator[](n);
	}

	void push_back(const_reference x)
	{
		if (finish.cur < finish.last - 1)
			construct(finish.cur++, x);
		else
			push_back_aux(x);
	}

	void pop_back()
	{
		if (finish.cur == finish.first)
		{
			destroy_nodes(finish.node, finish.node + 1);
			finish.set_node(finish.node - 1);
			finish.cur = finish.last;
		}
		destroy(--finish.cur);
	}

	void push_front(const_reference x)
	{
		if (start.cur != start.first)
		{
			--start.cur;
			construct(start.cur, x);
		}
		else
			push_front_aux(x);
	}

	void pop_front()
	{
		destroy(start.cur++);
		if (start.cur == start.last)
		{
			destroy_nodes(start.node, start.node + 1);
			start.set_node(start.node + 1);
			start.cur = start.first;
		}
	}

	iterator insert(iterator position, const_reference x)
	{
		size_type before = position - start, after = finish - position;
		value_type x_copy = x;
		
		if (before < after)
		{
			push_front(front());
			iterator front1 = start;
			++front1;
			iterator front2 = front1;
			++front2;
			position = start + before;
			copy(front2, position + 1, front1);
		}
		else
		{
			push_back(back());
			iterator back1 = finish;
			--back1;
			iterator back2 = back1;
			--back2;
			position = start + before;
			copy_backward(position, back2, back1);
		}

		*position = x_copy;
		return position;
	}

	iterator insert(iterator position, size_type n, const_reference x)
	{
		size_type before = position - start, after = finish - position;
		value_type x_copy = x;

		if (before < after)
		{
			iterator nstart = reserve_elems_at_front(n);
			position = start + before;

			try
			{
				if (before <= n)
				{
					uninitialized_copy(start, position, nstart);
					uninitialized_fill(nstart + before, start, x_copy);
					fill_n(start, before, x_copy);
				}
				else
				{
					uninitialized_copy(start, start + n, nstart);
					copy(start + n, position, start);
					fill_n(start + (before - n), n, x_copy);
				}
			}
			catch (...)
			{
				destroy_nodes(nstart.node, start.node - 1);
				throw;
			}
				
			start = nstart;
			position = start + before;
		}
		else
		{
			iterator nfinish = reserve_elems_at_back(n);
			position = start + before;

			try
			{
				if (after <= n)
				{
					iterator mid = nfinish - after;
					uninitialized_copy(position, finish, mid);
					uninitialized_fill(finish, mid, x_copy);
					fill(position, finish, x_copy);
				}
				else
				{
					iterator mid = finish - n;
					uninitialized_copy(mid, finish, finish);
					copy_backward(position, mid, finish);
					fill_n(position, n, x_copy);
				}
			}
			catch (...)
			{
				destroy_nodes(finish.node + 1, nfinish.node);
				throw;
			}
			
			finish = nfinish;
		}

		return position;
	}

	template<typename InputIterator>
	iterator insert(iterator position, InputIterator first, InputIterator last)
	{
		return insert_dispatch(position, first, last, iterator_category(first));
	}

	iterator erase(iterator position)
	{
		size_type before = position - start, after = finish - position;

		if (before > after)
		{
			copy_backward(start, position, position + 1);
			pop_front();
		}
		else
		{
			copy(position + 1, finish, position);
			pop_back();
		}

		return start + before;
	}

	iterator erase(iterator first, iterator last)
	{
		size_type n = last - first, before = first - start, after = finish - last;

		if (before > after)
		{
			copy_backward(start, first, last);
			iterator nstart = start + n;
			destroy(start, nstart);
			destroy_nodes(start.node, nstart.node - 1);
			start = nstart;
		}
		else
		{
			copy(last, finish, first);
			iterator nfinish = finish - n;
			destroy(nfinish, finish);
			destroy_nodes(nfinish.node + 1, finish.node);
			finish = nfinish;
		}

		return start + before;
	}
	
	void swap(deque &rhs)
	{
		ml::swap(map, rhs.map);
		ml::swap(map_size, rhs.map_size);
		ml::swap(start, rhs.start);
		ml::swap(finish, rhs.finish);
	}

	void assign(size_type n, const_reference x)
	{
		size_type len = size();

		if (n < len)
		{
			erase(start + n, finish);
			fill(start, finish, x);
		}
		else
		{
			fill(start, finish, x);
			insert(finish, n - len, x);
		}
	}

	template<typename InputIterator>
	void assign(InputIterator first, InputIterator last)
	{
		assign_dispatch(first, last, iterator_category(first));
	}

	void clear()
	{
		for (map_pointer cur = start.node + 1; cur < finish.node; ++cur)
		{
			destroy(*cur, *cur + buf_size());
		}
		
		// reserve a buf
		destroy(start.cur, start.last);
		if (finish != start)
		{
			destroy(finish.first, finish.cur);
			data_allocator::deallocate(finish.first, buf_size());
		}
	}




private:
	void fill_initialize(size_type n, const_reference x)
	{
		create_map(n);
		map_pointer cur;

		try
		{
			for (cur = start.node; cur < finish.node; ++cur)
				uninitialized_fill_n(*cur, buf_size(), x);
			uninitialized_fill(finish.first, finish.cur, x);
		}
		catch (...)
		{
			destroy(start, iterator(cur, *cur));
			destroy_nodes(start.node, finish.node);
			destroy_map();
			throw;
		}
	}

	// allocate memory for the map table ans initial start and finish
	void create_map(size_type n)
	{
		size_type num_nodes = n / buf_size() + 1;
		map_size = max(initial_map_size, num_nodes + 2);

		map = map_allocator::allocate(map_size);
		map_pointer nstart = map + (map_size - num_nodes) / 2;
		map_pointer nfinish = nstart + num_nodes - 1;

		try
		{
			create_nodes(nstart, nfinish);
		}
		catch (...)
		{
			destroy_map();
			map = 0;
			map_size = 0;
			throw;
		}

		start.set_node(nstart);
		start.cur = *nstart;
		finish.set_node(nfinish);
		finish.cur = *nfinish + n % buf_size();
	}

	// deallocate the memory of map
	void destroy_map()
	{
		map_allocator::deallocate(map, map_size);
	}

	// allocate memory for elements
	void create_nodes(map_pointer nstart, map_pointer nfinish)
	{
		map_pointer cur;

		try
		{
			for (cur = nstart; cur <= nfinish; ++cur)
				*cur = data_allocator::allocate(buf_size());
		}
		catch (...)
		{
			destroy_nodes(nstart, cur);
			throw;
		}
	}

	// deallocate memory for elements
	void destroy_nodes(map_pointer first, map_pointer last)
	{
		for (map_pointer cur = first; cur <= last; ++cur)
			data_allocator::deallocate(*cur, buf_size());
	}

	// initialized according to a internal
	template<typename InputIterator>
	void range_initialize_dispatch(InputIterator first, InputIterator last, input_iterator_tag)
	{
		create_map(0);

		try
		{
			for(; first != last; ++first)
				push_back(*first);
		}
		catch (...)
		{
			clear();
		}
	}

	template<typename ForwardIterator>
	void range_initialize_dispatch(ForwardIterator first, ForwardIterator last, random_access_iterator_tag)
	{
		size_type n = distance(first, last);
		create_map(n);

		map_pointer cur;

		try
		{
			for (cur = start.node; cur != finish.node; ++cur)
			{
				ForwardIterator mid;
				advance(mid, buf_size());
				uninitialized_copy(first, mid, *cur);
				first = mid;
			}
			uninitialized_copy(first, last, finish.first);
		}
		catch (...)
		{
			destroy(start, iterator(cur, *cur));
			destroy_nodes(start.node, finish.node);
			destroy_map();
			throw;
		}
	}

	void push_back_aux(const_reference x)
	{
		reserve_map_at_back();
		*(finish.node + 1) = data_allocator::allocate(buf_size());
		try
		{
			construct(finish.cur, x);
		}
		catch (...)
		{
			destroy_nodes(finish.node + 1, finish.node + 2);
			throw;
		}

		finish.set_node(finish.node + 1);
		finish.cur = finish.first;
	}

	void reserve_map_at_back(size_type nodes_to_add = 1)
	{
		if (nodes_to_add + 1 > map_size - (finish.node - map))
			reallocate_map(nodes_to_add, false);
	}

	void push_front_aux(const_reference x)
	{
		reserve_map_at_front();
		*(start.node - 1) = data_allocator::allocate(buf_size());
		try
		{
			construct(*(start.node - 1) + buf_size() - 1, x);
		}
		catch (...)
		{
			destroy_nodes(start.node - 1, start.node);
			throw;
		}

		start.set_node(start.node - 1);
		start.cur = start.last - 1;
	}

	void reserve_map_at_front(size_type nodes_to_add = 1)
	{
		if (nodes_to_add > start.node - map)
			reallocate_map(nodes_to_add, true);
	}

	// type is true indicated that add new node at front
	void reallocate_map(size_type nodes_to_add, bool type)
	{
		size_type old_size = finish.node - start.node + 1, new_size = old_size + nodes_to_add;
		
		map_pointer new_start;
		if (map_size > 2 * new_size)
		{
			new_start = map + (map_size - new_size) / 2 + (type ? nodes_to_add : 0);
			if (new_start <= start.node)
				copy(start.node, finish.node + 1, new_start);
			else
				copy_backward(start.node, finish.node + 1, new_start + old_size);
		}
		else
		{
			size_type new_map_size = map_size + max(map_size, nodes_to_add) + 2;
			map_pointer new_map = map_allocator::allocate(new_map_size);
			new_start = new_map + (new_map_size - new_size) / 2 + (type ? nodes_to_add : 0);
			copy(start.node, finish.node + 1, new_start);
			
			destroy_map();
			map = new_map;
			map_size = new_map_size;
		}
		
		start.set_node(new_start);
		finish.set_node(new_start + old_size - 1);
	}

	// adjust the map to ensure n elements' space in the front
	iterator reserve_elems_at_front(size_type n)
	{
		size_type vacancies = start.cur - start.first;
		
		if (vacancies < n)
		{
			size_type extra_nodes_size = (n - vacancies + buf_size() - 1) / buf_size();
			reserve_map_at_front(extra_nodes_size);
			create_nodes(map, start.node - 1);
		}

		return start - n;
	}

	// adjust the map to ensure n elements' space in the front
	iterator reserve_elems_at_back(size_type n)
	{
		size_type vacancies = finish.last - finish.cur;

		if (vacancies < n)
		{
			size_type extra_nodes_size = (n - vacancies + buf_size() - 1) / buf_size();
			reserve_map_at_back(extra_nodes_size);
			create_nodes(finish.node + 1, map + map_size);
		}

		return finish + n;
	}

	template<typename InputIterator>
	iterator insert_dispatch(iterator position, InputIterator first, InputIterator last, input_iterator_tag)
	{
		size_type offset = position - start;

		for (; first != last; ++first)
			position = insert(position, *first) + 1;

		return start + offset;
	}

	template<typename ForwardIterator>
	iterator insert_dispatch(iterator position, ForwardIterator first, ForwardIterator last, forward_iterator_tag)
	{
		size_type n = distance(first, last), before = position - start, after = finish - position;

		if (before < after)
		{
			 iterator nstart = reserve_elems_at_front(n);
			 position = start + before;

			 try
			 {
				 if (before <= n)
				 {
					 copy(start, position, nstart);
					 ForwardIterator mid = first;
					 advance(first, n - before);
					 uninitialized_copy(first, mid, nstart + before);
					 copy(mid, last, start);
				 }
				 else
				 {
					 iterator mid = start + n;
					 uninitialized_copy(start, mid, nstart);
					 copy(mid, position, start);
					 copy(first, last, position - n);
				 }
			 }
			 catch (...)
			 {
				 destroy_nodes(nstart.node, start.node - 1);
			 }
			 start = nstart;
			 position -= n;
		}
		else
		{
			iterator nfinish = reserve_elems_at_back(n);
			position = start + before;

			try
			{
				if (after <= n)
				{
					uninitialized_copy(position, finish, nfinish - after);
					ForwardIterator mid = first;
					advance(mid, after);
					copy(first, mid, position);
					uninitialized_copy(mid, last, finish);
				}
				else
				{
					iterator mid1 = finish - n, mid2 = nfinish - n;
					uninitialized_copy(mid1, finish, mid2);
					copy_backward(position, mid1, mid2);
					copy(first, last, position);
				}
			}
			catch (...)
			{
				destroy(finish.node + 1, nfinish.node);
				throw;
			}
			finish = nfinish;
		}

		return position;
	}

	template<typename InputIterator>
	void assign_dispatch(InputIterator first, InputIterator last, input_iterator_tag)
	{
		iterator cur = start;
		
		for (; first != last && cur != finish; ++first, ++cur) *cur = *first;

		if (first == last)
			erase(cur, finish);
		else
			insert(cur, first, last);
	}

	template<typename ForwardIterator>
	void assign_dispatch(ForwardIterator first, ForwardIterator last, forward_iterator_tag)
	{
		size_type len = size(), n = distance(first, last);

		if (n < len)
			erase(copy(first, last, start), finish);
		else
		{
			ForwardIterator mid = first;
			advance(mid, len);
			copy(first, mid, start);
			insert(finish, mid, last);
		}
	}
	



private:
	typedef simple_alloc<value_type, Alloc> data_allocator;
	typedef simple_alloc<pointer, Alloc> map_allocator;

	inline static size_type buf_size() {return __deque_buf_size(sizeof(T));}


private:
	map_pointer map;
	size_t map_size; // map's size, not the deque's size

	iterator start;
	iterator finish;

};


template<typename T, typename Alloc = alloc>
void swap(deque<T, Alloc> &x, deque<T, Alloc> &y)
{
	x.swap(y);
}

// relational operations
template<typename T, typename Alloc = alloc>
bool operator == (const deque<T, Alloc> &x, const deque<T, Alloc> &y)
{
	return x.size() == y.size() && equal(x.begin(), x.end(), y.begin());
}

template<typename T, typename Alloc = alloc>
bool operator != (const deque<T, Alloc> &x, const deque<T, Alloc> &y)
{
	return !(x == y);
}

template<typename T, typename Alloc = alloc>
bool operator < (const deque<T, Alloc> &x, const deque<T, Alloc> &y)
{
	return lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

template<typename T, typename Alloc = alloc>
bool operator <= (const deque<T, Alloc> &x, const deque<T, Alloc> &y)
{
	return !(y < x);
}

template<typename T, typename Alloc = alloc>
bool operator > (const deque<T, Alloc> &x, const deque<T, Alloc> &y)
{
	return y < x;
}

template<typename T, typename Alloc = alloc>
bool operator >= (const deque<T, Alloc> &x, const deque<T, Alloc> &y)
{
	return !(x < y);
}




__ML_END_NAMESPACE


#endif // __DEQUE_INTERNAL_H
