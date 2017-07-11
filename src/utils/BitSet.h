#pragma once
#include <vector>

namespace utils
{
	class BitSet
	{
	public:
		typedef uint64_t InternalIntT;

		BitSet(size_t size, bool initialize_by_false = true);
		virtual ~BitSet();

		void set(size_t index, bool value) const;
		//BitSet* with(size_t index, bool value);
		bool get(size_t index) const;
		bool operator[] (size_t index) const;
		int count() const;
		InternalIntT to_int() const;
		std::vector<uint16_t> to_vector() const;
		std::vector<uint16_t> to_complementary_vector() const;
		bool all() const;
		bool is_subset_of(BitSet* other) const;

		static BitSet* create_full_bitset(size_t size);
	private:
		InternalIntT* data;
		size_t element_size;
		size_t data_size;
		size_t size;
	};
}
