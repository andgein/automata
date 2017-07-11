#pragma once

#include "Automaton.h"
#include "../utils/BitSet.h"

namespace algorithms
{
	class BFS
	{
	public:
		BFS(IAutomaton* automaton);
		void run_from(AutomatonStateT state) const;
		bool is_all_states_visited() const;
		virtual ~BFS();

	private:
		IAutomaton* automaton;
		utils::BitSet* visited;
	};
}
