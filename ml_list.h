/**
  * @file ml_list.h
  * @brief 
  * @author malin malinbupt@gmail.com
  * @date 2015年04月28日 星期二 11时30分29秒
  * @version 
  * @note 
  */

#ifndef	__LIST_INTERNAL_H
#define __LIST_INTERNAL_H


#include "ml_alloc.h"
#include "ml_config.h"
#include "ml_exception.h"
#include "ml_construct.h"
#include "ml_algobase.h"
#include "ml_iterator_base.h"


__ML_BEGIN_NAMESPACE


/**
  * @brief list's node
  *
  */
template<typename T>
struct __list_node
{

	T data;
	__list_node *prev;
	__list_node *next;

};


/**
  * @brief list's iterator
  *
  */
template<typename T>
struct __list_iterator
{
	typedef __list_node<T> Node;
	typedef __list_iterator self;

	typedef bidirectional_iterator_tag  iterator_category;
	typedef T							value_type;
	typedef value_type*					pointer;
	typedef value_type&					reference;
	typedef ptrdiff_t					difference_type;

	
	// Deliberately do not use explicit,support for implicit conversion from node to iterator 
	__list_iterator(Node *n = 0) : node(n) {}

	reference operator * () const
	{
		return node->data;
	}

	pointer operator -> () const
	{
		return &(operator*());
	}

	self& operator ++ ()
	{
		node = node->next;
		return *this;
	}

	self operator ++ (int)
	{
		self tmp = *this;
		++tmp;
		return tmp;
	}

	self& operator -- ()
	{
		node = node->prev;
		return *this;
	}

	self operator -- (int)
	{
		self tmp = *this;
		--tmp;
		return tmp;
	}


	Node *node; // the list node pointer to

};


// relational operator for __list_iterator
template<typename T>
bool operator == (const __list_iterator<T> &lhs, const __list_iterator<T> &rhs)
{
	return lhs.node == rhs.node;
}

template<typename T>
bool operator != (const __list_iterator<T> &lhs, const __list_iterator<T> &rhs)
{
	return !(lhs == rhs);
}



/**
  * @brief list container
  *
  */
template<typename T, typename Alloc = alloc>
class list
{
public:
	typedef T				  value_type;
	typedef value_type*		  pointer;
	typedef value_type&		  reference;
	typedef const value_type* const_pointer;
	typedef const value_type& const_reference;
	typedef size_t			  size_type;
	typedef ptrdiff_t		  difference_type;

	typedef __list_iterator<T> iterator;

private:
	typedef __list_node<T> list_node;
	typedef simple_alloc<list_node, Alloc> node_allocator;

public:
	// construct
	explicit list(const Alloc &a = Alloc()) : node(get_node())
	{
		node->prev = node->next = node;
	}

	explicit list(size_type n, const_reference x = value_type(), const Alloc &a = Alloc())
	{
		node = get_node();
		node->prev = node->next = node;
		insert(node, n, x);
	}

	template<typename InputIterator>
	list(InputIterator first, InputIterator last, const Alloc &a = Alloc())
	{
		node = get_node();
		node->prev = node->next = node;
		insert(node, first, last);
	}




	// copy
	list(const list &rhs)
	{
		node = get_node();
		node->prev = node->next = node;
		insert(node, rhs.begin(), rhs.end());	
	}

	list& operator = (const list &rhs)
	{
		if (this != &rhs)
		{
			clear(); insert(begin(), rhs.begin(), rhs.end());
		}

		return *this;
	}




	// destruct
	~list()
	{
		clear(); put_node(node);
	}




	// unmutable interface
	iterator begin() const
	{
		return node->next;
	}

	iterator end() const
	{
		return node;
	}

	size_type size() const
	{
		list_node *cur = node->next; size_type n = 0;

		while (cur != node)
		{
			cur = cur->next;
			++n;
		}

		return n;
	}

	bool empty() const
	{
		return node->next == node;
	}

