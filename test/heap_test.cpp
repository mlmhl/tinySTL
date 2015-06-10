/**
  * @file heap_test.cpp
  * @brief 
  * @author malin malinbupt@gmail.com
  * @date 2015年05月10日 星期日 11时15分37秒
  * @version 
  * @note 
  */


#include <iostream>
#include "heap_test.h"
#include "../ml_heap.h"
#include "../ml_vector.h"


using namespace ml; using std::cout; using std::endl;


extern void print(const vector<int> &a);


void heap_test()
{
	int A[] = {0, 1, 2, 3, 4, 8, 9, 3, 5}; vector<int> a(A, A + sizeof(A) / sizeof(int));

	cout << "----------test for make_heap----------" << endl;

	make_heap(a.begin(), a.end()); print(a);

	cout << "----------test for push_heap----------" << endl;

	a.push_back(7); push_heap(a.begin(), a.end()); print(a);

	cout << "----------test for pop_heap----------" << endl;

	pop_heap(a.begin(), a.end());
	cout << a.back() << endl;
	a.pop_back(); print(a);

	cout << "----------test for sort_heap----------" << endl;

	sort_heap(a.begin(), a.end()); print(a);
}
