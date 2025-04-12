#pragma once

#include <cassert>
#include <vector>

#include "flx_types.hpp"

/// <summary>
/// This double linked list is backed by an std::vector
/// This allows for a cache frindly approach
/// Because std::vector will move data at some point,
/// nodes will use indexes to point to `next` and `prev`.
/// 
/// When moving nodes inside std::vector, neigbours should be updated.
/// 
/// Because at some time data will be moved, taking pointers from it
/// is dangerous.
/// </summary>

namespace flx
{
	/// <summary>
	/// std::vector baked double linked list.
	/// Be cautious taking pointers of elemts,
	/// becuase at some point they will be moded.
	/// </summary>
	/// <typeparam name="ty"> - Type of data</typeparam>
	template<typename ty>
	struct contiguous_double_linked_list
	{
		static constexpr u64 NULLNODE = ~0ull;

		struct node
		{
			friend contiguous_double_linked_list;

		flx_private:
			u64 next = NULLNODE;
			u64 prev = NULLNODE;
			ty data{};

		flx_public:
			node() = default;
			node(const ty& new_data) : data(new_data)
			{
			}
		};

		struct iterator;
		struct const_iterator;
		template<typename iter_ty>
		struct reverse_iterator;

	flx_private:
		u64 front_idx = NULLNODE;
		u64 back_idx = NULLNODE;
		std::vector<node> nodes{};

	flx_public:

		ty& front();
		ty& back();

		void push_back(const ty&);
		ty pop_back();
		void push_front(const ty&);
		ty pop_front();
		iterator insert(const const_iterator&, const ty&);
		iterator erase(const const_iterator&);
		iterator erase(const const_iterator&, const const_iterator&);

		bool empty() const;
		void clear();
		u64 size() const;

		void swap(const const_iterator&, const const_iterator&);
		void reverse();
		void unique(const ty&);

		iterator begin();
		iterator end();

		const_iterator cbegin() const;
		const_iterator cend() const;

		reverse_iterator<iterator> rbegin();
		reverse_iterator<iterator> rend();

		reverse_iterator<const_iterator> crbegin();
		reverse_iterator<const_iterator> crend();	

		struct iterator
		{
			friend contiguous_double_linked_list;

		flx_private:
			std::vector<node>& nodes;
			u64 current_idx = NULLNODE;

		flx_public:

			iterator(std::vector<node>& ref, u64 idx)
				: nodes(ref), current_idx(idx)
			{
			}

			ty& operator * ()
			{
				return nodes[current_idx].data;
			}

			iterator& operator ++ () // prefix ++
			{
				current_idx = nodes[current_idx].next;
				return *this;
			}

			iterator& operator -- () // prefix --
			{
				current_idx = nodes[current_idx].prev;
				return *this;
			}

			iterator operator + (u64 offset) const
			{
				iterator result(*this);
				while (offset-- > 0)
				{
					++result;
				}
				return result;
			}

			iterator operator - (u64 offset) const
			{
				iterator result(*this);
				while (offset-- > 0)
				{
					--result;
				}
				return result;
			}

			bool operator == (const iterator& other) const
			{
				return current_idx == other.current_idx && &nodes == &other.nodes;
			}

			bool operator != (const iterator& other) const
			{
				return current_idx != other.current_idx || &nodes != &other.nodes;
			}
		};



		struct const_iterator
		{
			friend contiguous_double_linked_list;

		flx_private:
			const std::vector<node>& nodes;
			u64 current_idx = NULLNODE;

		flx_public:

			const_iterator(const std::vector<node>& ref, u64 idx)
				: nodes(ref), current_idx(idx)
			{
			}

			const ty& operator * () const
			{
				return nodes[current_idx].data;
			}

			const_iterator& operator ++ () // prefix ++
			{
				current_idx = nodes[current_idx].next;
				return *this;
			}

