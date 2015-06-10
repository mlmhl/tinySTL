/**
  * @file stl_alloc.h
  * @brief 
  * @author malin malinbupt@gmail.com
  * @date 2015年03月30日 星期一 14时43分30秒
  * @version 
  * @note 
  */

#ifndef __ML_ALLOC_H
#define __ML_ALLOC_H

#include <cstdlib>
#include <cstddef>
#include <cstring>

#include <new>

#include "ml_config.h"
#include "ml_exception.h"

#define __instHROW_BAD_ALLOC throw bad_alloc("no memory!")


__ML_BEGIN_NAMESPACE

template<int inst>
class __malloc_alloc_template
{
private:
	static void *oom_malloc(size_t);
	static void *oom_realloc(void*, size_t);

	static void (*__malloc_alloc_oom_handler) ();

public:
	static void *allocate(size_t n)
	{
		void *res = malloc(n);

		if (res == 0) res = oom_malloc(n);

		return res;
	}

	static void deallocate(void *p, size_t /*n*/)
	{
		free(p);
	}

	static void *reallocate(void *p, size_t /* old_sz*/, size_t n_sz)
	{
		void *res = realloc(p, n_sz);

		if (res == 0) res = oom_realloc(p, n_sz);

		return res;
	}

	static void (*set_malloc_handler(void (*f)())) ()
	{
		void (*old)() = __malloc_alloc_oom_handler;

		__malloc_alloc_oom_handler = f;

		return old;
	}

};

