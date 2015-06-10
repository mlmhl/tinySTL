/**
  * @file queue_test.cpp
  * @brief 
  * @author malin malinbupt@gmail.com
  * @date 2015年05月09日 星期六 14时58分45秒
  * @version 
  * @note 
  */

#include <iostream>
#include "queue_test.h"
#include "../ml_queue.h"


using namespace std; using namespace ml;


void queue_test()
{

	{
		cout << "----------test for queue----------" << endl;

		queue<int> que; int n = 5;

		for (int i = 0; i < n; ++i) que.push(i);

		cout << "size: " << que.size() << endl;
		cout << "empty: " << que.empty() << endl;
		cout << "front: " << que.front() << endl;

		while (!que.empty()) que.pop();

		cout << que.size() << endl;
	}
	
	{
		cout << "----------test for priority_queue----------" << endl;

		int A[] = {0, 1, 2, 3, 4, 8, 9, 3, 5}; priority_queue<int> que(A, A + sizeof(A) / sizeof(int));

		while (!que.empty())
		{
			cout << "size: " << que.size() << endl;
			cout << "top: " << que.top() << endl;
			cout << endl;

			que.pop();
		}
	}
}
