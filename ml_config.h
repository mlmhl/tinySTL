/**
  * @file stl_config.h
  * @brief 
  * @author malin malinbupt@gmail.com
  * @date 2015年03月30日 星期一 14时51分24秒
  * @version 
  * @note 
  */

#ifndef __ML_CONFIG_H
#define __ML_CONFIG_H

#define __ML_BEGIN_NAMESPACE namespace ml {
#define __ML_END_NAMESPACE }

#define __ML_NOTHROW throw()

#define __ML_TEMPLATE_NULL template<>

#define __ML_TRY try
#define __ML_UNWIND(action) catch(...) {action; throw;}


#endif // __ML_CONFIG_H
