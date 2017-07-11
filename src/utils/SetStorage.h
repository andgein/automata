#pragma once

#include <vector>

/*
	SetStorage is data structure which stores subsets of set {0, ..., size - 1}.
	Max size is 65535.

	SetStorage can be queried for set A is any set in storage is subset of A or is any set is superset of A.

	It's Set Trie.
*/
namespace utils
{
	namespace details
	{
		struct SetStorageNode;

		struct SetStorageNode
		{
			std::vector<SetStorageNode*> children = std::vector<SetStorageNode*>();
			std::vector<uint16_t> children_labels = std::vector<uint16_t>();
			bool is_finished = false;

			~SetStorageNode()
			{
				for (auto child : children)
					delete child;
			}
		};
	}

	class SetStorage
	{
	public:
		SetStorage(size_t set_sizes);
		virtual ~SetStorage();
		void add(std::vector<uint16_t> ordered_subset) const;
		bool has_superset_of(const std::vector<uint16_t> & ordered_set) const;		
	private:
		size_t set_sizes;
		details::SetStorageNode* root;

		static details::SetStorageNode* _ensure_node_exists(details::SetStorageNode* current, uint16_t label);
		static bool _has_superset_of(details::SetStorageNode* current, const std::vector<uint16_t> & ordered_set, size_t start_index);
	};
}

