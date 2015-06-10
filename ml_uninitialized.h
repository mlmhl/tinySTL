/**
  * @file ml_uninitialized.h
  * @brief 
  * @author malin malinbupt@gmail.com
  * @date 2015年04月02日 星期四 14时27分38秒
  * @version 
  * @note 
  */

#ifndef __ML_INTERNAL_UNINITIALIZED_H
#define __ML_INTERNAL_UNINITIALIZED_H 

#include <cstring>

#include "ml_config.h"
#include "type_traits.h"
#include "ml_algobase.h"
#include "ml_iterator_base.h"

__ML_BEGIN_NAMESPACE


// definition for uninitialized_copy

template<typename InputIterator, typename ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator dest, __false_type)
{
	ForwardIterator cur = dest;

	__ML_TRY
	{
		while (first != last)
		{
			constructor(&*cur, *first); ++first; ++cur;
		}
	}
	__ML_UNWIND(destroy(dest, cur))

	return dest;
}

template<typename InputIterator, typename ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator dest, __true_type) 
{
	return copy(first, last, dest);
}

template<typename InputIterator, typename ForwardIterator, typename T>
inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator dest, T*)
{
	typedef typename __type_traits<T>::is_POD_type is_POD;

	return __uninitialized_copy_aux(first, last, dest, is_POD());
}

template<typename InputIterator, typename ForwardIterator>
inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator dest)
{
	return __uninitialized_copy(first, last, dest, value_type(dest));
}

///< @todo why just partial specialist for char?
inline char *uninitialized_copy(char *first, char *last, char *dest)
{
	memmove(dest, first, last - first);

	return dest + (last - first);
}

// end of definition for uninitialized_copy


// definition for uninitialized_fill

template<typename ForwardIterator, typename T>
inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T &x, __false_type)
{
	ForwardIterator cur = first;

	__ML_TRY
	{
		while (cur != last)
		{
			constructor(&*cur, x); ++cur;
		}
	}
	__ML_UNWIND(destroy(first, cur));
}

template<typename ForwardIterator, typename T>
inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T &x, __true_type)
{
	fill(first, last, x);
}


template<typename ForwardIterator, typename T1, typename T2>
inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T1 &x, T2*)
{
	typedef typename __type_traits<T1>::is_POD_type is_POD;

	__uninitialized_fill_aux(first, last, x, is_POD());
}

template<typename ForwardIterator, typename T>
inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T &x)
{
	__uninitialized_fill(first, last, x, value_type(first));
}

// end of definition for uninitialized_fill


// definition for uninitialized_fill_n

template<typename ForwardIterator, typename Size, typename T>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T &x, __false_type)
{
	ForwardIterator cur = first;

	__ML_TRY
	{
		while (n-- > 0)
		{
			constructor(&*cur, x); ++cur;
		}
	}
	__ML_UNWIND(destroy(first, cur));

	return cur;
}

template<typename ForwardIterator, typename Size, typename T>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T &x, __true_type)
{
	return fill_n(first, n, x);
}

template<typename ForwardIterator, typename Size, typename T1, typename T2>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T1 &x, T2*)
{
	typedef typename __type_traits<T2>::is_POD_type is_POD;

	return __uninitialized_fill_n_aux(first, n, x, is_POD());
}

template<typename ForwardIterator, typename Size, typename T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T &x)
{
	return __uninitialized_fill_n(first, n, x, value_type(first));
}


__ML_END_NAMESPACE


#endif //  __ML_INTERNAL_UNINITIALIZED_H
