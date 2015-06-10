/**
  * @file ml_algobase.h
  * @brief 
  * @author malin malinbupt@gmail.com
  * @date 2015年04月02日 星期四 16时47分22秒
  * @version 
  * @note 
  */

#ifndef __ML_INTERNAL_ALGOBASE_H
#define __ML_INTERNAL_ALGOBASE_H


#include "ml_config.h"
#include "ml_pair.h"
#include "type_traits.h"
#include "ml_iterator_base.h"

__ML_BEGIN_NAMESPACE

template<typename InputIterator1, typename InputIterator2>
bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
{
	for (; first1 != last1; ++first1, ++first2)
	{
		if (*first1 != *first2) return false;
	}

	return true;
}

template<typename InputIterator1, typename InputIterator2, typename Predict>
bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, Predict pred)
{
	for (; first1 != last1; ++first1, ++first2)
	{
		if (!pred(*first1, *first2)) return false;
	}

	return true;
}


template<typename ForwardIterator, typename T>
void fill(ForwardIterator first, ForwardIterator last, const T &x)
{
	while (first != last) *first++ = x;
}


template<typename ForwardIterator, typename Size, typename T>
ForwardIterator fill_n(ForwardIterator first, Size n, const T &x)
{
	while (n-- > 0) *first++ = x;

	return first;
}


template<typename ForwardIterator1, typename ForwardIterator2>
inline void iter_swap(ForwardIterator1 a, ForwardIterator2 b)
{
	typename iterator_traits<ForwardIterator1>::value_type tmp;

	tmp = *a; *a = *b; *b = tmp;
}


template<typename InputIterator1, typename InputIterator2>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2)
{
	for (; first1 != last1, first2 != last2; ++first1, ++first2)
	{
		if (*first1 < *first2) return true;

		if (*first2 < *first1) return false;
	}

	return first1 == last1 && first2 != last2;
}

template<typename InputIterator1, typename InputIterator2, typename Compare>
bool lexicographical_comapre(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare cmp)
{
	for (; first1 != last1, first2 != last2; ++first1, ++first2)
	{
		if (cmp(*first1, *first2)) return true;

		if (cmp(*first2, *first1)) return false;
	}

	return first1 == last1 && first2 != last2;
}


template<typename T>
const T &max(const T &a, const T &b)
{
	return a < b ? b : a;
}

template<typename T, typename Compare>
const T &max(const T &a, const T &b, Compare comp)
{
	return comp(a, b) ? b : a;
}


template<typename T>
const T &min(const T &a, const T &b)
{
	return a < b ? a : b;
}

template<typename T, typename Compare>
const T &min(const T &a, const T &b, Compare comp)
{
	return comp(a, b) ? a : b;
}


template<typename InputIterator1, typename InputIterator2>
pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator2 last1, InputIterator2 first2)
{
	for (; first1 != last1 && *first1 == *first2; ++first1, ++last1);

	return make_pair(first1, first2);
}


template<typename T>
inline void swap(T &a, T &b)
{
	T tmp = a; a = b; b = tmp;
}


// copy

// special for iterators which support for arithmetic operations(random_access_iterator and pointer)
template<typename RandomIterator, typename OutputIterator, typename Distance>
OutputIterator __copy_d(RandomIterator first, RandomIterator last, OutputIterator dest, Distance*)
{
	for (Distance n = last - first; n > 0; --n, ++first, ++dest) *dest = *first;

	return dest;
}


// special for bidirectional_iterator
template<typename InputIterator, typename OutputIterator>
OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator dest, input_iterator_tag)
{
	for (; first != last; ++first, ++dest) *first = *dest;

	return dest;
}

// special for random_access_iterator_tag
template<typename RandomAccessIterator, typename OutputIterator>
OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator dest, random_access_iterator_tag)
{
	return __copy_d(first, last, dest, distance_type(first));
}


// dipatch function template
template<typename InputIterator, typename OutputIterator>
struct __copy_dispatch
{
	OutputIterator operator () (InputIterator first, InputIterator last, OutputIterator dest)
	{
		return __copy(first, last, dest, iterator_category(dest));
	}

};


