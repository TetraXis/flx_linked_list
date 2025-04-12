#include <iostream>
#include "flx_linked_list.hpp"

int main()
{
	flx::contiguous_double_linked_list<char> a;
	char c = 'a';

	for (size_t i = 0; i < 100; i++)
	{
		a.push_back(c++);
	}

	a.erase(a.cbegin()+ 10, a.cbegin() + 20);

	for (char it : a)
	{
		std::cout << it << ' ';
	}

	return 0;
}