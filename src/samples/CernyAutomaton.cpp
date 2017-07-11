#include "CernyAutomaton.h"

IAutomaton* CernyAutomaton(size_t states_count)
{
	auto automaton = new Automaton(states_count, 2);
	for (auto state = 0; state < states_count; state++)
	{
		automaton->add_transition(state, 0, state == states_count - 1 ? 0 : state + 1);
		automaton->add_transition(state, 1, state == 0 ? 1 : state);
	}
	return automaton;
}
