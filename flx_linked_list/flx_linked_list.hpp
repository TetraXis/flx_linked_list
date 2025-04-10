#pragma once

#include <cassert>
#include <vector>

#include "flx_types.hpp"

/// <summary>
/// This double linked list is based on a std::vector
/// This allows for a cache frindly approach
/// Because std::vector will move data at some point,
/// nodes will use indexes to point to `next` and `prev`.
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

		template<typename ty>
		struct node;

		template<typename ty>
		struct iterator;

		template<typename ty>
		struct const_iterator;

		template<typename iter_ty>
		struct reverse_iterator;

	flx_private:
		u64 front_idx = NULLNODE;
		u64 back_idx = NULLNODE;
		std::vector<node<ty>> nodes{};

	flx_public:

		ty& front();
		ty& back();

		void push_back(const ty&);
		ty pop_back();
		void push_front(const ty&);
		ty pop_front();
		iterator<ty> insert(const const_iterator<ty>&, const ty&);
		iterator<ty> erase(const const_iterator<ty>&);
		iterator<ty> erase(const const_iterator<ty>&, const const_iterator<ty>&);

		bool empty() const;
		void clear();
		u64 size() const;

		void swap(u64, u64);
		void reverse();
		void unique(const ty&);

		iterator<ty> begin();
		iterator<ty> end();

		const_iterator<ty> cbegin() const;
		const_iterator<ty> cend() const;

		reverse_iterator<iterator<ty>> rbegin();
		reverse_iterator<iterator<ty>> rend();

		reverse_iterator<const_iterator<ty>> crbegin();
		reverse_iterator<const_iterator<ty>> crend();


		template<typename ty>
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

		template<typename ty>
		struct iterator
		{
			friend contiguous_double_linked_list;

		flx_private:
			std::vector<node<ty>>& nodes;
			u64 current_idx = NULLNODE;

		flx_public:

			iterator(std::vector<node<ty>>& ref, u64 idx)
				: nodes(ref), current_idx(idx)
			{
			}

			ty& operator * ()
			{
				return nodes[current_idx].data;
			}

			iterator<ty>& operator ++ () // prefix ++
			{
				current_idx = nodes[current_idx].next;
				return *this;
			}

			iterator<ty>& operator -- () // prefix --
			{
				current_idx = nodes[current_idx].prev;
				return *this;
			}

			iterator<ty> operator + (u64 offset) const
			{
				iterator<ty> result(*this);
				while (offset-- > 0)
				{
					++result;
				}
				return result;
			}

			iterator<ty> operator - (u64 offset) const
			{
				iterator<ty> result(*this);
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
				return !(*this == other);
			}
		};

		template<typename ty>
		struct const_iterator
		{
			friend contiguous_double_linked_list;

		flx_private:
			const std::vector<node<ty>>& nodes;
			u64 current_idx = NULLNODE;

		flx_public:

			const_iterator(const std::vector<node<ty>>& ref, u64 idx)
				: nodes(ref), current_idx(idx)
			{
			}

			const ty& operator * () const
			{
				return nodes[current_idx].data;
			}

			iterator<ty>& operator ++ () // prefix ++
			{
				current_idx = nodes[current_idx].next;
				return *this;
			}

			iterator<ty>& operator -- () // prefix --
			{
				current_idx = nodes[current_idx].prev;
				return *this;
			}

			const_iterator<ty> operator + (u64 offset) const
			{
				const_iterator<ty> result(*this);
				while (offset-- > 0)
				{
					++result;
				}
				return result;
			}

			const_iterator<ty> operator - (u64 offset) const
			{
				const_iterator<ty> result(*this);
				while (offset-- > 0)
				{
					--result;
				}
				return result;
			}

			bool operator == (const iterator<ty>& other) const
			{
				return current_idx == other.current_idx && &nodes == &other.nodes;
			}

			bool operator != (const iterator<ty>& other) const
			{
				return !(*this == other);
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

			auto operator * () // "auto" because we dont know iter_ty<ty>'s `ty` here
			{
				return *iter;
			}

			bool operator == (const iter_ty& other) const
			{
				return iter == other;
			}

			bool operator != (const iter_ty& other) const
			{
				return iter != other;
			}
		};
	};


	template<typename ty>
	inline ty& contiguous_double_linked_list<ty>::front()
	{
		return nodes[front_idx];
	}

	template<typename ty>
	inline ty& contiguous_double_linked_list<ty>::back()
	{
		return nodes[back_idx];
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
		assert(back_idx != NULLNODE && "pop_back called on empty list");

		ty value = nodes[back_idx];

		if (size() == 1)
		{
			clear();
			return value;
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
		assert(front_idx != NULLNODE && "pop_front called on empty list");

		ty value = nodes[front_idx];

		if (size() == 1)
		{
			clear();
			return value;
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
	inline contiguous_double_linked_list<ty>::iterator<ty> contiguous_double_linked_list<ty>::insert(const const_iterator<ty>& where, const ty& value)
	{
		assert(where.current_idx < size() && "insert pos is out of bounds");
		assert(where.nodes == nodes && "insert iterator is from other list");
		
		u64 new_idx = nodes.size();
		nodes.emplace_back(value);

		if (where.current_idx == front_idx)
		{
			front_idx = new_idx;
		}

		nodes[new_idx].next = where.current_idx;
		nodes[new_idx].prev = nodes[where.current_idx].prev;
		nodes[nodes[where.current_idx].prev].next = new_idx;
		nodes[where.current_idx].prev = new_idx;

		return iterator<ty>(nodes, new_idx);
	}

	template<typename ty>
	inline contiguous_double_linked_list<ty>::iterator<ty> contiguous_double_linked_list<ty>::erase(const const_iterator<ty>& where)
	{
		assert(size() > 0 && "erase attempt on empty list");
		assert(where.current_idx < size() && "erase pos is out of bounds");
		assert(where.nodes == nodes && "erase iterator is from other list");

		iterator<ty> return_iter(nodes, nodes[where.current_idx].next);

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

		nodes[nodes[where.current_idx].next].prev = nodes[where.current_idx].prev;
		nodes[nodes[where.current_idx].prev].next = nodes[where.current_idx].next;
		std::swap(nodes[where.current_idx], nodes.back());
		nodes.pop_back();

		return return_iter;
	}

	template<typename ty>
	inline contiguous_double_linked_list<ty>::iterator<ty> contiguous_double_linked_list<ty>::erase(const const_iterator<ty>& first, const const_iterator<ty>& last)
	{
		assert(size() > 0 && "erase attempt on empty list");
		assert(first.current_idx < size() && "erase first iterator is out of bounds");
		assert(last.current_idx == NONE || last.current_idx < size() && "erase last iterator is out of bounds");
		assert(first.nodes == nodes && "erase first iterator is from other list");
		assert(last.nodes == nodes && "erase last iterator is from other list");

		u64 pos = first.current_idx;

		if (pos == front_idx && last.current_idx == NONE)
		{
			clear();
			return end();
		}

		u64 distance = 0;
		u64 left_idx = nodes[pos].prev;
		u64 right_idx = last.current_idx;

		while (pos != last.current_idx)
		{
			std::swap(nodes[pos], nodes[nodes.size() - 1 - distance]);
			++distance;
			++pos;
		}

		//TODO: finish edge cases

		// TODO: Check if rbegin is correct here
		nodes.erase(nodes.rbegin() + distance, nodes.end());

		if (first.current_idx == front_idx)
		{

		}
	}

	template<typename ty>
	inline bool contiguous_double_linked_list<ty>::empty() const
	{
		return false;
	}

	template<typename ty>
	inline void contiguous_double_linked_list<ty>::clear()
	{
	}

	template<typename ty>
	inline u64 contiguous_double_linked_list<ty>::size() const
	{
		return u64();
	}

	template<typename ty>
	inline void contiguous_double_linked_list<ty>::swap(u64, u64)
	{
	}

	template<typename ty>
	inline void contiguous_double_linked_list<ty>::reverse()
	{
	}

	template<typename ty>
	inline void contiguous_double_linked_list<ty>::unique(const ty&)
	{
	}

	template<typename ty>
	inline contiguous_double_linked_list<ty>::iterator<ty> contiguous_double_linked_list<ty>::begin()
	{
		return iterator<ty>(nodes, front_idx);
	}

	template<typename ty>
	inline contiguous_double_linked_list<ty>::iterator<ty> contiguous_double_linked_list<ty>::end()
	{
		return iterator<ty>(nodes, NULLNODE);
	}

	template<typename ty>
	inline contiguous_double_linked_list<ty>::const_iterator<ty> contiguous_double_linked_list<ty>::cbegin() const
	{
		return const_iterator<ty>(nodes, front_idx);
	}

	template<typename ty>
	inline contiguous_double_linked_list<ty>::const_iterator<ty> contiguous_double_linked_list<ty>::cend() const
	{
		return const_iterator<ty>(nodes, NULLNODE);
	}

	template<typename ty>
	inline contiguous_double_linked_list<ty>::reverse_iterator<typename contiguous_double_linked_list<ty>::iterator<ty>> contiguous_double_linked_list<ty>::rbegin()
	{
		return reverse_iterator<iterator<ty>>(nodes, back_idx);
	}

	template<typename ty>
	inline contiguous_double_linked_list<ty>::reverse_iterator<typename contiguous_double_linked_list<ty>::iterator<ty>> contiguous_double_linked_list<ty>::rend()
	{
		return reverse_iterator<iterator<ty>>(nodes, NULLNODE);
	}

	template<typename ty>
	inline contiguous_double_linked_list<ty>::reverse_iterator<typename contiguous_double_linked_list<ty>::const_iterator<ty>> contiguous_double_linked_list<ty>::crbegin()
	{
		return reverse_iterator<const_iterator<ty>>(nodes, back_idx);
	}

	template<typename ty>
	inline contiguous_double_linked_list<ty>::reverse_iterator<typename contiguous_double_linked_list<ty>::const_iterator<ty>> contiguous_double_linked_list<ty>::crend()
	{
		return reverse_iterator<const_iterator<ty>>(nodes, NULLNODE);
	}
}