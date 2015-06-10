/**
  * @file ml_vector.h
  * @brief 
  * @author malin malinbupt@gmail.com
  * @date 2015年04月06日 星期一 10时58分11秒
  * @version 
  * @note 
  */

#ifndef __ML_INTERNAL_VECTOR_H
#define __ML_INTERNAL_VECTOR_H

#include "ml_config.h"
#include "ml_alloc.h"
#include "ml_exception.h"
#include "ml_algobase.h"
#include "ml_construct.h"
#include "ml_uninitialized.h"
#include "ml_iterator_base.h"


__ML_BEGIN_NAMESPACE


template<typename T, typename Alloc = alloc>
class vector
{
public:
	typedef T				  value_type;
	typedef size_t			  size_type;
	typedef ptrdiff_t		  difference_type;
	typedef value_type*		  pointer;
	typedef const value_type* const_pointer;
	typedef value_type*		  iterator;
	typedef const value_type* const_iterator;
	typedef value_type&		  reference;
	typedef const value_type& const_reference;

	typedef Alloc allocator_type;

public:
	// construct functions
	explicit vector(const allocator_type &a = allocator_type()) : start(0), finish(0), end_of_storage(0) {}

	explicit vector(size_type n, const value_type &value = value_type(), const allocator_type &_a = allocator_type())
	{
		start = data_allocator::allocate(n);

		end_of_storage = finish = uninitialized_fill_n(start, n, value);
	}

	template<typename InputIterator> 
	vector(InputIterator first, InputIterator last)
	{
		typename iterator_traits<InputIterator>::iterator_category cat;

		// dispatched by iterator tag
		range_initialize(first, last, cat);
	}

	vector(const vector &rhs)
	{
		start = allocate_and_copy(rhs.size(), rhs.begin(), rhs.end());
		finish = start + rhs.size(); end_of_storage = finish;
	}

	/*
	 * assignment operator
	 * 
	 * version1: invoke clear() and copy_aux(), but this method is not efficient, because unnecessaried memory raleasing
	 * and allcaoting may be caused
	 *
	 * version2: implement optimzation according to the old size and capacity sa well as new size
	 *
	 */
	vector& operator = (const vector &rhs)
	{
		if (this != &rhs)
		{
			size_type new_size = rhs.size();

			if (new_size > capacity())
			{
				destroy_and_deallocate();	
				start = allocate_and_copy(new_size, rhs.begin(), rhs.end());

				finish = start + new_size; end_of_storage = finish;
			}
			else if (new_size >= size())
			{
				uninitialized_copy(rhs.begin() + size(), rhs.end(), copy(rhs.begin(), rhs.begin() + size(), start));
				finish = start + new_size;
			}
			else
			{
				copy(rhs.begin(), rhs.end(), start);
				destroy(start + new_size, finish);
				finish = start + new_size;
			}
		}

		return *this;
	}




	// unmutable functions
	
	size_type size() const {return end() - begin();}

	size_type capacity() const {return size_type(end_of_storage - start);}

	bool empty() const {return begin() == end();}

	iterator begin() const {return start;}

	iterator end() const {return finish;}

	const_reference front() const {return *begin();}

	const_reference back() const {return *(end() - 1);}

	const_reference operator [] (size_type index) const {return *(begin() + index);}

	const_reference at(size_type index) const
	{
		range_check(index);

		return operator[](index);
	}




	// capacity mutable functions
	
	void resize(size_type n, const value_type &x = value_type())
	{
		if (size() > n) erase(start + n, finish);
		else insert(finish, n - size(), x);
	}

	void reserve(size_type n)
	{
		if (n <= size()) return;

		iterator new_start = allocate_and_copy(n, start, finish);
		destroy_and_deallocate();

		start = new_start; finish = start + size(); end_of_storage = start + n;
	}

	void assign(size_type n, const value_type &x)
	{
		if (capacity() >= n)
		{
			size_type old_size = size();

			if (old_size <= n)
			{
				fill(start, finish, x);
				finish = uninitialized_fill_n(finish, n - old_size, x);
			}
			else
			{
				fill_n(start, n, x); destroy(start + n, finish);
			}
		}
		else
		{
			iterator new_start = data_allocator::allocate(n);
			try
			{
				uninitialized_fill_n(new_start, n, x);
			}
			catch (...)
			{
				data_allocator::deallocate(new_start, n);
				throw;
			}

			destroy_and_deallocate();
			start = new_start; finish = start + n; end_of_storage = finish;
		}
	}

	template<typename InputIterator>
	void assign(InputIterator first, InputIterator last)
	{
		typename iterator_traits<InputIterator>::iterator_category cat;
		assign_dispatch(first, last, cat);
	}




	// mutable function, the memory may be modified

	reference front() {return *begin();}

	reference back() {return *(end() - 1);}

	reference operator [] (size_type index) {return *(begin() + index);}

	reference at(size_type index)
	{
		range_check();

		return operator[](index);
	}

