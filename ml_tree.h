/**
  * @file ml_tree.h
  * @brief 
  * @author malin malinbupt@gmail.com
  * @date 2015年05月12日 星期二 14时13分03秒
  * @version 
  * @note 
  */

#ifndef __ML_INTERNAL_TREE_H
#define __ML_INTERNAL_TREE_H


#include "ml_config.h"
#include "ml_alloc.h"
#include "ml_construct.h"
#include "ml_iterator_base.h"


__ML_BEGIN_NAMESPACE




typedef bool __rb_tree_color_type;
const __rb_tree_color_type __rb_tree_red = false;
const __rb_tree_color_type __rb_tree_black = true;


// base class of the tree's node
struct __rb_tree_node_base
{
	typedef __rb_tree_node_base* base_ptr;
	typedef __rb_tree_color_type color_type;


	static base_ptr minimum(base_ptr r)
	{
		while (0 != r->left) r = r->left;
		return r;
	}

	static base_ptr maximum(base_ptr r)
	{
		while (0 != r->right) r = r->right;
		return r;
	}


	base_ptr left;
	base_ptr right;
	base_ptr parent;
	color_type color;

};

// actual node of tree
template<typename T>
struct __rb_tree_node : public __rb_tree_node_base
{
	T data;

};


//base iterator of tree
struct __rb_tree_iterator_base
{	
	typedef bidirectional_iterator_tag iterator_category;
	typedef ptrdiff_t difference_type;

	typedef __rb_tree_node_base::base_ptr base_ptr;


	explicit __rb_tree_iterator_base(base_ptr x = 0) : node(x) {}
	__rb_tree_iterator_base(const __rb_tree_iterator_base &rhs) : node(rhs.node) {}


	void increment()
	{
		if (0 != node->right)
		{
			node = node->right;
			while (0 != node->left) node = node->left;
		}
		else
		{
			base_ptr parent = node->parent;
			while (node == parent->right)
			{
				node = parent;
				parent = node->parent;
			}

			if (parent != node->right) node = parent;
		}
	}

	void decrement()
	{
		if (node->parent->parent == node && node->color == __rb_tree_red)
		{
			node = node->right;
		}
		else if (0 != node->left)
		{
			node = node->left;
			while (0 != node->right) node = node->right;
		}
		else
		{
			base_ptr parent = node->parent;
			while (node == parent->left)
			{
				node = parent;
				parent = node->parent;
			}

			node = parent;
		}
	}


	base_ptr node;


};


// iterator for rb tree
template<typename T>
struct __rb_tree_iterator : public __rb_tree_iterator_base
{
	typedef T value_type;
	typedef value_type* pointer;
	typedef value_type& reference;

	typedef __rb_tree_node<T>* link_type;
	typedef __rb_tree_iterator<T> self;


	explicit __rb_tree_iterator(link_type x = 0) : __rb_tree_iterator(x) {}
	__rb_tree_iterator(const self &rhs) : __rb_tree_iterator_base(rhs) {}


	reference operator * () const
	{
		return link_type(node)->data;
	}

	pointer operator -> () const
	{
		return &(operator*());
	}

	self& operator ++ ()
	{
		increment();
		return *this;
	}

	self operator ++ (int)
	{
		self tmp = *this;
		++*this;
		return tmp;
	}

	self& operator -- ()
	{
		decrement();
		return *this;
	}

	self operator -- (int)
	{
		self tmp = *this;
		--*this;
		return tmp;
	}


};


template<typename T>
bool operator == (const __rb_tree_iterator<T> &x, const __rb_tree_iterator<T> &y)
{
	return x.node == y.node;
}

template<typename T>
bool operator != (const __rb_tree_iterator<T> &x, const __rb_tree_iterator<T> &y)
{
	return !(x == y);
}




inline void __rb_tree_rotate_left(__rb_tree_node_base *x, __rb_tree_node_base *&root)
{
	__rb_tree_node_base *y = x->right;
	
	x->right = y->left;
	if (0 != x->right) x->right->parent = x;
	
	if (x == root)
		root = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;

	y->parent = x->parent;
	x->parent = y;
	y->left = x;

}

inline void __rb_tree_rotate_right(__rb_tree_node_base *x, __rb_tree_node_base *&root)
{
	__rb_tree_node_base *y = x->left;

	x->left = y->right;
	if (0 != x->left) x->left->parent = x;

	if (x == root)
		y = root;
	else if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;

	y->parent = x->parent;
	x->parent = y;
	y->right = x;
}




