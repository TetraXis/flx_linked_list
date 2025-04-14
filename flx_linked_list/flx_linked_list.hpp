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
/// When doing anything, that changes amount of elements or changes their order,
/// nodes in internal vector will get moved, thus invalidatin all pointers and iterators.
/// </summary>

namespace flx
{
	/// <summary>
	/// An std::vector baked doubly linked list for cache friendliness.
	/// !!! All iterators and pointers to elements will get invalid after
	/// any function call that changes amount of elements or changes their order.
	/// </summary>
	/// <typeparam name="ty"> - Type of data</typeparam>
	template<typename ty>
	struct contiguous_doubly_linked_list
	{
		static constexpr u64 NULLNODE = ~0ull;

		struct node
		{
			friend contiguous_doubly_linked_list;

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

		reverse_iterator<const_iterator> crbegin() const;
		reverse_iterator<const_iterator> crend() const;	

		struct iterator
		{
			friend contiguous_doubly_linked_list;

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

			ty* operator -> ()
			{
				return &nodes[current_idx].data;
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

			u64 prev_idx() const
			{
				return nodes[current_idx].prev;
			}
			u64 next_idx() const
			{
				return nodes[current_idx].next;
			}
		}; // iterator

		struct const_iterator
		{
			friend contiguous_doubly_linked_list;

		flx_private:
			const std::vector<node>& nodes;
			u64 current_idx = NULLNODE;

		flx_public:

			const_iterator(const std::vector<node>& ref, u64 idx)
				: nodes(ref), current_idx(idx)
			{
			}

			const_iterator(const iterator& iter)
				: nodes(iter.nodes), current_idx(iter.current_idx)
			{
			}

			const ty& operator * () const
			{
				return nodes[current_idx].data;
			}

			const ty* operator -> () const
			{
				return &nodes[current_idx].data;
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

			u64 prev_idx() const
			{
				return nodes[current_idx].prev;
			}
			u64 next_idx() const
			{
				return nodes[current_idx].next;
			}
		}; // const_iterator

		template<typename iter_ty>
		struct reverse_iterator
		{
			friend contiguous_doubly_linked_list;

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

			ty& operator*()
			{
				return *iter;
			}

			const ty& operator*() const
			{
				return *iter;
			}

			ty& operator -> ()
			{
				return *iter;
			}

			const ty& operator -> () const
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

			u64 prev_idx() const
			{
				return nodes[iter.current_idx].next;
			}
			u64 next_idx() const
			{
				return nodes[iter.current_idx].prev;
			}
		}; // reverse_iterator

	flx_private:

		/// <summary>
		/// Moves erased element at the iterator to the back of a vector.
		/// Also changes the first input iterator, because nodes were moved.
		/// </summary>
		void imp_move_erased(const_iterator&, const const_iterator&);
	}; // contiguous_doubly_linked_list


	template<typename ty>
	inline ty& contiguous_doubly_linked_list<ty>::front()
	{
		return nodes[front_idx].data;
	}

	template<typename ty>
	inline ty& contiguous_doubly_linked_list<ty>::back()
	{
		return nodes[back_idx].data;
	}

	template<typename ty>
	inline void contiguous_doubly_linked_list<ty>::push_back(const ty& value)
	{
		const u64 new_idx = nodes.size();
		nodes.emplace_back(value);

		if (new_idx == 0)
		{
			front_idx = new_idx;
			back_idx= new_idx;
			return;
		}

		nodes[back_idx].next = new_idx;
		nodes[new_idx].prev = back_idx;
		back_idx = new_idx;

		return;
	}

	template<typename ty>
	inline ty contiguous_doubly_linked_list<ty>::pop_back()
	{
		assert(!empty() && "pop_back called on empty list");

		ty value = nodes[back_idx].data;

		if (size() == 1)
		{
			clear();
			return value;
		}

		imp_move_erased(end(), { nodes, nodes.size() - 1 });
		back_idx = nodes[back_idx].prev;
		nodes.pop_back();

		return value;
	}

