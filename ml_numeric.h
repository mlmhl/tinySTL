/**
  * @file ml_numeric.h
  * @brief 
  * @author malin malinbupt@gmail.com
  * @date 2015年04月12日 星期日 14时30分54秒
  * @version 
  * @note 
  */

#ifndef __ML_INTERNAL_NUMERIC_H
#define __ML_INTERNAL_NUMERIC_H


#include "ml_config.h"
#include "ml_iterator_base.h"

__ML_BEGIN_NAMESPACE


template<typename InputIterator, typename T>
T accumulate(InputIterator first, InputIterator last, T init)
{
	for (; first != last; ++first) init = init + *first;

	return init;
}

template<typename InputIterator, typename T, typename BinaryOperation>
T accumulate(InputIterator first, InputIterator last, T init, BinaryOperation op)
{
	for (; first != last; ++first) init = op(init, *first);

	return init;
}


template<typename InputIterator1, typename InputIterator2, typename T>
T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init)
{
	for (; first1 != last1; ++first1, ++first2) init = init + *first1 * *first2;

	return init;
}

template<typename InputIterator1, typename InputIterator2, typename T, typename BinaryOperation1, typename BinaryOperation2>
T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init, BinaryOperation1 op1,
		BinaryOperation2 op2)
{
	for (; first1 != last1; ++first1, ++first2) init = op1(init, op2(*first1, *first2));

	return init;
}


template<typename InputIterator, typename OutputIterator>
OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator dest)
{
	if (first == last) return dest;

	typename iterator_traits<InputIterator>::value_type value = *first;
	
	*dest = *first;

	while (++first != last)
	{
		value = value + *first; *++dest = value; 
	}

	return ++dest;
}

template<typename InputIterator, typename OutputIterator, typename BinaryOperation>
OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator dest, BinaryOperation op)
{
	if (first == last) return dest;

	typename iterator_traits<InputIterator>::value_type value = *first;

	*dest = *first;

	while (++ first != last)
	{
		value = op(value, *first); *++dest = value;
	}

	return ++dest;
}


template<typename InputIterator, typename OutputIterator>
OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator dest)
{
	if (first == last) return dest;

	typedef typename iterator_traits<InputIterator>::value_type value_type;

	value_type value = *first; *dest = *first;

	while (++first != last)
	{
		value_type tmp = *first; *++dest = tmp - value; value = tmp;
	}

	return ++dest;
}

template<typename InputIterator, typename OutputIterator, typename BinaryOperation>
OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator dest, BinaryOperation op)
{
	if (first == last) return dest;

	typedef typename iterator_traits<InputIterator>::value_type value_type;

	value_type value = *first; *dest = *first;

	while (++first != last)
	{
		value_type tmp = *first; *++dest = op(tmp, value); value = tmp;
	}

	return ++dest;
}


template<typename ForwardIterator, typename T>
void iota(ForwardIterator first, ForwardIterator last, T value)
{
	for (; first != last; ++first) *first = value++;
}


__ML_END_NAMESPACE


#endif // __ML_INTERNAL_NUMERIC_H
