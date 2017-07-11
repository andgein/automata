#include "AnanichevBravilovAutomaton.h"

#if DEBUG
	#include <cassert>
#endif


/* TODO(andgein): replace with std::list<AnanichevBravilovBlock> */
IAutomaton* AnanichevBravilovAutomaton(const std::list<IAutomaton*>& blocks)
{
	auto states_count = 1;
	for (auto block: blocks)
		states_count += block->states_count;

	auto automaton = new Automaton(states_count, 2);

	/* Add sink state */
	automaton->add_transition(0, 0, 0);
	automaton->add_transition(0, 1, 0);
	
	auto counter = 1;
	auto block_id = 0;
	for (auto block: blocks)
	{
		auto symbol0 = 1 - block_id % 2;
		auto symbol1 = block_id % 2;

		automaton->add_transition(counter, symbol1, counter - 1);
		if (block_id)
			automaton->add_transition(counter - 1, symbol1, counter);		

		/* Copy block to the end of automaton */
		for (auto block_state = 0, new_state = counter; block_state < block->states_count; block_state++, new_state++)
		{
			automaton->add_transitions(new_state, symbol0, block->get_transitions(block_state, 0), counter);
			automaton->add_transitions(new_state, symbol1, block->get_transitions(block_state, 1), counter);
		}
		counter += block->states_count;
		block_id++;
	}
	/* Add last loop */
	automaton->add_transition(states_count - 1, block_id % 2, states_count - 1);

	return automaton;
}

IAutomaton* AnanichevBravilovBlock(size_t states_count)
{
#if DEBUG
	assert(states_count % 3 == 1);
#endif
	auto automaton = new Automaton(states_count, 2);
	for (auto part_start = 0; part_start + 3 < static_cast<int>(states_count); part_start += 3)
	{
		automaton->add_transition(part_start, 0, part_start + 1);
		automaton->add_transition(part_start + 1, 0, part_start + 2);
		automaton->add_transition(part_start + 2, 0, part_start);
		automaton->add_transition(part_start + 1, 1, part_start + 2);
		automaton->add_transition(part_start + 2, 1, part_start + 3);
		automaton->add_transition(part_start + 3, 1, part_start + 1);
	}
	return automaton;
}