	void push_back(const value_type &x)
	{
		// needn't special treatment for the exception which may be rised by the constructor, the user code
		// should handle the exception correctly
		if (finish < end_of_storage) construct(finish++, x);
		
		else elem_insert_aux(end(), x);
	}

	void pop_back()
	{
		if (empty()) throw underflow_error("vector is empty!");
			
		destroy(--finish);
	}

	iterator insert(iterator position, const value_type &x)
	{
		// needn't special treatment for the exception which may be rised by the constructor, the user code
		// should handle the exception correctly
		if (finish < end_of_storage && position == finish) construct(finish++, x);

		else
		{
			size_type n = position - start;
			elem_insert_aux(position, x);
			return start + n;
		}
	}

	void insert(iterator position, size_type n, const value_type& x)
	{
		if (n == 0) return;

		if (size_type(end_of_storage - finish) >= n)
		{
			size_type elems_after = finish - position;
			iterator old_finish = finish;

			if (elems_after < n)
			{
				finish += n - elems_after;
				uninitialized_fill(old_finish, finish, x);

				uninitialized_copy(position, old_finish, finish);

				fill(position, old_finish, x);

				finish += elems_after;
			}
			else
			{
				value_type tmp = x;

				finish += n;
				uninitialized_copy(old_finish - n, old_finish, old_finish);

				// must use copy_abckward instead of copy
				copy_backward(position, old_finish - n, old_finish);

				fill_n(position, n, tmp);
			}
		}
		else
		{
			// must allocate a bigger memory

			size_type old_size = size(), new_size = old_size + max(old_size, n);

			iterator new_start = data_allocator::allocate(new_size), new_finish = new_start;

			try
			{
				value_type tmp = x;

				new_finish = uninitialized_copy(start, position, new_finish);
				new_finish = uninitialized_fill_n(new_finish, n, tmp);
				new_finish = uninitialized_copy(position, finish, new_finish);
			}
			catch (...)
			{
				destroy(new_start, new_finish);
				data_allocator::deallocate(new_start, new_size);

				throw;
			}

			// destroy and release the old vector
			destroy_and_deallocate();

			start = new_start; finish = new_finish; end_of_storage = start + new_size;
		}
	}

	template<typename InputIterator>
	void insert(iterator position, InputIterator first, InputIterator last)
	{
		if (first == last) return;

		// dispatch
		typename iterator_traits<InputIterator>::iterator_category cat;
		insert_dispatch(position, first, last, cat);
	}

	iterator erase(iterator position)
	{
		if (position + 1 != end()) copy(position + 1, end(), position);
		destroy(--finish);

		return position;
	}

	iterator erase(iterator first, iterator last)
	{
		if (last != end()) copy(last, end(), first);
		size_type n = (last - first);
		destroy(end() - n, end());
		finish -= n;

		return first;
	}

	void clear()
	{
		destroy(begin(), end());
		data_allocator::deallocate(begin(), size());

		start = 0; finish = 0; end_of_storage = 0;
	}

	void swap(vector &x)
	{
		ml::swap(start, x.start);
		ml::swap(finish, x.finish);
		ml::swap(end_of_storage, x.end_of_storage);
	}


private:
	void range_check(size_type n) const
	{
		if (n >= size()) throw out_of_range("vector's subscript is out of range!");
	}


	// destroy and release memroy
	void destroy_and_deallocate()
	{
		destroy(start, finish);
		data_allocator::deallocate(start, size());
	}


	//used for internal construct
	template<typename RandomAccessIterator>
	void range_initialize(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag)
	{
		typename iterator_traits<RandomAccessIterator>::difference_type n = last - first;

		start = allocate_and_copy(n, first, last);
		end_of_storage = finish = start + n;
	}

	template<typename InputIterator>
	void range_initialize(InputIterator first, InputIterator last, input_iterator_tag)
	{
		while (first != last) push_back(*first++);
	}




	// allocate memory and fill it by coping from ainternal
	template<typename InputIterator>
	inline iterator allocate_and_copy(size_type n, InputIterator first, InputIterator last)
	{
		iterator result = data_allocator::allocate(n);
		
		try {uninitialized_copy(first, last, result);}
		catch (...) {data_allocator::deallocate(result, n); throw;}

		return result;
	}


	// this function is used for insert an element in the vector
	void elem_insert_aux(iterator position, const T &x)
	{
		if (finish < end_of_storage)
		{
			construct(finish, *(finish - 1));
			value_type tmp = x;	// as x may be an element of the vector, we must reserve a temporary
			copy_backward(position, finish, finish + 1);
			*position = tmp;
			++finish;
		}
		else
		{
			size_type old_size = size(), new_size = old_size == 0 ? 1 : 2 * old_size;
			iterator new_start = data_allocator::allocate(new_size), new_finish = new_start;

			try
			{
				// transform the first half
				new_finish = uninitialized_copy(start, position, new_finish);
				construct(new_finish++, x);
				// transform the second half
				new_finish = uninitialized_copy(position, finish, new_finish);
			}
			catch (...)
			{
				// roll back
				destroy(new_start, new_finish);
				data_allocator::deallocate(new_start, new_size);

				throw;
			}

			destroy(start, finish);
			data_allocator::deallocate(start, old_size);

			start = new_start; finish = new_finish; end_of_storage = start + new_size;
		}
	}


