#include "BFS.h"

#include <queue>

namespace algorithms
{
	BFS::BFS(IAutomaton* automaton)
	{
		this->automaton = automaton;
		visited = new utils::BitSet(automaton->states_count);
	}

	void BFS::run_from(AutomatonStateT state) const
	{
		std::queue<AutomatonStateT> queue;
		queue.push(state);
		visited->set(state, true);

		while (! queue.empty())
		{
			auto current_state = queue.front();
			queue.pop();
			for (auto symbol = 0; symbol < automaton->alphabet_size; symbol++)
			{
				auto new_states = automaton->get_transitions(current_state, symbol);
				for (auto new_state : new_states)
				{
					if (!visited->get(new_state))
					{
						visited->set(new_state, true);
						queue.push(new_state);
					}
				}
			}
		}
	}

	bool BFS::is_all_states_visited() const
	{
		return visited->all();
	}

	BFS::~BFS()
	{
		delete visited;
	}
}