	template<typename ty>
	inline void contiguous_doubly_linked_list<ty>::push_front(const ty& value)
	{
		const u64 new_idx = nodes.size();
		nodes.emplace_back(value);

		if (new_idx == 0)
		{
			front_idx = new_idx;
			back_idx = new_idx;
			return;
		}

		nodes[front_idx].prev = new_idx;
		nodes[new_idx].next = front_idx;
		front_idx = new_idx;

		return;
	}

	template<typename ty>
	inline ty contiguous_doubly_linked_list<ty>::pop_front()
	{
		assert(!empty() && "pop_front called on empty list");

		ty value = nodes[front_idx].data;

		if (size() == 1)
		{
			clear();
			return value;
		}

		imp_move_erased(begin(), { nodes, nodes.size() - 1 });
		front_idx = nodes.back().next;
		nodes.pop_back();

		return value;
	}

	template<typename ty>
	inline contiguous_doubly_linked_list<ty>::iterator contiguous_doubly_linked_list<ty>::insert(const contiguous_doubly_linked_list<ty>::const_iterator& where, const ty& value)
	{
		assert(!empty() && "insert attempt on empty list");
		assert(where.current_idx < size() && "insert pos is out of bounds");
		assert(&where.nodes == &nodes && "insert iterator is from other list");
		
		if (where.current_idx == front_idx)
		{
			push_front(value);
			return begin();
		}

		u64 new_idx = nodes.size();
		nodes.emplace_back(value);

		nodes[new_idx].next = where.current_idx;
		nodes[new_idx].prev = where.prev_idx();
		nodes[where.prev_idx()].next = new_idx;
		nodes[where.current_idx].prev = new_idx;

		return { nodes, new_idx };
	}

	template<typename ty>
	inline contiguous_doubly_linked_list<ty>::iterator contiguous_doubly_linked_list<ty>::erase(const contiguous_doubly_linked_list<ty>::const_iterator& where)
	{
		assert(!empty() && "erase attempt on empty list");
		assert(where.current_idx < size() && "erase pos is out of bounds");
		assert(&where.nodes == &nodes && "erase iterator is from other list");

		iterator return_iter(nodes, where.next_idx());

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

		imp_move_erased(where, { nodes, nodes.size() - 1 });
		nodes.pop_back();

		return return_iter;
	}

	template<typename ty>
	inline contiguous_doubly_linked_list<ty>::iterator contiguous_doubly_linked_list<ty>::erase(const contiguous_doubly_linked_list<ty>::const_iterator& first, const contiguous_doubly_linked_list<ty>::const_iterator& last)
	{
		assert(!empty() && "erase attempt on empty list");
		assert(first.current_idx < size() && "erase first iterator is out of bounds");
		assert(last.current_idx == NULLNODE || last.current_idx < size() && "erase last iterator is out of bounds");
		assert(&first.nodes == &nodes && "erase first iterator is from other list");
		assert(&last.nodes == &nodes && "erase last iterator is from other list");

		if (first.current_idx == front_idx && last.current_idx == NULLNODE)
		{
			clear();
			return end();
		}

		if (first.current_idx == front_idx)
		{
			front_idx = last.current_idx;
		}
		if (last.current_idx == NULLNODE)
		{
			back_idx = first.prev_idx();
		}

		const_iterator it = first;
		u64 distance = 0;

		while (it != last)
		{
			imp_move_erased(it, { nodes, nodes.size() - 1 - distance });
			++it;
			++distance;
		}

		nodes.erase(nodes.end() - distance, nodes.end());

		return { nodes, last.current_idx };
	}

	template<typename ty>
	inline bool contiguous_doubly_linked_list<ty>::empty() const
	{
		return nodes.size() == 0;
	}

	template<typename ty>
	inline void contiguous_doubly_linked_list<ty>::clear()
	{
		front_idx = NULLNODE;
		back_idx = NULLNODE;
		nodes.clear();
	}

	template<typename ty>
	inline u64 contiguous_doubly_linked_list<ty>::size() const
	{
		return nodes.size();
	}

	template<typename ty>
	inline void contiguous_doubly_linked_list<ty>::swap(const contiguous_doubly_linked_list<ty>::const_iterator& where_a, const contiguous_doubly_linked_list<ty>::const_iterator& where_b)
	{
		assert(where_a.current_idx < size() && where_b.current_idx < size() && "swap attempt outside of bounds");

		std::swap(*where_a, *where_b);
		return;
	}

