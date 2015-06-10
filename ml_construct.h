/**
  * @file ml_construct.h
  * @brief 
  * @author malin malinbupt@gmail.com
  * @date 2015年04月02日 星期四 11时23分41秒
  * @version 
  * @note 
  */

#ifndef __ML_INTERNAL_CONSTRUCT_H
#define __ML_INTERNAL_CONSTRUCT_H


#include "ml_config.h"
#include "type_traits.h"
#include "ml_iterator_base.h"
#include <new>


__ML_BEGIN_NAMESPACE



template<typename T1, typename T2>
inline void construct(T1 *p, const T2 &val)
{
	new(p) T1(val);
}

template<typename T>
inline void destroy(T *p)
{
	p->~T();
}

template<typename ForwardIterator>
inline void __destroy_aux(ForwardIterator &first, ForwardIterator &last, __false_type)
{
	while (first != last)
	{
		destroy(&*first); ++first;
	}
}

template<typename ForwardIterator>
inline void __destroy_aux(ForwardIterator &first, ForwardIterator &last, __true_type) {}

template<typename ForwardIterator, typename T>
inline void __destroy(ForwardIterator &first, ForwardIterator &last, T*)
{
	__destroy_aux(first, last, typename __type_traits<T>::has_trivial_destructor());
}

template<typename ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last)
{
	__destroy(first, last, value_type(first));
}


inline void destroy(char*, char*) {}
inline void destroy(int*, int*) {}
inline void destroy(long*, long*) {}
inline void destroy(float*, float*) {}
inline void destroy(double*, double*) {}


__ML_END_NAMESPACE


#endif // __ML_INTERNAL_CONSTRUCT_H 
