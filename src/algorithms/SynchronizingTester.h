#pragma once

#include "automaton.h"

class SynchronizingTester
{
public:
	SynchronizingTester();

	bool is_automaton_synchronizing(IAutomaton* automaton);	
	int get_reset_threshold(IAutomaton* automaton);
};