			const_iterator& operator -- () // prefix --
			{
				current_idx = nodes[current_idx].prev;
				return *this;
			}

			const_iterator operator + (u64 offset) const
			{
				const_iterator result(*this);
				while (offset-- > 0)
				{
					++result;
				}
				return result;
			}

			const_iterator operator - (u64 offset) const
			{
				const_iterator result(*this);
				while (offset-- > 0)
				{
					--result;
				}
				return result;
			}

			bool operator == (const const_iterator& other) const
			{
				return current_idx == other.current_idx && &nodes == &other.nodes;
			}

			bool operator != (const const_iterator& other) const
			{
				return current_idx != other.current_idx || &nodes != &other.nodes;;
			}
		};



		template<typename iter_ty>
		struct reverse_iterator
		{
			friend contiguous_double_linked_list;

		flx_private:
			iter_ty iter;

		flx_public:

			explicit reverse_iterator(const iter_ty& new_iter)
				: iter(new_iter)
			{
			}

			reverse_iterator<iter_ty>& operator ++ () // prefix ++
			{
				--iter;
				return *this;
			}

			reverse_iterator<iter_ty>& operator -- () // prefix --
			{
				++iter;
				return *this;
			}

			reverse_iterator<iter_ty> operator + (u64 offset) const
			{
				reverse_iterator<iter_ty> result(*this);
				while (offset-- > 0)
				{
					--result;
				}
				return result;
			}

			reverse_iterator<iter_ty> operator - (u64 offset) const
			{
				reverse_iterator<iter_ty> result(*this);
				while (offset-- > 0)
				{
					++result;
				}
				return result;
			}

			// I hope this works

			ty& operator*()
			{
				return *iter;
			}

			const ty& operator*() const
			{
				return *iter;
			}

			bool operator == (const reverse_iterator<iter_ty>& other) const
			{
				return iter.iter == other.iter;
			}

			bool operator != (const reverse_iterator<iter_ty>& other) const
			{
				return iter.iter != other.iter;
			}
		};
	};


	template<typename ty>
	inline ty& contiguous_double_linked_list<ty>::front()
	{
		return nodes[front_idx].data;
	}

	template<typename ty>
	inline ty& contiguous_double_linked_list<ty>::back()
	{
		return nodes[back_idx].data;
	}

	template<typename ty>
	inline void contiguous_double_linked_list<ty>::push_back(const ty& value)
	{
		const u64 new_idx = nodes.size();
		nodes.emplace_back(value);

		if (front_idx == NULLNODE)
		{
			front_idx = new_idx;
			back_idx= new_idx;
			return;
		}
		if (front_idx == back_idx) // only when adding second element
		{
			back_idx = new_idx;
			nodes[front_idx].next = new_idx;
			nodes[back_idx].prev = front_idx;
			return;
		}

		nodes[back_idx].next = new_idx;
		nodes[new_idx].prev = back_idx;
		back_idx = new_idx;

		return;
	}

	template<typename ty>
	inline ty contiguous_double_linked_list<ty>::pop_back()
	{
		assert(!empty() && "pop_back called on empty list");

		ty value = nodes[back_idx].data;

		if (size() == 1)
		{
			clear();
			return value;
		}

		// updating back node's neighbours
		if (nodes.back().prev != NULLNODE)
		{
			nodes[nodes.back().prev].next = back_idx;
		}
		if (nodes.back().next != NULLNODE)
		{
			nodes[nodes.back().next].prev = back_idx;
		}

		// moves back node to be actually at back
		std::swap(nodes[back_idx], nodes.back());
		back_idx = nodes.back().prev;
		nodes[back_idx].next = NULLNODE;
		nodes.pop_back();

		if (size() == 1)
		{
			nodes[front_idx].next = NULLNODE;
			nodes[front_idx].prev = NULLNODE;
		}

		return value;
	}

