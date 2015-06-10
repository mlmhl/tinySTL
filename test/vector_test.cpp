/**
  * @file vector_test.cpp
  * @brief 
  * @author malin malinbupt@gmail.com
  * @date 2015年04月26日 星期日 14时57分14秒
  * @version 
  * @note 
  */

#include <iostream>
#include "../ml_vector.h"

using namespace ml; using namespace std;

void print(const vector<int> &a)
{
	for (int i = 0; i < a.size(); ++i) cout << a[i] << " ";

	cout << endl;
}

void vector_test()
{
	// test for construct
	{
		cout << "----------test for constructor---------" << endl;

		vector<int> a; vector<int> b((unsigned int)(10), 0);
		int A[] = {1, 2, 3, 4, 5}; vector<int> c(A, A + 5);

		cout << a.size() << " " << b.size() << " " << c.size() << endl;
	}

	// test for copy
	{
		cout <<	"----------test for copy---------" << endl;

		int A[] = {1, 2, 3, 4, 5}; vector<int> a(A, A + sizeof(A) / sizeof(int)), b(a); print(b);
		vector<int> c; c = a; print(c);
	}

	// test for add element
	{
		cout << "----------test for add element---------" << endl;

		vector<int> a; int n = 5;
		for (int i = 0; i < n; ++i) a.push_back(i);

		print(a);
		vector<int>::iterator itr = a.begin() + 3;
		cout << *(itr = a.insert(itr, 10)) << endl;

		int A[] = {7, 8, 9};
		a.insert(itr, A, A + sizeof(A) / sizeof (int)); print(a);
		a.insert(a.begin() + 2, (unsigned int)5, 100); print(a);
	}

	// test for unmutable interface
	{
		cout << "----------test for unmutable interface---------" << endl;

		vector<int> a; int n = 5;

		for (int i = 0; i < n; ++i) a.push_back(i);

		cout << "size: " << a.size() << endl;
		cout << "capacity: " << a.capacity() << endl;
		cout << "front: " << a.front() << endl;
		cout << "back: " << a.back() << endl;
		cout << "empty: " << boolalpha << a.empty() << endl;
	}

	// test for delete element
	{
		cout << "----------test for delete element---------" << endl;

		int A[] = {1, 2, 3, 4, 5, 6, 7, 8, 9}; vector<int> a(A, A + sizeof(A) / sizeof(int));

		print(a); a.pop_back(); print(a);

		vector<int>::iterator itr = a.begin() + 5; itr = a.erase(itr);
		cout << *itr << endl; print(a);

		a.erase(a.begin() + 1, a.begin() + 5); print(a);
	}

	// test for swap
	{
		cout << "----------test for swap---------" << endl;

		int A[] = {1, 2, 3, 4, 5}, B[] = {7, 8, 9};
		vector<int> a(A, A + sizeof(A) / sizeof(int)), b(B, B + sizeof(B) / sizeof(int));

		cout << "before swap: " << endl;	cout << "\ta: "; print(a); cout << "\tb: "; print(b);

		a.swap(b);

		cout << "after swap: " << endl;	cout << "\ta: "; print(a); cout << "\tb: "; print(b); 
	}

	// test for capacity interface
	{
		cout << "----------test for capacity interface---------" << endl;

		vector<int> a; a.reserve(10);

		cout << "a's capacity before reserve: " << a.capacity();
		cout << "\ta's capacity after reserve(10): " << a.capacity() << endl;

		int B[] = {1, 2, 3, 4, 5}; vector<int> b(B, B + sizeof(B) / sizeof(int));

		cout << "b before resize: "; print(b);
		b.resize(3);
		cout << "after resize(3): "; print(b);
		b.resize(10, 1);
		cout << "after resize(10, 1): "; print(b);
	}

	// test for assign
	{
		cout << "----------test for capacity interface---------" << endl;

		vector<int> a;

		a.assign((unsigned int)5, 1); cout << "assign(5, 1): "; print(a);
		int A[] = {1, 2, 3, 4, 5}; a.assign(A, A + sizeof(A) / sizeof(int));
		cout << "assign(itr1, itr2): "; print(a);
	}
}
