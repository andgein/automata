#include "BasicAlgorithms.h"

namespace algorithms
{
	Automaton* reverse_automaton(IAutomaton* automaton)
	{
		auto reversed_automaton = new Automaton(automaton->states_count, automaton->alphabet_size);
		for (auto state = 0; state < automaton->states_count; state++)
			for (auto symbol = 0; symbol < automaton->alphabet_size; symbol++)
			{
				auto to_states = automaton->get_transitions(state, symbol);
				for (auto to_state: to_states)
					reversed_automaton->add_transition(to_state, symbol, state);
			}

		return reversed_automaton;
	}
}