	template<typename ty>
	inline void contiguous_double_linked_list<ty>::push_front(const ty& value)
	{
		const u64 new_idx = nodes.size();
		nodes.emplace_back(value);

		if (front_idx == NULLNODE)
		{
			front_idx = new_idx;
			back_idx = new_idx;
			return;
		}
		if (front_idx == back_idx) // only when adding second element
		{
			front_idx = new_idx;
			nodes[front_idx].next = back_idx;
			nodes[back_idx].prev = new_idx;
			return;
		}

		nodes[front_idx].prev = new_idx;
		nodes[new_idx].next = front_idx;
		front_idx = new_idx;

		return;
	}

	template<typename ty>
	inline ty contiguous_double_linked_list<ty>::pop_front()
	{
		assert(!empty() && "pop_front called on empty list");

		ty value = nodes[front_idx].data;

		if (size() == 1)
		{
			clear();
			return value;
		}

		// updating back node's neighbours
		if (nodes.back().prev != NULLNODE)
		{
			nodes[nodes.back().prev].next = front_idx;
		}
		if (nodes.back().next != NULLNODE)
		{
			nodes[nodes.back().next].prev = front_idx;
		}

		// moves front node to be actually at back
		std::swap(nodes[front_idx], nodes.back());
		front_idx = nodes.back().next;
		nodes[front_idx].prev = NULLNODE;
		nodes.pop_back();

		if (size() == 1)
		{
			nodes[front_idx].next = NULLNODE;
			nodes[front_idx].prev = NULLNODE;
		}

		return value;
	}

	template<typename ty>
	inline contiguous_double_linked_list<ty>::iterator contiguous_double_linked_list<ty>::insert(const contiguous_double_linked_list<ty>::const_iterator& where, const ty& value)
	{
		assert(!empty() && "insert attempt on empty list");
		assert(where.current_idx < size() && "insert pos is out of bounds");
		assert(where.nodes == nodes && "insert iterator is from other list");
		
		if (where.current_idx == front_idx)
		{
			push_front(value);
			return;
		}

		u64 new_idx = nodes.size();
		nodes.emplace_back(value);

		nodes[new_idx].next = where.current_idx;
		nodes[new_idx].prev = nodes[where.current_idx].prev;
		nodes[nodes[where.current_idx].prev].next = new_idx;
		nodes[where.current_idx].prev = new_idx;

		return iterator(nodes, new_idx);
	}

	template<typename ty>
	inline contiguous_double_linked_list<ty>::iterator contiguous_double_linked_list<ty>::erase(const contiguous_double_linked_list<ty>::const_iterator& where)
	{
		assert(!empty() && "erase attempt on empty list");
		assert(where.current_idx < size() && "erase pos is out of bounds");
		assert(where.nodes == nodes && "erase iterator is from other list");

		iterator return_iter(nodes, nodes[where.current_idx].next);

		if (where.current_idx == front_idx)
		{
			pop_front();
			return return_iter;
		}
		if (where.current_idx == back_idx)
		{
			pop_back();
			return return_iter;
		}

		// updating erased node's neighbours
		nodes[nodes[where.current_idx].next].prev = nodes[where.current_idx].prev;
		nodes[nodes[where.current_idx].prev].next = nodes[where.current_idx].next;
		// updating back node's neighbours
		if (nodes.back().prev != NULLNODE)
		{
			nodes[nodes.back().prev].next = where.current_idx;
		}
		if (nodes.back().next != NULLNODE)
		{
			nodes[nodes.back().next].prev = where.current_idx;
		}

		std::swap(nodes[where.current_idx], nodes.back());
		nodes.pop_back();

		return return_iter;
	}

