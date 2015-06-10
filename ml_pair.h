/**
  * @file ml_pair.h
  * @brief 
  * @author malin malinbupt@gmail.com
  * @date 2015年04月13日 星期一 10时48分03秒
  * @version 
  * @note 
  */

#ifndef __ML_INTERNAL_PAIR_H
#define __ML_INTERNAL_PAIR_H


__ML_BEGIN_NAMESPACE


template<typename T1, typename T2>
struct pair
{
	pair() : first(T1()), second(T2()) {}

	pair(const T1 &a, const T2 &b) : first(a), second(b) {}

	template<typename U1, typename U2>
	pair(const pair<U1, U2> &rhs) : first(rhs.first), second(rhs.second) {}

	typedef T1 first_type;
	typedef T2 second_type;

	T1 first;
	T2 second;
};


template<typename T1, typename T2>
inline bool operator == (const pair<T1, T2> &a, const pair<T1, T2> &b)
{
	return a.first == b.first && a.second == b.second;
}

template<typename T1, typename T2>
inline bool operator != (const pair<T1, T2> &a, const pair<T1, T2> &b)
{
	return !(a == b);
}

template<typename T1, typename T2>
inline bool operator < (const pair<T1, T2> &a, const pair<T1, T2> &b)
{
	return a.first < b.first || !(b.first < a.first) && a.second < b.second;
}

template<typename T1, typename T2>
inline bool operator <= (const pair<T1, T2> &a, const pair<T1, T2> &b)
{
	return !(b < a);
}

template<typename T1, typename T2>
inline bool operator > (const pair<T1, T2> &a, const pair<T1, T2> &b)
{
	return !(a <= b);
}

template<typename T1, typename T2>
inline bool operator >= (const pair<T1, T2> &a, const pair<T1, T2> &b)
{
	return !(a < b);
}

template<typename T1, typename T2>
inline pair<T1, T2> make_pair(const T1 &first, const T2 &second)
{
	return pair<T1, T2>(first, second);
}


__ML_END_NAMESPACE


#endif //__ML_INTERNAL_PAIR_H
