#pragma once

#include "Automaton.h"

namespace algorithms
{
	namespace details
	{
		inline AutomatonStateT get_quadric_automaton_state(AutomatonStateT first, AutomatonStateT second, int states_count);
	}

	Automaton* build_square_automaton(IAutomaton* automaton);
}