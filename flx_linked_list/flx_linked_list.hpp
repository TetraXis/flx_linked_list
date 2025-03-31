#pragma once

#include "flx_types.hpp"

namespace flx
{
	template<typename ty>
	struct dll_element
	{
	flx_public:
		dll_element<ty>* next = nullptr;
		dll_element<ty>* prev = nullptr;
		ty data{};

	flx_public:
		dll_element() = default;
		dll_element(const ty& new_data) : data(new_data){}
	};

	// TODO: Make it like a vector, so elements are in order one by one.
	template<typename ty>
	struct dll
	{
	flx_public:
		dll_element<ty>* first = nullptr;
		dll_element<ty>* last = nullptr;

	flx_public:
		~dll();

		void push_back(const ty&);
		ty pop_back();

		void swap(ty*, ty*);
	};


	template<typename ty>
	inline dll<ty>::~dll()
	{
		while (first)
		{
			last = first->next; // used as temp
			delete first;
			first = last;
		}
	}

	template<typename ty>
	inline void dll<ty>::push_back(const ty& elem)
	{
		if (!first)
		{
			first = new dll_element<ty>(elem);
			last = first;
		}
		if (first == last)
		{
			last = new dll_element<ty>(elem);
			first->next = last;
			last->prev = first;
		}

		last->next = new dll_element<ty>(elem);
		last->next->prev = last;
		last = last->next;
	}

	template<typename ty>
	inline ty dll<ty>::pop_back()
	{
		// TODO: ADD
	}

	template<typename ty>
	inline void dll<ty>::swap(ty* a, ty* b)
	{
		dll_element<ty>* a_elem = nullptr;
		dll_element<ty>* b_elem = nullptr;

		dll_element<ty>* i = first;

		while (i)
		{
			if (i->data == a)
			{
				a_elem = i;
			}
			if (i->data == b)
			{
				b_elem = i;
			}
		}

		if (!a_elem || !b_elem)
		{
			return;
		}


	}
}