template<int inst>
void (*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;

template<int inst>
void* __malloc_alloc_template<inst>::oom_malloc(size_t n)
{
	void (*my_malloc_handler)();

	void *res = 0;

	while (!res)
	{
		my_malloc_handler = __malloc_alloc_oom_handler;

		if (my_malloc_handler == 0) __instHROW_BAD_ALLOC;

		my_malloc_handler();

		res = malloc(n);
	}

	return res;
}

template<int inst>
void * __malloc_alloc_template<inst>::oom_realloc(void *p, size_t n)
{
	void (*my_malloc_handler)(); void *res = 0;

	while (!res)
	{
		my_malloc_handler = __malloc_alloc_oom_handler;

		if (0 == my_malloc_handler) __instHROW_BAD_ALLOC;

		res = realloc(p, n);
	}

	return res;
}

typedef __malloc_alloc_template<0> malloc_alloc;



const size_t __ALIGN = 8, __MAX_BYTES = 128, __NFREELISTS = __MAX_BYTES / __ALIGN;

template<bool thread, int inst>
class __default_alloc_template
{
private:
	static size_t bound_up(size_t bytes)
	{
		return (bytes + __ALIGN - 1) & ~(__ALIGN - 1);
	}

	static size_t freelist_index(size_t bytes)
	{
		return (bytes + __ALIGN - 1) / __ALIGN - 1;
	}

private:
	union obj
	{
		obj *free_list_link;
		char client_data[1];
	};

	static obj* volatile free_list[__NFREELISTS];

private:
	static void *refill(size_t n);

	static char *chunk_alloc(size_t size, int &nobjs);

	static char *start_free;
	static char *end_free;
	static size_t heap_size;

public:
	static void *allocate(size_t n);

	static void deallocate(void *p, size_t n);

	static void *reallocate(void *p, size_t old_sz, size_t new_sz);

};


template<bool thread, int inst> char* __default_alloc_template<thread, inst>::start_free = 0;
template<bool thread, int inst> char* __default_alloc_template<thread, inst>::end_free = 0;
template<bool thread, int inst> size_t __default_alloc_template<thread, inst>::heap_size = 0;

template<bool thread, int inst> typename __default_alloc_template<thread, inst>::obj* volatile
__default_alloc_template<thread, inst>::free_list[__NFREELISTS] = {0};

template<bool thread, int inst>
void* __default_alloc_template<thread, inst>::allocate(size_t n)
{
	if (n > 128) return malloc_alloc::allocate(n);

	obj* volatile *my_free_list = free_list + freelist_index(n); obj *result = *my_free_list;

	if (result == 0)
	{
		return refill(bound_up(n));
	}
	else *my_free_list = result->free_list_link;

	return result;
}

template<bool thread, int inst>
void __default_alloc_template<thread, inst>::deallocate(void *p, size_t n)
{
	if (n > 128) malloc_alloc::allocate(n);

	else
	{
		obj* q = (obj*)p, *volatile *my_free_list = free_list + freelist_index(n);

		q->free_list_link = *my_free_list; *my_free_list = q;
	}
}

template<bool thread, int inst>
void* __default_alloc_template<thread, inst>::reallocate(void *p, size_t old_sz, size_t new_sz)
{
	if (old_sz > __MAX_BYTES && new_sz > __MAX_BYTES) return realloc(p, new_sz);

	if (bound_up(old_sz) == bound_up(new_sz)) return p;

	void *result = allocate(new_sz); size_t copy_sz = old_sz > new_sz ? new_sz : old_sz;

	memcpy(result, p, copy_sz); deallocate(p, old_sz);

	return result;
}

template<bool thread, int inst>
void* __default_alloc_template<thread, inst>::refill(size_t n)
{
	int nobjs = 20; char *chunk = chunk_alloc(n, nobjs);

	if (nobjs == 1) return chunk;

	obj *result = (obj*)chunk, *cur_obj, *next_obj, *volatile *my_free_list = free_list + freelist_index(n);

	*my_free_list = next_obj = (obj*)(chunk + n);

	for (int i = 1; i < nobjs - 1; ++i)
	{
		cur_obj = next_obj; next_obj = (obj*)((char*)next_obj + n);

		cur_obj->free_list_link = next_obj;
	}

	next_obj->free_list_link = 0;

	return result;
}

template<bool thread, int inst>
char* __default_alloc_template<thread, inst>::chunk_alloc(size_t size, int &nobjs)
{
	size_t total_bytes = size * nobjs, bytes_left = end_free - start_free;

	char *result;

	if (bytes_left >= total_bytes)
	{
		result = start_free; start_free += total_bytes;

		return result;
	}
	else if (bytes_left >= size)
	{
		nobjs = bytes_left / size;
		
		result = start_free; start_free += nobjs * size;
		
		return result;
	}
	else
	{
		if (bytes_left > 0)
		{
			obj* volatile *my_free_list = free_list + freelist_index(bytes_left);

			((obj*)start_free)->free_list_link = *my_free_list;

			*my_free_list = (obj*)start_free;
		}

		size_t bytes_to_get = 2 * total_bytes + (heap_size >> 4);

		start_free = (char*)malloc(bytes_to_get);

		if (start_free == 0)
		{
			obj *volatile *my_free_list, *p;

			for (size_t i = size; i <= __MAX_BYTES; i += __ALIGN)
			{
				my_free_list = free_list + i / __ALIGN; p = *my_free_list;

				if (0 != p)
				{
					start_free = (char*)p; end_free = start_free + i; *my_free_list = p->free_list_link;

					return chunk_alloc(size, nobjs);
				}
			}

			end_free = 0;

			start_free = (char*)malloc_alloc::allocate(bytes_to_get);
		}

		end_free = start_free + bytes_to_get; heap_size += bytes_to_get;

		return chunk_alloc(size, nobjs);
	}
}

typedef __default_alloc_template<0, 0> alloc;


template<typename T, typename Alloc>
class simple_alloc
{
public:
	typedef T		  value_type;
	typedef T*		  pointer;
	typedef const T*  const_pointer;
	typedef T&		  reference;
	typedef const T&  const_reference;
	typedef size_t	  size_type;
	typedef ptrdiff_t diffrence_type;

	static T *allocate(size_t n)
	{
		return 0 == n ? 0 : (T*)Alloc::allocate(n * sizeof(T));
	}

	static T *allocate()
	{
		return (T*)Alloc::allocate(sizeof(T));
	}

	static void deallocate(T *p, size_t n)
	{
		if (0 != n) Alloc::deallocate(p, sizeof(T) * n);
	}

	static void deallocate(T *p)
	{
		Alloc::deallocate(p, sizeof(T));
	}

	static pointer address(reference x)
	{
		return &x;
	}

};


__ML_END_NAMESPACE

#endif // __ML_ALLOC_H