	const_reference front() const
	{
		return node->next->data;
	}

	const_reference back() const
	{
		return node->prev->data;
	}

	


	// mutable interface
	reference front()
	{
		return node->data->data;
	}

	reference back()
	{
		return node->prev->data;
	}

	void push_back(const_reference x)
	{
		insert(end(), x);
	}

	void pop_back()
	{
		erase(--end());
	}

	void push_front(const_reference &x)
	{
		insert(begin(), x);
	}

	void pop_front()
	{
		erase(begin());
	}

	iterator insert(iterator position, const_reference x)
	{
		list_node *new_node = get_node(), *cur = position.node;;
		construct(&(new_node->data), x);

		new_node->prev = cur->prev;
		new_node->prev->next = new_node;
		new_node->next = cur;
		cur->prev = new_node;

		return new_node;
	}

	void insert(iterator position, size_type n, const value_type &x)
	{
		list_node *cur = position.node, *tmp = position.node->next;

		while (n-- > 0)
		{
			cur->next = get_node();
			cur->next->prev = cur;
			cur = cur->next;
			construct(&(cur->data), x);
		}

		cur->next = tmp; tmp->prev = cur;
	}

	template<typename InputIterator>
	void insert(iterator position, InputIterator first, InputIterator last)
	{
		list_node *cur = position.node, *tmp = position.node->next;

		while (first != last)
		{
			cur->next = get_node();
			cur->next->prev = cur;
			cur = cur->next;
			construct(&(cur->data), *first);
			++first;
		}

		cur->next = tmp; tmp->prev = cur;
	}

	iterator erase(iterator position)
	{
		if (empty()) throw underflow_error("the list is empty!");

		list_node *node_to_delete = position.node, *result = node_to_delete->next;
		
		result->prev = node_to_delete->prev;
		node_to_delete->prev->next = result;

		destroy(&(node_to_delete->data));
		put_node(node_to_delete);

		return result;
	}

	iterator erase(iterator first, iterator last)
	{
		list_node *prev = first.node->prev, *next = last.node;
		iterator cur;

		while (first != last)
		{
			cur = first;
			++first;
			destroy(&(cur.node->data));
			put_node(cur.node);
		}

		prev->next = last.node; last.node->prev = prev;

		return last;
	}

	void clear()
	{
		list_node *cur = node->next;
		
		while (cur != node)
		{
			destroy(&(cur->data));
			put_node(cur);
			cur = cur->next;
		}

		node->next = node; node->prev = node;
	}

	void assign(size_type n, const_reference x)
	{
		list_node *cur = node->next;
		
		while (cur != node && n-- > 0)
		{
			cur->data = x;
			cur = cur->next;
		}

		if (n > 0) insert(end(), n, x);
		else erase(cur, end());
	}

	template<typename InputIterator>
	void assign(InputIterator first, InputIterator last)
	{
		list_node *cur = node->next;

		for (; first != last && cur != node; cur = cur->next, ++first)
		{
			cur->data = *first;
		}

		if (first != last) insert(end(), first, last);
		else erase(cur, end());
	}

	void resize(size_type n, const_reference x)
	{
		list_node *cur = node->next;

		for (; cur != node && n > 0; cur = cur->next, --n);

		if (n == 0) erase(cur, end());
		else insert(end(), n, x);
	}

	void swap(list &x)
	{
		ml::swap(node, x.node);
	}




	// algorithms
	
	void splice(iterator position, list &x)
	{
		// if x is empty, its soilder node will be inserted into current list
		if (!x.empty())
		{
			transfer(position, x.begin(), x.end());
		}
	}

	void splice(iterator position, list&, iterator i)
	{
		iterator j = i;
		++j;
		if (position == i || position == j) return;
		transfer(position, i, j);
	}

	void splice(iterator position, list&, iterator first, iterator last)
	{
		transfer(position, first, last);
	}