inline void __rb_tree_rebalance_for_insert(__rb_tree_node_base *x, __rb_tree_node_base *&root)
{
	x->color = __rb_tree_red;

	while (x != root && x->parent->color == __rb_tree_red)
	{
		__rb_tree_node_base *y = x->parent;

		if (y == y->parent->left)
		{
			if (x == y->right)
			{
				__rb_tree_rotate_left(y, root);
				x = y;
				y = x->parent;
			}

			__rb_tree_node_base *s = y->parent->right;

			if (0 != s && s->color == __rb_tree_red)
			{
				y->color = __rb_tree_black;
				s->color = __rb_tree_black;
				y->parent->color = __rb_tree_red;
				x = y->parent;
			}
			else
			{
				y->color = __rb_tree_black;
				y->parent->color = __rb_tree_red;
				__rb_tree_rotate_right(y->parent, root);
			}
		}
		else
		{
			if (x == y->left)
			{
				__rb_tree_rotate_right(y, root);
				x = y;
				y = x->parent;
			}

			__rb_tree_node_base *s = y->parent->left;

			if (0 != s && s->color == __rb_tree_red)
			{
				y->color = __rb_tree_black;
				s->color = __rb_tree_black;
				y->parent->color = __rb_tree_red;
				x = y->parent;
			}
			else
			{
				y->color = __rb_tree_black;
				y->parent->color = __rb_tree_red;
				__rb_tree_rotate_left(y->parent, root);
			}
		}

		root->color = __rb_tree_black;
	}
}

inline void __rb_tree_rebalance_for_erase(__rb_tree_node_base *x, __rb_tree_node_base *&root)
{
	while (x != root && __rb_tree_black == x->color)
	{
		__rb_tree_node_base *y = x->parent;

		if (x == y->left)
		{
			__rb_tree_node_base *w = y->right;

			if (__rb_tree_red == w->color)
			{
				w->color = __rb_tree_black;
				y->color = __rb_tree_red;
				__rb_tree_rotate_left(y, root);
			}

			if ((0 == w->left || __rb_tree_black == w->left->color) && (0 == w->right || __rb_tree_black == w->right->color))
			{
				w->color = __rb_tree_red;
				x = y;
			}
			else
			{
				if (0 == w->right || __rb_tree_black == w->right->color)
				{
					w->left->color = __rb_tree_black;
					w->color = __rb_tree_red;
					__rb_tree_rotate_right(w, root);
				}
				
				w->color = y->color;
				y->color = __rb_tree_black;
				w->right->color = __rb_tree_black;
				__rb_tree_rotate_left(y, root);

				x = root;
			}
		}
		else
		{
			__rb_tree_node_base *w = y->left;
			
			if (__rb_tree_red == w->color)
			{
				w->color = __rb_tree_black;
				y->color = __rb_tree_red;
				__rb_tree_rotate_right(y, root);
			}

			if ((0 == w->left || __rb_tree_black == w->left->color) && (0 == w->right || __rb_tree_black == w->right->color))
			{
				w->color = __rb_tree_red;
				x = y;
			}
			else
			{
				if (0 == w->left || __rb_tree_black == w->left->color)
				{
					w->color = __rb_tree_red;
					w->right->color = __rb_tree_black;
					__rb_tree_rotate_left(w, root);
				}

				w->color = y->color;
				y->color = __rb_tree_black;
				w->left->color = __rb_tree_black;
				__rb_tree_rotate_right(y, root);

				x = root;
			}
		}

		x->color = __rb_tree_black;
	}
}



// rb tree
template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc = alloc>
class rb_tree
{
public:
	typedef Key key_type;
	typedef Value value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

private:
	typedef __rb_tree_node_base* base_ptr;
	typedef __rb_tree_node<Value> rb_tree_node;
	typedef rb_tree_node* link_type;
	typedef __rb_tree_color_type color_type;

	typedef simple_alloc<rb_tree_node, Alloc> node_allocator;


private:
	link_type get_node()
	{
		return node_allocator::allocate();
	}

	void put_node(link_type p)
	{
		node_allocator::deallocate(p);
	}

	link_type create_node(const_reference x)
	{
		link_type tmp = get_node();

		try
		{
			construct(tmp->data, x);
		}
		catch (...)
		{
			put_node(tmp);
			throw;
		}

		return tmp;
	}

	void destroy_node(link_type p)
	{
		destroy(&(p->data));
		put_node(p);
	}


private:
	link_type header;
	size_type node_count;
	Compare key_compare;

};




__ML_END_NAMESPACE


#endif // __ML_INTERNAL_TREE_H
