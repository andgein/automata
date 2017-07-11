#include "PowerAutomaton.h"

namespace algorithms
{
	utils::BitSet* get_power_automaton_transition(IAutomaton* automaton, utils::BitSet* from, AutomatonSymbolT symbol)
	{
		auto result = new utils::BitSet(automaton->states_count);
		for (auto state = 0; state < automaton->states_count; state++)
			if (from->get(state))
			{
				auto transitions = automaton->get_transitions(state, symbol);
				for (auto transition : transitions)
					result->set(transition, true);
			}
		return result;
	}
}
