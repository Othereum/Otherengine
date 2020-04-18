#pragma once
#include <compare>
#include <iterator>

namespace game::detail
{
	template <class C>
	struct ConstIterator
	{
		using iterator_category = std::contiguous_iterator_tag;
		using value_type = typename C::value_type;
		using difference_type = typename C::difference_type;
		using pointer = typename C::const_pointer;
		using reference = typename C::const_reference;

		constexpr ConstIterator() noexcept = default;

		constexpr explicit operator pointer() const noexcept { return ptr; }

		constexpr reference operator*() const noexcept { return *ptr; }
		constexpr reference operator[](difference_type n) const noexcept { return ptr[n]; }
		constexpr pointer operator->() const noexcept { return ptr; }

		constexpr ConstIterator& operator++() noexcept
		{
			++ptr;
			return *this;
		}

		constexpr ConstIterator operator++(int) noexcept
		{
			ConstIterator it = *this;
			++ptr;
			return it;
		}

		constexpr ConstIterator& operator--() noexcept
		{
			--ptr;
			return *this;
		}

		constexpr ConstIterator operator--(int) noexcept
		{
			ConstIterator it = *this;
			--ptr;
			return it;
		}

		constexpr ConstIterator& operator+=(difference_type n) noexcept
		{
			ptr += n;
			return *this;
		}

		constexpr ConstIterator operator+(difference_type n) const noexcept { return ConstIterator{ptr + n}; }

		constexpr ConstIterator& operator-=(difference_type n) noexcept
		{
			ptr -= n;
			return *this;
		}

		constexpr ConstIterator operator-(difference_type n) const noexcept { return ConstIterator{ptr - n}; }
		constexpr difference_type operator-(const ConstIterator& rhs) const noexcept { return ptr - rhs.ptr; }
		constexpr auto operator<=>(const ConstIterator& it) const noexcept = default;

	protected:
		friend C;
		
		constexpr explicit ConstIterator(typename C::pointer data) noexcept: ptr{data}
		{
		}
		
		typename C::pointer ptr = nullptr;
	};

	template <class C>
	struct Iterator : ConstIterator<C>
	{
		using iterator_category = std::contiguous_iterator_tag;
		using value_type = typename C::value_type;
		using difference_type = typename C::difference_type;
		using pointer = typename C::pointer;
		using reference = typename C::reference;

		constexpr Iterator() noexcept = default;

		constexpr explicit operator pointer() const noexcept { return this->ptr; }

		constexpr reference operator*() const noexcept { return *this->ptr; }
		constexpr reference operator[](difference_type n) const noexcept { return this->ptr[n]; }
		constexpr pointer operator->() const noexcept { return this->ptr; }

		constexpr Iterator& operator++() noexcept
		{
			++this->ptr;
			return *this;
		}

		constexpr Iterator operator++(int) noexcept
		{
			Iterator it = *this;
			++this->ptr;
			return it;
		}

		constexpr Iterator& operator--() noexcept
		{
			--this->ptr;
			return *this;
		}

		constexpr Iterator operator--(int) noexcept
		{
			Iterator it = *this;
			--this->ptr;
			return it;
		}

		constexpr Iterator& operator+=(difference_type n) noexcept
		{
			this->ptr += n;
			return *this;
		}

		constexpr Iterator operator+(difference_type n) const noexcept { return Iterator{this->ptr + n}; }

		constexpr Iterator& operator-=(difference_type n) noexcept
		{
			this->ptr -= n;
			return *this;
		}

		constexpr Iterator operator-(difference_type n) const noexcept { return Iterator{this->ptr - n}; }
		constexpr difference_type operator-(const Iterator& rhs) const noexcept { return this->ptr - rhs.ptr; }
		constexpr auto operator<=>(const Iterator& it) const noexcept = default;

		constexpr Iterator& operator<<(const value_type& v) noexcept { **this = v; return ++*this; }
		constexpr Iterator& operator<<(value_type&& v) noexcept { **this = std::move(v); return ++*this; }
		constexpr Iterator& operator>>(value_type& v) noexcept { v = **this; return ++*this; }

	protected:
		friend C;
		
		constexpr Iterator(pointer data) noexcept: ConstIterator{data}
		{
		}
	};
}
