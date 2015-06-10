/**
  * @file ml_exception.h
  * @brief 
  * @author malin malinbupt@gmail.com
  * @date 2015年04月24日 星期五 16时20分54秒
  * @version 
  * @note 
  */

#ifndef __ML_INTERNAL_EXCEPTION
#define __ML_INTERNAL_EXCEPTION

#include "ml_config.h"


__ML_BEGIN_NAMESPACE


// root class for exceptions
class exception
{
public:
	virtual const char *what() const
	{
		return "";
	}

	virtual ~exception() {}

};



// logic_error exceptions

class logic_error : public exception
{
public:
	logic_error(const char *str) : Message(str) {}

	const char *what() const
	{
		return Message;
	}

private:
	const char *Message;

};

class out_of_range : public logic_error
{
public:
	out_of_range(const char *str) : logic_error(str) {}

};



// runtime_error exceptions

class runtime_error : public exception
{
public:
	runtime_error(const char *str) : Message(str) {}

	const char *what() const
	{
		return Message;
	}

private:
	const char *Message;

};

class underflow_error : public runtime_error
{
public:
	underflow_error(const char *str) : runtime_error(str) {}

};



// bad_alloc

class bad_alloc : public exception
{
public:
	bad_alloc(const char *str) : Message(str) {}

private:
	const char *Message;

};


__ML_END_NAMESPACE



#endif //  __ML_INTERNAL_EXCEPTION