	// dispatch funvtion for interval insert
	template<typename InputIterator>
	void insert_dispatch(iterator position, InputIterator first, InputIterator last, input_iterator_tag)
	{
		// if the input iterator's category is input_iterator_tag, we can only insert them one by one
		for (; first != last; ++first) position = insert(position, *first) + 1;
	}

	template<typename ForwardIterator>
	void insert_dispatch(iterator position, ForwardIterator first, ForwardIterator last, forward_iterator_tag)
	{
		size_type n = distance(first, last);

		if (end_of_storage - finish >= n)
		{
			size_type elems_after = finish - position;
			iterator old_finish = finish;

			if (elems_after < n)
			{
				finish += n - elems_after;

				uninitialized_copy(position, old_finish, finish);
				
				ForwardIterator mid = first;
				advance(mid, elems_after);
				copy(first, mid, position);
				uninitialized_copy(mid, last, old_finish);

				finish += elems_after;
			}
			else
			{
				finish += n;
				uninitialized_copy(old_finish - n,  old_finish, old_finish);
				copy_backward(position, old_finish - n, old_finish);
				copy(first, last, position);
			}
		}
		else
		{
			size_type old_size = size(), new_size = old_size + max(old_size, n);
			iterator new_start = data_allocator::allocate(new_size), new_finish = new_start;

			try
			{
				new_finish = uninitialized_copy(start, position, new_finish);
				new_finish = uninitialized_copy(first, last, new_finish);
				new_finish = uninitialized_copy(position, finish, new_finish);
			}
			catch (...)
			{
				destroy(new_start, new_finish);
				data_allocator::deallocate(new_start, new_size);

				throw;
			}

			destroy(start, finish);
			data_allocator::deallocate(start, old_size);

			start = new_start; finish = new_finish; end_of_storage = new_start + new_size;
		}
	}


	// interval assign function for input_iterator_tag
	template<typename InputIterator>
	void assign_dispatch(InputIterator first, InputIterator last, input_iterator_tag)
	{
		iterator cur = start;
		for (; cur != finish, first != last; ++first, ++cur) *cur = *first;

		if (cur != finish) erase(cur, finish);
		else insert(finish, first, last);
	}

	// interval assign funcion for forward_iterator_tag
	template<typename ForwardIterator>
	void assign_dispatch(ForwardIterator first, ForwardIterator last, forward_iterator_tag)
	{
		size_type n = distance(first, last);

		if (capacity() >= n)
		{
			size_type old_size = size();

			if (old_size < n)
			{
				ForwardIterator mid;
				advance(mid, old_size);
				copy(first, mid, start);
				finish = uninitialized_copy(mid, last, finish);
			}
			else
			{
				iterator new_finish = copy(first, last, start);
				destroy(new_finish, finish);
				finish = new_finish;
			}
		}
		else
		{
			iterator new_start = allocate_and_copy(n, first, last);
			destroy_and_deallocate();
			start = new_start; finish = start + n; end_of_storage = finish;
		}
	}




private:
	typedef simple_alloc<value_type, Alloc> data_allocator;

	iterator start;
	iterator finish;
	iterator end_of_storage;

};


// relation operators
	
template<typename T, typename Alloc>
inline bool operator < (const vector<T, Alloc> &x, const vector<T, Alloc> &y)
{
	return lexicgraphical_compare(x.begin(), x.end(), y.begin(), y.end());
}

template<typename T, typename Alloc>
bool operator <= (const vector<T, Alloc> &x, const vector<T, Alloc> &y)
{
	return !(y < x);
}

template<typename T, typename Alloc>
bool operator > (const vector<T, Alloc> &x, const vector<T, Alloc> &y)
{
	return y < x;
}

template<typename T, typename Alloc>
bool operator >= (const vector<T, Alloc> &x, const vector<T, Alloc> &y)
{
	return !(x < y);
}

template<typename T, typename Alloc>
bool operator == (const vector<T, Alloc> &x, vector<T, Alloc> &y)
{
	return x.size() == y.size() && equal(x.begin(), x.end(), y.begin());
}

template<typename T, typename Alloc>
bool operator != (const vector<T, Alloc> &x, const vector<T, Alloc> &y)
{
	return !(x == y);
}

template<typename T, typename Alloc>
inline void swap(vector<T, Alloc> &x, vector<T, Alloc> &y)
{
	x.swap(y);
}


__ML_END_NAMESPACE


#endif //  __ML_INTERNAL_VECTOR_H
