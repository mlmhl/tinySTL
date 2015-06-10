/**
  * @file stack_test.cpp
  * @brief 
  * @author malin malinbupt@gmail.com
  * @date 2015年05月09日 星期六 11时27分18秒
  * @version 
  * @note 
  */

#include <iostream>
#include "../ml_stack.h"
#include "stack_test.h"

using namespace std; using namespace ml;


void stack_test()
{
	cout << "----------test for stack----------" << endl;

	stack<int> st; int n = 5;

	for (int i = 0; i < n; ++i) st.push(i);

	cout << "size: " << st.size() << endl;
	cout << "empty: " << st.empty() << endl;
	cout << "top: " << st.top() << endl;

	while (!st.empty()) st.pop();

	cout << st.size() << endl;
}
