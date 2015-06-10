/**
  * @file list_test.cpp
  * @brief 
  * @author malin malinbupt@gmail.com
  * @date 2015年04月28日 星期二 15时37分25秒
  * @version 
  * @note 
  */

#include <iostream>
#include "../ml_list.h"

using namespace std; using namespace ml;


void print(const list<int> &a)
{
	for (list<int>::iterator itr = a.begin(); itr != a.end(); ++itr) cout << *itr << " "; cout << endl;
}


void list_test()
{
	// test for construct
	{
		cout << "----------test for construct----------" << endl;

		list<int> a; list<int> b((unsigned int)5, 1); print(b);

		int A[] = {1, 2, 3}; list<int> c(A, A + sizeof(A) / sizeof(int)); print(c);
	}

	// test for copy
	{
		cout << "----------test for copy----------" << endl;

		list<int> a((unsigned int)5, 2), b(a), c; c = a;

		cout << "copy construct: "; print(b);
		cout << "assignment: "; print(c);
	}

	// test for unmutable interface
	{
		cout << "----------test for unmutable interface----------" << endl;

		int A[] = {1, 2 ,3}; list<int> a(A, A + sizeof(A) / sizeof(int));

		cout << "size: " << a.size() << endl; cout << "empty: " << a.empty() << endl;
	}

	// test for mutable interface
	{
		cout << "----------test for mutable interface----------" << endl;

		list<int> a; int n = 5;
		
		for (int i = 0; i < n; ++i) a.push_back(i); print(a);
		a.pop_back(); print(a);
		a.pop_front(); print(a);
		a.push_front(10); print(a);

		a.insert(++a.begin(), (unsigned int)3, 100); print(a);
		int A[] = {7, 8, 9}; a.insert(++a.begin(), A, A + sizeof(A) / sizeof(int)); print(a);

		list<int>::iterator itr1 = ++a.begin(), itr2;
		itr1 = a.erase(itr1); print(a);
		itr2 = itr1++++++++;
		a.erase(itr1, itr2); print(a);

		a.assign((unsigned int)5, 1); print(a);
		a.assign(A, A + sizeof(A) / sizeof(int)); print(a);
		a.clear(); print(a);
	}

	// test for algorithms
	{
		cout << "----------test for mutable interface----------" << endl;

		int A[] = {1, 2, 3}, B[] = {4, 5, 6};
		list<int> a(A, A + sizeof(A) / sizeof(int)), b(B, B + sizeof(B) / sizeof(int));

		cout << "splice_1: ";
		a.splice(a.begin(), b);
		print(a);
		cout << "splice_2: ";
		b.splice(b.begin(), a, a.begin(), ++++++a.begin());
		print(b);

		a.remove(2);
		cout << "remove: ";
		print(a);

		a.insert(++a.begin(), 1);
		a.push_back(3);
		a.unique();
		cout << "unique: ";
		print(a);

		b.reverse();
		cout << "reverse: ";
		print(b);

		b.sort();
		cout << "sort: ";
		print(b);

		a.merge(b);
		cout << "merge: ";
		print(a);
	}
}
