/**
  * @file deque_test.cpp
  * @brief 
  * @author malin malinbupt@gmail.com
  * @date 2015年04月30日 星期四 11时11分35秒
  * @version 
  * @note 
  */

#include <iostream>
#include "../ml_deque.h"
#include "deque_test.h"


using namespace std; using namespace ml;


void print(const deque<int> &a)
{
	for (int i = 0; i < a.size(); ++i)
		cout << a[i] << " ";
	cout << endl;
}


void deque_test()
{
	// test for construct
	{
		cout << "----------test for construct----------" << endl;

		deque<int> a; deque<int> b((unsigned int)5, 1);
		int C[] = {1, 2, 3, 4, 5}; deque<int> c(C, C + sizeof(C) / sizeof(int));
		print(b);
		print(c);
	}

	// test for copy construct
	{
		cout << "----------test for copy construct----------" << endl;

		deque<int> a((unsigned int)5, 1), b(a), c;
		c = a;
		print(a);
		print(b);
		print(c);
	}

	// test for add element
	{
		cout << "----------test for add element---------" << endl;

		deque<int> a; int n = 5;
		for (int i = 0; i < n; ++i) a.push_back(i);
		for (int i = 0; i < n; ++i) a.push_front(i);
		print(a);
		
		deque<int>::iterator itr = a.begin() + 5;
		itr = a.insert(itr, 5);
		cout << *itr << endl;
		print(a);
		++itr;
		int A[] = {6, 7, 8}; itr = a.insert(itr, A, A + sizeof(A) / sizeof(int));
		cout << *itr << endl;
		print(a);
		itr = a.insert(itr, (unsigned int)3, 9);
		cout << (itr - a.begin()) << endl;
		print(a);
	}

	// test for unmutable interface
	{
		cout << "----------test for unmutable interface---------" << endl;

		deque<int> a; int n = 5;

		for (int i = 0; i < n; ++i) a.push_back(i);

		cout << "size: " << a.size() << endl;
		cout << "front: " << a.front() << endl;
		cout << "back: " << a.back() << endl;
		cout << "empty: " << boolalpha << a.empty() << endl;
	}

	// test for delete element
	{
		cout << "----------test for delete element---------" << endl;

		int A[] = {1, 2, 3, 4, 5, 6, 7, 8, 9}; deque<int> a(A, A + sizeof(A) / sizeof(int));

		print(a); a.pop_back(); print(a);

		deque<int>::iterator itr = a.begin() + 5; itr = a.erase(itr);
		cout << *itr << endl; print(a);

		a.erase(a.begin() + 1, a.begin() + 5); print(a);

		a.pop_front(); print(a);
	}

	// test for swap
	{
		cout << "----------test for swap---------" << endl;

		int A[] = {1, 2, 3, 4, 5}, B[] = {7, 8, 9};
		deque<int> a(A, A + sizeof(A) / sizeof(int)), b(B, B + sizeof(B) / sizeof(int));

		cout << "before swap: " << endl;	cout << "\ta: "; print(a); cout << "\tb: "; print(b);

		a.swap(b);

		cout << "after swap: " << endl;	cout << "\ta: "; print(a); cout << "\tb: "; print(b); 
	}

	// test for assign
	{
		cout << "----------test for capacity interface---------" << endl;

		deque<int> a;

		a.assign((unsigned int)5, 1); cout << "assign(5, 1): "; print(a);
		int A[] = {1, 2, 3, 4, 5}; a.assign(A, A + sizeof(A) / sizeof(int));
		cout << "assign(itr1, itr2): "; print(a);
	}
}