// special for pointer which has tivial assignment operator. mememove is the best solution
template<typename T>
T *__copy_t(const T *first, const T *last, T *dest, __true_type)
{
	memmove(dest, first, (last - first) * sizeof(T));

	return dest + (last - first);
}

// special for pointer which has nontrivial assignment operator. invoke the random_iterator_tag version
template<typename T>
T *__copy_t(const T *first, const T *last, T *dest, __false_type)
{
	return __copy_d(first, last, dest, (ptrdiff_t*)(0));
}


// dipatch function template, special for pointer
template<typename T>
struct __copy_dispatch<T*, T*>
{
	T* operator () (T *first, T *last, T *dest)
	{
		typename __type_traits<T>::has_trivial_assignment_operator t;

		return __copy_t(first, last, dest, t);
	}

};

template<typename T>
struct __copy_dispatch<const T*, T*>
{
	T* operator () (const T *first, const T *last, T *dest)
	{
		typename __type_traits<T>::has_trivial_assignment_operator t;

		return __copy_t(first, last, dest, t);
	}

};


// common interface
template<typename InputIterator, typename OutputIterator>
OutputIterator copy(InputIterator first, InputIterator last, OutputIterator dest)
{
	return __copy_dispatch<InputIterator, OutputIterator>()(first, last, dest);
}



// copy_backward

// special for iterators which support for arithmetic operations(random_access_iterator and pointer)
template<typename RandomAccessIterator, typename BidirectionalIterator, typename Distance>
BidirectionalIterator __copy_backward_d(RandomAccessIterator first, RandomAccessIterator last, BidirectionalIterator dest,
			Distance*)
{
	for (Distance n = last - first; n > 0; --n) *--dest = *--last;

	return dest;
}


// special for bidirectional_iterator
template<typename BidirectionalIterator1, typename BidirectionalIterator2>
BidirectionalIterator2 __copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 dest,
		bidirectional_iterator_tag)
{
	while (first != last) *--dest = *--last;

	return dest;
}

// special for random_access_iterator
template<typename RandomAccessIterator, typename BidirectionalIterator>
inline BidirectionalIterator __copy_backward(RandomAccessIterator first, RandomAccessIterator last, BidirectionalIterator dest,
		random_access_iterator_tag)
{
	typename iterator_traits<RandomAccessIterator>::difference_type *dis(0);

	return __copy_backward_d(first, last, dest, dis);
}


// dispatch function template, used for iterator
template<typename BidirectionalIterator1, typename BidirectionalIterator2>
struct __copy_backward_dispatch
{
	inline BidirectionalIterator2 operator () (BidirectionalIterator1 first, BidirectionalIterator1 last,
			BidirectionalIterator2 dest)
	{
		typename iterator_traits<BidirectionalIterator1>::iterator_category category;

		return __copy_backward(first, last, dest, category);
	}

};


// special for pointer which has tivial assignment operator. mememove is the best solution
template<typename T>
inline T *__copy_backward_t(const T *first, const T *last, T *dest, __true_type)
{
	ptrdiff_t n = last - first;

	memmove(dest - n, first, n * sizeof(T));

	return dest + n;
}

// special for pointer which has nontrivial assignment operator. invoke the random_iterator_tag version
template<typename T>
inline T *__copy_backward_t(const T *first, const T *last, const T *dest, __false_type)
{
	return __copy_backward_d(first, last, dest, (ptrdiff_t*)(0));
}


// dispatch function tenplate, used for pointer
template<typename T>
struct __copy_backward_dispatch<T*, T*>
{
	inline T* operator () (T *first, T *last, T *dest)
	{
		typename __type_traits<T>::has_trivial_assignment_operator t;

		return __copy_backward_t(first, last, dest, t);
	}

};

template<typename T>
struct __copy_backward_dispatch<const T*, T*>
{
	inline T* operator () (const T *first, T *last, T *dest)
	{
		typename __type_traits<T>::has_trivial_assignment_operator t;

		return __copy_backward_t(first, last, dest, t);
	}
};


// common interface
template<typename BidirectionalIterator1, typename BidirectionalIterator2>
inline BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 dest)
{
	return __copy_backward_dispatch<BidirectionalIterator1, BidirectionalIterator2>()(first, last, dest);
}


__ML_END_NAMESPACE


#endif // __ML_INTERNAL_ALGOBASE_H


