#include "SetStorage.h"


namespace utils
{
	SetStorage::SetStorage(size_t set_sizes) : set_sizes(set_sizes)
	{
		root = new details::SetStorageNode();
	}

	SetStorage::~SetStorage()
	{
		delete root;
	}

	details::SetStorageNode* SetStorage::_ensure_node_exists(details::SetStorageNode* current, uint16_t element)
	{
		for (size_t index = 0; index < current->children_labels.size(); index++)
			if (current->children_labels[index] == element)
				return current->children[index];
		auto new_node = new details::SetStorageNode();
		current->children.push_back(new_node);
		current->children_labels.push_back(element);
		return new_node;
	}

	void SetStorage::add(std::vector<uint16_t> ordered_subset) const
	{
		auto current = root;
		for (auto element : ordered_subset)
			current = _ensure_node_exists(current, element);
		current->is_finished = true;
	}

	bool SetStorage::has_superset_of(const std::vector<uint16_t>& ordered_set) const
	{
		return _has_superset_of(root, ordered_set, 0);
	}

	bool SetStorage::_has_superset_of(details::SetStorageNode* current, const std::vector<uint16_t>& ordered_set, size_t start_index)
	{
		if (start_index >= ordered_set.size())
			return true;

		auto result = false;
		for (size_t index = 0; index < current->children_labels.size() && ! result; index++)
		{
			if (current->children_labels[index] > ordered_set[start_index])
				continue;

			if (current->children_labels[index] == ordered_set[start_index])
				result = result || _has_superset_of(current->children[index], ordered_set, start_index + 1);
			else
				result = result || _has_superset_of(current->children[index], ordered_set, start_index);
		}
		return result;
	}
}