	template<typename ty>
	inline contiguous_double_linked_list<ty>::iterator contiguous_double_linked_list<ty>::erase(const contiguous_double_linked_list<ty>::const_iterator& first, const contiguous_double_linked_list<ty>::const_iterator& last)
	{
		assert(!empty() && "erase attempt on empty list");
		assert(first.current_idx < size() && "erase first iterator is out of bounds");
		assert(last.current_idx == NULLNODE || last.current_idx < size() && "erase last iterator is out of bounds");
		assert(&first.nodes == &nodes && "erase first iterator is from other list");
		assert(&last.nodes == &nodes && "erase last iterator is from other list");

		u64 pos = first.current_idx;

		if (first.current_idx == front_idx && last.current_idx == NULLNODE)
		{
			clear();
			return end();
		}

		// erasing from `front_idx`, only one node reconnection needed
		if (first.current_idx == front_idx)
		{
			// `last` garanteed to be not `NULLNODE`
			front_idx = last.current_idx; // since `last` won't get erased
			nodes[front_idx].prev = NULLNODE;
		}

		// erasing from `back_idx`, only one node reconnection needed
		if (last.current_idx == NULLNODE)
		{
			// `first` garanteed to be not `front_idx`
			back_idx = nodes[first.current_idx].prev;
			nodes[back_idx].next = NULLNODE;
		}

		// erasing chunk in the middle, both node reconnections are required
		if (first.current_idx != front_idx && last.current_idx != NULLNODE)
		{
			nodes[nodes[first.current_idx].prev].next = last.current_idx; // since `last` won't get erased
			nodes[last.current_idx].prev = nodes[first.current_idx].prev; // since `last` won't get erased
		}

		u64 distance = 0;
		/*u64 left_idx = nodes[pos].prev;
		u64 right_idx = last.current_idx;*/

		while (pos != last.current_idx)
		{
			// updating back node's neigbours
			if (nodes[nodes.size() - 1 - distance].prev != NULLNODE)
			{
				nodes[nodes[nodes.size() - 1 - distance].prev].next = pos;
			}
			if (nodes[nodes.size() - 1 - distance].next != NULLNODE)
			{
				nodes[nodes[nodes.size() - 1 - distance].next].prev = pos;
			}

			std::swap(nodes[pos], nodes[nodes.size() - 1 - distance]);
			++distance;
			pos = nodes[pos].next;
		}

		nodes.erase(nodes.end() - distance, nodes.end());

		return { nodes, last.current_idx };
	}

	template<typename ty>
	inline bool contiguous_double_linked_list<ty>::empty() const
	{
		return nodes.size() == 0;
	}

	template<typename ty>
	inline void contiguous_double_linked_list<ty>::clear()
	{
		front_idx = NULLNODE;
		back_idx = NULLNODE;
		nodes.clear();
	}

	template<typename ty>
	inline u64 contiguous_double_linked_list<ty>::size() const
	{
		return nodes.size();
	}

	template<typename ty>
	inline void contiguous_double_linked_list<ty>::swap(const contiguous_double_linked_list<ty>::const_iterator& where_a, const contiguous_double_linked_list<ty>::const_iterator& where_b)
	{
		assert(where_a.current_idx < size() && where_b.current_idx < size() && "swap attempt outside of bounds");

		if (nodes[where_a.current_idx].prev != NULLNODE)
		{
			nodes[nodes[where_a.current_idx].prev].next = where_b.current_idx;
		}
		if (nodes[where_a.current_idx].next != NULLNODE)
		{
			nodes[nodes[where_a.current_idx].next].prev = where_b.current_idx;
		}
		if (nodes[where_b.current_idx].prev != NULLNODE)
		{
			nodes[nodes[where_b.current_idx].prev].next = where_a.current_idx;
		}
		if (nodes[where_b.current_idx].next != NULLNODE)
		{
			nodes[nodes[where_b.current_idx].next].prev = where_a.current_idx;
		}

		std::swap(nodes[where_a.current_idx], nodes[where_b.current_idx]);
		return;
	}

