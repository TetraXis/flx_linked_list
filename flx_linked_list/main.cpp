#include <iostream>
#include "flx_linked_list.hpp"
#include <random>
#include <sstream>

int main()
{
	flx::contiguous_doubly_linked_list<char> a;
	char c = 'a';


	for (size_t i = 0; i < 40; i++)
	{
		a.push_back(c++);
	}
	for (size_t i = 0; i < 40; i++)
	{
		a.push_back('b');
	}
	//a.push_back('a');

	std::stringstream ss;

	for (size_t i = 0; i < 1000; i++)
	{
		flx::u64 rand_a = rand() % 79;
		flx::u64 rand_b = rand() % 79;

		ss << i << '\n';
		ss << rand_a << '\n';
		ss << rand_b << '\n';
		for (char it : a)
		{
			ss << it << ' ';
		}
		ss << '\n';
		ss << '\n';
		a.swap(a.cbegin() + rand_a, a.cbegin() + rand_b);
	}

	std::cout << ss.str();

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

	//a.erase(a.begin() + 1, a.begin() + 3);

	//a.erase(a.begin() + 1);

	/*for (char it : a)
	{
		std::cout << it << ' ';
	}
	std::cout << '\n';*/

	a.unique('b');
	std::cout << "unique:\n";

	for (char it : a)
	{
		std::cout << it << ' ';
	}
	std::cout << '\n';

	a.reverse();
	std::cout << "reverse:\n";

	for (char it : a)
	{
		std::cout << it << ' ';
	}
	std::cout << '\n';

	return 0;
}