	template<typename ty>
	inline void contiguous_doubly_linked_list<ty>::reverse()
	{
		std::swap(front_idx, back_idx);
		for (u64 i = 0; i < nodes.size(); i++)
		{
			std::swap(nodes[i].next, nodes[i].prev);
		}
	}

	template<typename ty>
	inline void contiguous_doubly_linked_list<ty>::unique(const ty& value)
	{
		u64 erased_amount = 0;

		iterator it = begin();

		while (it.current_idx != NULLNODE && *it != value)
		{
			++it;
		}
		if (it.current_idx == NULLNODE)
		{
			return;
		}

		for (; it.current_idx != back_idx; ++it)
		{
			if (*it == value)
			{
				imp_move_erased(it, { nodes, nodes.size() - 1 - erased_amount });
				++erased_amount;
			}
		}

		nodes.erase(nodes.end() - erased_amount, nodes.end());

		if (*it == value)
		{
			pop_back();
		}

		return;
	}

	template<typename ty>
	inline contiguous_doubly_linked_list<ty>::iterator contiguous_doubly_linked_list<ty>::begin()
	{
		return iterator(nodes, front_idx);
	}

	template<typename ty>
	inline contiguous_doubly_linked_list<ty>::iterator contiguous_doubly_linked_list<ty>::end()
	{
		return iterator(nodes, NULLNODE);
	}

	template<typename ty>
	inline contiguous_doubly_linked_list<ty>::const_iterator contiguous_doubly_linked_list<ty>::cbegin() const
	{
		return const_iterator(nodes, front_idx);
	}

	template<typename ty>
	inline contiguous_doubly_linked_list<ty>::const_iterator contiguous_doubly_linked_list<ty>::cend() const
	{
		return const_iterator(nodes, NULLNODE);
	}

	template<typename ty>
	inline contiguous_doubly_linked_list<ty>::reverse_iterator<typename contiguous_doubly_linked_list<ty>::iterator> contiguous_doubly_linked_list<ty>::rbegin()
	{
		return reverse_iterator<iterator>(iterator(nodes, back_idx));
	}

	template<typename ty>
	inline contiguous_doubly_linked_list<ty>::reverse_iterator<typename contiguous_doubly_linked_list<ty>::iterator> contiguous_doubly_linked_list<ty>::rend()
	{
		return reverse_iterator<iterator>(iterator(nodes, NULLNODE));
	}

	template<typename ty>
	inline contiguous_doubly_linked_list<ty>::reverse_iterator<typename contiguous_doubly_linked_list<ty>::const_iterator> contiguous_doubly_linked_list<ty>::crbegin() const
	{ 
		return reverse_iterator<const_iterator>(const_iterator(nodes, back_idx));
	}

	template<typename ty>
	inline contiguous_doubly_linked_list<ty>::reverse_iterator<typename contiguous_doubly_linked_list<ty>::const_iterator> contiguous_doubly_linked_list<ty>::crend() const
	{
		return reverse_iterator<const_iterator>(const_iterator(nodes, NULLNODE));
	}

	template<typename ty>
	inline void contiguous_doubly_linked_list<ty>::imp_move_erased(contiguous_doubly_linked_list<ty>::const_iterator& erased, const contiguous_doubly_linked_list<ty>::const_iterator& dest)
	{
		// order of these matter in case if `back` node is an erased one
		if (dest.next_idx() != NULLNODE)
		{
			nodes[dest.next_idx()].prev = erased.current_idx;
		}
		if (dest.prev_idx() != NULLNODE)
		{
			nodes[dest.prev_idx()].next = erased.current_idx;
		}
		if (erased.next_idx() != NULLNODE)
		{
			nodes[erased.next_idx()].prev = erased.prev_idx();
		}
		if (erased.prev_idx() != NULLNODE)
		{
			nodes[erased.prev_idx()].next = erased.next_idx();
		}

		std::swap(nodes[erased.current_idx], nodes[dest.current_idx]);
		erased.current_idx = dest.current_idx;

		return;
	}
}