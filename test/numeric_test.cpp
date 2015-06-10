/**
  * @file numeric_test.cpp
  * @brief 
  * @author malin malinbupt@gmail.com
  * @date 2015年04月12日 星期日 15时32分00秒
  * @version 
  * @note 
  */

#include <iostream>
#include <vector>
#include <iterator>
#include <functional>

#include "../ml_numeric.h"


using namespace std; using namespace ml;


int main(int argc, char *argv[])
{
	vector<int> a = {1, 2, 3, 4, 5};

	cout << "accumulate_version_1: " << accumulate(a.begin(), a.end(), 0) << endl;

	cout << "accumulate_version_2: " << accumulate(a.begin(), a.end(), 0, minus<int>()) << endl;

	cout << "inner_product_version_1: " << inner_product(a.begin(), a.end(), a.begin(), 0) << endl;

	cout << "inner_product_version_2: " << inner_product(a.begin(), a.end(), a.begin(), 0, minus<int>(), plus<int>()) << endl;

	ostream_iterator<int> out(cout, " ");

	cout << "partial_sum_version_1: "; partial_sum(a.begin(), a.end(), out); cout << endl;

	cout << "partial_sum_version_2: "; partial_sum(a.begin(), a.end(), out, minus<int>()); cout << endl;

	cout << "adjacent_difference_version_1: "; adjacent_difference(a.begin(), a.end(), out); cout << endl;

	cout << "adjacent_difference_version_2: "; adjacent_difference(a.begin(), a.end(), out, plus<int>()); cout << endl;

	cout << "iota: ";

	iota(a.begin(), a.end(), 2); for (int i = 0; i < a.size(); ++i) cout << a[i] << " "; cout << endl;

	return 0;
}
