#pragma once

#include "Automaton.h"
#include "../utils/BitSet.h"

namespace algorithms
{
	utils::BitSet* get_power_automaton_transition(IAutomaton* automaton, utils::BitSet* from, AutomatonSymbolT symbol);
}