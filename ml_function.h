/**
  * @file ml_function.h
  * @brief 
  * @author malin malinbupt@gmail.com
  * @date 2015年05月11日 星期一 11时10分43秒
  * @version 
  * @note 
  */

#ifndef __FUNCTION_INTERNAL_H
#define __FUNCTION_INTERNAL_H


#include "ml_config.h"


__ML_BEGIN_NAMESPACE;


template <class _Arg1, class _Arg2, class _Result>
struct binary_function
{
  typedef _Arg1 first_argument_type;
  typedef _Arg2 second_argument_type;
  typedef _Result result_type;
};


template <class _Tp>
struct less : public binary_function<_Tp,_Tp,bool> 
{
  bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};


__ML_END_NAMESPACE;


#endif // __FUNCTION_INTERNAL_H
