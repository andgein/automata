#pragma once
#include "../algorithms/Automaton.h"

IAutomaton* AnanichevBravilovAutomaton(const std::list<IAutomaton*>& blocks);

IAutomaton* AnanichevBravilovBlock(size_t states_count);
