#include "SquareAutomaton.h"

namespace algorithms {

	namespace details
	{
		inline AutomatonStateT get_quadric_automaton_state(AutomatonStateT first, AutomatonStateT second, int states_count)
		{
			return first * states_count + second;
		}
	}

	Automaton* build_square_automaton(IAutomaton* automaton)
	{
		auto power_automaton = new Automaton(automaton->states_count * automaton->states_count, automaton->alphabet_size);
		for (auto first = 0; first < automaton->states_count; first++)
			for (auto second = 0; second < automaton->states_count; second++)
			{
				auto new_state = details::get_quadric_automaton_state(first, second, automaton->states_count);
				for (auto symbol = 0; symbol < automaton->alphabet_size; symbol++)
				{
					auto first_transitions = automaton->get_transitions(first, symbol);
					auto second_transitions = automaton->get_transitions(second, symbol);
					for (auto first_transition: first_transitions)
						for (auto second_transition: second_transitions)
							power_automaton->add_transition(new_state, symbol,
															details::get_quadric_automaton_state(
																first_transition,
																second_transition,
																automaton->states_count
															)
					);
				}
			}
		return power_automaton;
	}
}