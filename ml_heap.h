/**
  * @file ml_heap.h
  * @brief 
  * @author malin malinbupt@gmail.com
  * @date 2015年05月10日 星期日 11时12分49秒
  * @version 
  * @note 
  */

#ifndef __HEAP_INTERNAL_H
#define __HEAP_INTERNAL_H


#include "ml_config.h"
#include "ml_iterator_base.h"


__ML_BEGIN_NAMESPACE



// swim operation
template<typename RandomAccessIterator, typename Distance, typename T>
void __push_heap(RandomAccessIterator first, Distance top, Distance holdIndex, T value)
{
	Distance parent = (holdIndex - 1) / 2;

	while (holdIndex > top && *(first + parent) < value)
	{
		*(first + holdIndex) = *(first + parent);
		holdIndex = parent;
		parent = (holdIndex - 1) / 2;
	}

	*(first + holdIndex) = value;
}

template<typename RandomAccessIterator, typename Distance, typename T>
inline void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*)
{
	Distance holdIndex = (last - first) - 1;
	__push_heap(first, 0, holdIndex, *(last - 1));
}

template<typename RandomAccessIterator>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last)
{
	__push_heap_aux(first, last, distance_type(first), value_type(first));
}




// swim operation
template<typename RandomAccessIterator, typename Compare, typename Distance, typename T>
void __push_heap(RandomAccessIterator first, Distance top, Distance holdIndex, T value, Compare comp)
{
	Distance parent = (holdIndex - 1) / 2;

	while (holdIndex > top && comp(*(first + parent), value))
	{
		*(first + holdIndex) = *(first + parent);
		holdIndex = parent;
		parent = (holdIndex - 1) / 2;
	}

	*(first + holdIndex) = value;
}

template<typename RandomAccessIterator, typename Compare, typename Distance, typename T>
inline void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Compare comp, Distance*, T*)
{
	Distance holdIndex = (last - first) - 1;
	__push_heap(first, 0, holdIndex, *(last - 1), comp);
}

template<typename RandomAccessIterator, typename Compare>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
	__push_heap_aux(first, last, comp, distance_type(first), value_type(first));
}




// sink operation
template<typename RandomAccessIterator, typename Distance, typename T>
void __pop_heap(RandomAccessIterator first, Distance holdIndex, Distance len, const T &value)
{
	Distance rightchild = 2 * holdIndex + 2;

	while (rightchild < len)
	{
		if (*(first + rightchild) < *(first + (rightchild - 1))) --rightchild;
		if (value < *(first + rightchild))
		{
			*(first + holdIndex) = *(first + rightchild);
			holdIndex = rightchild;
			rightchild = 2 * holdIndex + 2;
		}
		else break;
	}

	if (rightchild == len)
	{
		--rightchild;
		if (value < *(first + rightchild))
		{
			*(first + holdIndex) = *(first + rightchild);
			holdIndex = rightchild;
		}
	}

	*(first + holdIndex) = value;
}

template<typename RandomAccessIterator, typename Distance, typename T>
inline void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*)
{
	T value = *(last - 1);
	*(last - 1) = *first;
	Distance len = last - first - 1;

	__pop_heap(first, 0, len, value);
}

template<typename RandomAccessIterator>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last)
{
	__pop_heap_aux(first, last, distance_type(first), value_type(first));
}




// sink operation
template<typename RandomAccessIterator, typename Compare, typename Distance, typename T>
void __pop_heap(RandomAccessIterator first, Distance holdIndex, Distance len, const T &value, Compare comp)
{
	Distance rightchild = 2 * holdIndex + 2;

	while (rightchild < len)
	{
		if (comp(*(first + rightchild), *(first + (rightchild - 1)))) --rightchild;
		if (comp(value, *(first + rightchild)))
		{
			*(first + holdIndex) = *(first + rightchild);
			holdIndex = rightchild;
			rightchild = 2 * holdIndex + 2;
		}
		else break;
	}

	if (rightchild == len)
	{
		--rightchild;
		if (comp(value, *(first + rightchild)))
		{
			*(first + holdIndex) = *(first + rightchild);
			holdIndex = rightchild;
		}
	}

	*(first + holdIndex) = value;
}

template<typename RandomAccessIterator, typename Compare, typename Distance, typename T>
inline void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Compare comp, Distance*, T*)
{
	T value = *(last - 1);
	*(last - 1) = *first;
	Distance len = last - first - 1;

	__pop_heap(first, 0, len, value, comp);
}

template<typename RandomAccessIterator, typename Compare>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
	__pop_heap_aux(first, last, comp, distance_type(first), value_type(first));
}




// the input interval must be a heap
template<typename RandomAccessIterator>
inline void sort_heap(RandomAccessIterator first, RandomAccessIterator last)
{
	while (last - first > 1)
		pop_heap(first, last--);
}

// the input interval must be a heap
template<typename RandomAccessIterator, typename Compare>
inline void sort_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
	while (last - first > 1)
		pop_heap(first, last--, comp);
}




template<typename RandomAccessIterator, typename Distance, typename T>
void __make_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*)
{
	Distance len = last - first;
	if (len < 2) return;

	Distance holdIndex = (len - 2) / 2;
	while (true)
	{
		T value = *(first + holdIndex);
		__pop_heap(first, holdIndex, len, value);
		
		if (holdIndex == 0) break;
		--holdIndex;
	}
}

template<typename RandomAccessIterator>
inline void make_heap(RandomAccessIterator first, RandomAccessIterator last)
{
	__make_heap_aux(first, last, distance_type(first), value_type(first));
}

template<typename RandomAccessIterator, typename Compare, typename Distance, typename T>
void __make_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Compare comp, Distance*, T*)
{
	Distance len = last - first;
	if (len < 2) return;

	Distance holdIndex = (len - 2) / 2;
	while (true)
	{
		T value = *(first + holdIndex);
		__pop_heap(first, holdIndex, len, value, comp);
		
		if (holdIndex == 0) break;
		--holdIndex;
	}
}

template<typename RandomAccessIterator, typename Compare>
inline void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
	__make_heap_aux(first, last, comp, distance_type(first), value_type(first));
}




__ML_END_NAMESPACE



#endif // __HEAP_INTERNAL_H
