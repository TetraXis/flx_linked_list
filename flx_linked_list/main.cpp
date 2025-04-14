#include <iostream>
#include "flx_linked_list.hpp"

int main()
{
	flx::contiguous_doubly_linked_list<char> a;
	char c = 'a';

	for (size_t i = 0; i < 40; i++)
	{
		a.push_back(c++);
	}

	/*a.front();
	a.back();
	a.push_front({});
	a.pop_front();
	a.insert(a.begin(), 'a');
	a.erase(a.begin());
	a.empty();
	a.clear();
	a.size();
	a.swap(a.begin(), a.begin() + 1);
	a.reverse();
	a.unique('a');*/

	for (char it : a)
	{
		std::cout << it << ' ';
	}
	std::cout << '\n';

	a.erase(a.begin() + 1, a.begin() + 3);

	//a.erase(a.begin() + 1);

	for (char it : a)
	{
		std::cout << it << ' ';
	}
	std::cout << '\n';

	return 0;
}