	template<typename ty>
	inline void contiguous_double_linked_list<ty>::reverse()
	{
		std::swap(front_idx, back_idx);
		for (u64 i = 0; i < nodes.size(); i++)
		{
			std::swap(nodes[i].next, nodes[i].prev);
		}
	}

	template<typename ty>
	inline void contiguous_double_linked_list<ty>::unique(const ty& value)
	{// something is wrong here logically
		u64 value_idx = front_idx;
		u64 erased_amount = 0;

		while (value_idx != NULLNODE && nodes[value_idx].data != value)
		{
			value_idx = nodes[value_idx].next;
		}
		if (value_idx == NULLNODE)
		{
			return;
		}

		// dont need to worry about `front_idx`
		while (back_idx != NULLNODE && nodes[back_idx].data == value)
		{
			back_idx = nodes[back_idx].prev;
		}
		if (back_idx == NULLNODE)
		{
			clear();
			return;
		}

		value_idx = nodes[value_idx].next;

		while (value_idx != NULLNODE)
		{
			if (nodes[value_idx].data == value)
			{
				// no need to check for `prev`, it will always be there
				nodes[nodes[value_idx].prev].next = nodes[value_idx].next;

				if (nodes[value_idx].next != NULLNODE)
				{
					nodes[nodes[value_idx].next].prev = nodes[value_idx].prev;
				}

				// updating future moving node's neighbours
				if (nodes[nodes.size() - 1 - erased_amount].prev != NULLNODE)
				{
					nodes[nodes[nodes.size() - 1 - erased_amount].prev].next = value_idx;
				}
				if (nodes[nodes.size() - 1 - erased_amount].next != NULLNODE)
				{
					nodes[nodes[nodes.size() - 1 - erased_amount].next].prev = value_idx;
				}

				std::swap(nodes[value_idx], nodes[nodes.size() - 1 - erased_amount]);
				++erased_amount;
			}
			value_idx = nodes[value_idx].next;
		}

		nodes.erase(nodes.end() - erased_amount, nodes.end());
		return;
	}

	template<typename ty>
	inline contiguous_double_linked_list<ty>::iterator contiguous_double_linked_list<ty>::begin()
	{
		return iterator(nodes, front_idx);
	}

	template<typename ty>
	inline contiguous_double_linked_list<ty>::iterator contiguous_double_linked_list<ty>::end()
	{
		return iterator(nodes, NULLNODE);
	}

	template<typename ty>
	inline contiguous_double_linked_list<ty>::const_iterator contiguous_double_linked_list<ty>::cbegin() const
	{
		return const_iterator(nodes, front_idx);
	}

	template<typename ty>
	inline contiguous_double_linked_list<ty>::const_iterator contiguous_double_linked_list<ty>::cend() const
	{
		return const_iterator(nodes, NULLNODE);
	}

	template<typename ty>
	inline contiguous_double_linked_list<ty>::reverse_iterator<typename contiguous_double_linked_list<ty>::iterator> contiguous_double_linked_list<ty>::rbegin()
	{
		return reverse_iterator<iterator>(iterator(nodes, back_idx));
	}

	template<typename ty>
	inline contiguous_double_linked_list<ty>::reverse_iterator<typename contiguous_double_linked_list<ty>::iterator> contiguous_double_linked_list<ty>::rend()
	{
		return reverse_iterator<iterator>(iterator(nodes, NULLNODE));
	}

	template<typename ty>
	inline contiguous_double_linked_list<ty>::reverse_iterator<typename contiguous_double_linked_list<ty>::const_iterator> contiguous_double_linked_list<ty>::crbegin()
	{
		return reverse_iterator<const_iterator>(const_iterator(nodes, back_idx));
	}

	template<typename ty>
	inline contiguous_double_linked_list<ty>::reverse_iterator<typename contiguous_double_linked_list<ty>::const_iterator> contiguous_double_linked_list<ty>::crend()
	{
		return reverse_iterator<const_iterator>(const_iterator(nodes, NULLNODE));
	}
}