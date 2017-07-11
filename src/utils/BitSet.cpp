#include "BitSet.h"

#if DEBUG
	#include <cassert>
#endif
#include <cstring>
#include <stdexcept>

namespace utils
{
	inline int count_bits_in_int64(BitSet::InternalIntT value);

	BitSet::BitSet(size_t size, bool initialize_by_false)
		: size(size)
	{
		element_size = sizeof(InternalIntT) * 8;
		data_size = (size + element_size) / element_size;
		data = new InternalIntT[data_size];
		if (initialize_by_false)
			memset(data, 0, sizeof(InternalIntT) * data_size);
	}

	void BitSet::set(size_t index, bool value) const
	{
#if DEBUG
		assert(index >= 0 && index < size);
#endif
		if (value)
			data[index / element_size] |= (static_cast<InternalIntT>(1) << (index % element_size));
		else
			data[index / element_size] &= ~(static_cast<InternalIntT>(1) << (index % element_size));
	}

	/*
	BitSet* BitSet::with(size_t index, bool value)
	{
		set(index, value);
		return this;
	}
	*/

	bool BitSet::get(size_t index) const
	{
#if DEBUG
		assert(index >= 0 && index < size);
#endif

		return static_cast<bool>(
			data[index / element_size] & (static_cast<InternalIntT>(1) << (index % element_size))
			);
	}

	bool BitSet::operator[](size_t index) const
	{
		return get(index);
	}

	int BitSet::count() const
	{
		auto result = 0;
		for (size_t index = 0; index < data_size - 1; index++)
			result += count_bits_in_int64(data[index]);
		for (auto index = element_size * (data_size - 1); index < size; index++)
			if (get(index))
				result++;
		return result;
	}

	BitSet::InternalIntT BitSet::to_int() const
	{
		if (data_size == 0)
			return 0;
		if (data_size > 1)
			throw std::runtime_error("BitSet::to_int(). Can\'t cast BitSet to int: it's too large");
		return data[0];
	}

	std::vector<uint16_t> BitSet::to_vector() const
	{
		std::vector<uint16_t> result;
		for (size_t index = 0; index < size; index++)
			if (get(index))
				result.push_back(index);
		return result;
	}

	std::vector<uint16_t> BitSet::to_complementary_vector() const
	{
		std::vector<uint16_t> result;
		for (size_t index = 0; index < size; index++)
			if (! get(index))
				result.push_back(index);
		return result;
	}

	bool BitSet::all() const
	{
		for (size_t index = 0; index < data_size - 1; index++)
			if (data[index] != ~(static_cast<InternalIntT> (0)))
				return false;
		for (auto index = element_size * (data_size - 1); index < size; index++)
			if (!get(index))
				return false;
		return true;
	}

	bool BitSet::is_subset_of(BitSet* other) const
	{
		if (size != other->size)
			throw std::runtime_error("BitSet::is_subset_of(). Sets should be equal size");

		for (size_t index = 0; index < data_size - 1; index++)
			if (data[index] & other->data[index] != data[index])
				return false;

		return true;
	}

	BitSet::~BitSet()
	{
		delete[] data;
	}

	BitSet* BitSet::create_full_bitset(size_t size)
	{
		auto result = new BitSet(size, false);
		for (size_t index = 0; index < result->data_size - 1; index++)
			result->data[index] = ~(static_cast<InternalIntT> (0));
		result->data[result->data_size - 1] = 0;
		for (auto index = result->element_size * (result->data_size - 1); index < result->size; index++)
			result->set(index, true);
		return result;
	}

	static const unsigned char BitsSetTable256[256] =
	{
#define B2(n) n,     n+1,     n+1,     n+2
#define B4(n) B2(n), B2(n+1), B2(n+1), B2(n+2)
#define B6(n) B4(n), B4(n+1), B4(n+1), B4(n+2)
		B6(0), B6(1), B6(1), B6(2)
	};

	inline int count_bits_in_int64(BitSet::InternalIntT value)
	{
		return BitsSetTable256[value & 0xff] +
			BitsSetTable256[(value >> 8) & 0xff] +
			BitsSetTable256[(value >> 16) & 0xff] +
			BitsSetTable256[(value >> 24) & 0xff] +
			BitsSetTable256[(value >> 32) & 0xff] +
			BitsSetTable256[(value >> 40) & 0xff] +
			BitsSetTable256[(value >> 48) & 0xff] +
			BitsSetTable256[(value >> 56) & 0xff];
	}
}