	void remove(const_reference x)
	{
		list_node *cur = node->next;

		while (cur != node)
		{
			if (cur->data == x) cur = erase(cur).node;
			else cur = cur->next;
		}
	}

	template<typename Predicate>
	void remove_if(Predicate pred)
	{
		list_node *cur = node->next;

		while (cur != node)
		{
			if (pred(cur->data)) cur = erase(cur).node;
		}
	}

	void unique()
	{
		list_node *slow = node->next, *fast = slow->next;

		while (fast != node)
		{
			if (slow->data == fast->data) fast = erase(fast).node;
			else {slow = fast; fast = fast->next;}
		}
	}

	template<typename BinaryPredicate>
	void unique(BinaryPredicate pred)
	{
		list_node *slow = node->next, *fast = slow->next;

		while (fast != node)
		{
			if (pred(slow->data, fast->data)) fast = erase(fast).node;
			else {slow = fast; fast = fast->next;}
		}
	}

	void merge(list &x)
	{
		list_node *first = node->next, *second = x.node->next;

		while (first != node && second != x.node)
		{
			list_node *tmp = second;
			while (tmp != x.node && tmp->data < first->data) tmp = tmp->next;
			transfer(first, second, tmp);
			first = first->next;
			second = tmp;
		}

		if (second != x.node) transfer(end(), iterator(second), x.end());
	}

	template<typename Compare>
	void merge(list &x, Compare comp)
	{
		list_node *first = node->next, *second = x.node->next;

		while (first != node && second != x.node)
		{
			list_node *tmp = second;
			while (tmp != x.node && comp(tmp->data, first->data)) tmp = tmp->next;
			transfer(first, second, tmp);
			first = first->next;
			second = tmp;
		}

		if (second != x.node) transfer(end(), iterator(second), x.end());
	}

	void sort()
	{
		list tmp, buf[64]; int cnt = 0;

		while (!empty())
		{
			tmp.splice(tmp.begin(), *this, begin());

			int i = 0;
			for(; i < cnt && !buf[i].empty(); ++i)
			{
				tmp.merge(buf[i]);
			}

			buf[i].swap(tmp);
			if (i >= cnt) ++cnt;
		}

		for (int i = 0; i < cnt; ++i) merge(buf[i]);
	}

	template<typename Compare>
	void sort(Compare comp)
	{
		list tmp, buf[64]; int cnt = 0;

		while (!empty())
		{
			tmp.splice(tmp.begin(), *this, begin());

			int i = 0;
			for(; i < cnt && !buf[i].empty(); ++i)
			{
				tmp.merge(buf[i], comp);
			}

			buf[i].swap(tmp);
			if (i >= cnt) ++cnt;
		}

		for (int i = 0; i < cnt; ++i) merge(buf[i], comp);
	}

	void reverse()
	{
		if (node->next == node || node->next->next == node) return;

		list_node *cur = node, *next = node->next;

		while (next != node)
		{
			list_node *tmp = next->next;
			next->next = cur;
			cur->prev = next;
			cur = next;
			next = tmp;
		}
		next->next = cur;
		cur->prev = next;
	}




private:
	list_node *get_node()
	{
		return node_allocator::allocate(1);
	}

	void put_node(list_node *p)
	{
		destroy(&(p->data)); node_allocator::deallocate(p, 1);
	}

	void transfer(iterator position, iterator first, iterator last)
	{
		if (position == last || first == last) return;

		first.node->prev->next = last.node;
		last.node->prev->next = position.node;
		list_node *tmp = position.node->prev;
		tmp->next = first.node;
		position.node->prev = last.node->prev;
		last.node->prev = first.node->prev;
		first.node->prev = tmp;
	}


private:
	list_node *node;

};



// global swap
template<typename T, typename Alloc = alloc>
void swap(list<T, Alloc> &x, list<T, Alloc> &y)
{
	x.swap(y);
}



__ML_END_NAMESPACE


#endif // __LIST_INTERNAL_H
