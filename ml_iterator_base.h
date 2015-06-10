/**
  * @file ml_iterator_base.h
  * @brief 
  * @author malin malinbupt@gmail.com
  * @date 2015年04月01日 星期三 11时04分38秒
  * @version 
  * @note 
  */

#ifndef __ML_INTERNAL_ITERATOR_BASE_H
#define __ML_INTERNAL_ITERATOR_BASE_H


#include <cstddef>
#include "ml_config.h"

__ML_BEGIN_NAMESPACE


struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};


template<typename Category, typename T, typename Distance = ptrdiff_t, typename Pointer = T*, typename Reference = T&>
struct iterator
{
	typedef Category  iterator_category;
	typedef T		  value_type;
	typedef Distance  difference_type;
	typedef Pointer   pointer;
	typedef Reference reference;
};


template<typename Iterator>
struct iterator_traits
{
	typedef typename Iterator::iterator_category iterator_category;
	typedef typename Iterator::value_type		 value_type;
	typedef typename Iterator::difference_type   difference_type;
	typedef typename Iterator::pointer			 pointer;
	typedef typename Iterator::reference		 reference;

};

template<typename T>
struct iterator_traits<T*>
{
	typedef random_access_iterator_tag  iterator_category;
	typedef ptrdiff_t					difference_type;
	typedef T							value_type;
	typedef T*							pointer;
	typedef T&							reference;

};

template<typename T>
struct iterator_traits<const T*>
{
	typedef random_access_iterator_tag  iterator_category;
	typedef ptrdiff_t					difference_type;
	typedef T							value_type;
	typedef const T*					pointer;
	typedef const T&					reference;

};


template<typename Iterator>
inline typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&)
{
	typedef typename iterator_traits<Iterator>::iterator_category category;

	return category();
}

template<typename Iterator>
inline typename iterator_traits<Iterator>::value_type*
value_type(const Iterator&)
{
	return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

template<typename Iterator>
inline typename iterator_traits<Iterator>::difference_type*
distance_type(const Iterator&)
{
	return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}


template<typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
__distance(InputIterator first, InputIterator last, input_iterator_tag)
{
	typename iterator_traits<InputIterator>::difference_type n = 0;

	while (first != last) {++first; ++n;}

	return n;
}

template<typename RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type
__distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag)
{
	return last - first;
}

template<typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last)
{
	return __distance(first, last, iterator_category(first));
}


template<typename InputIterator, typename Distance>
inline void __advance(InputIterator &i, Distance n, input_iterator_tag)
{
	while (n--) ++i;
}

template<typename BidirectionalIterator, typename Distance>
inline void __advance(BidirectionalIterator &i, Distance n, bidirectional_iterator_tag)
{
	if (n >= 0)
	{
		while (n--) ++i;
	}
	else
	{
		while (n++) --i;
	}
}

template<typename RandomAccessIterator, typename Distance>
inline void __advance(RandomAccessIterator &i, Distance n, random_access_iterator_tag)
{
	i += n;
}

template<typename InputIterator, typename Distance>
inline void advance(InputIterator &i, Distance n)
{
	__advance(i, n, iterator_category(i));
}


__ML_END_NAMESPACE


#endif // __ML_INTERNAL_ITERATOR_BASE_H
