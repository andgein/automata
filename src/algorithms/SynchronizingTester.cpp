#include "SynchronizingTester.h"

#include <queue>
/* TODO (andgein): It's too bad to include heavy set and map */
#include <map>

#include "BasicAlgorithms.h"
#include "SquareAutomaton.h"
#include "BFS.h"
#include "PowerAutomaton.h"
#include "../utils/SetStorage.h"

SynchronizingTester::SynchronizingTester()
{
	
}

bool SynchronizingTester::is_automaton_synchronizing(IAutomaton* automaton)
{
	if (! automaton->is_dfa())
		return false;

	/* TODO(andgein): Optimization: no need to build full square automaton, half of it will be enough */
	auto square_automaton = algorithms::build_square_automaton(automaton);
	auto reversed_automaton = algorithms::reverse_automaton(square_automaton);

	auto bfs = new algorithms::BFS(reversed_automaton);
	for (auto state = 0; state < automaton->states_count; state++)
	{
		auto quadric_automaton_state = algorithms::details::get_quadric_automaton_state(state, state, automaton->states_count);
		bfs->run_from(quadric_automaton_state);
	}
	auto result = bfs->is_all_states_visited();
	
	delete bfs;
	delete square_automaton;
	delete reversed_automaton;

	return result;
}

bool _bfs_one_step(IAutomaton* automaton,
	std::queue<utils::BitSet*>* bfs_queue,
	std::map<utils::BitSet::InternalIntT, int>& visited,
	std::map<utils::BitSet::InternalIntT, int>& ivisited,
	utils::SetStorage& visited_sets,
	utils::SetStorage& ivisited_sets,
	bool is_complementary)
{
	/* TODO(andgein): reuse BFS.cpp here */
	if (bfs_queue->size() == 0)
	{
		return true;
	}
	auto iterations = bfs_queue->size();
	for (auto iteration = 0; iteration < iterations; iteration++)
	{
		auto power_state = bfs_queue->front();
		bfs_queue->pop();

		for (auto symbol = 0; symbol < automaton->alphabet_size; symbol++)
		{
			auto transition_state = algorithms::get_power_automaton_transition(automaton, power_state, symbol);
			auto transition_state_int = transition_state->to_int();
			if (visited.find(transition_state_int) == visited.end())
			{
				auto transition_state_vector = is_complementary ? transition_state->to_complementary_vector() : transition_state->to_vector();
				if (ivisited_sets.has_superset_of(transition_state_vector))
				{
					/*
					printf("Meet at %lld\n", transition_state_int);
					printf("At left: %lld, at right %d\n", power_state->to_int(), ivisited[transition_state_int]);
					*/
					delete transition_state;
					delete power_state;
					return true;
				}
				visited.insert_or_assign(transition_state_int, power_state->to_int());
				visited_sets.add(is_complementary ? transition_state->to_vector() : transition_state->to_complementary_vector());
				bfs_queue->push(transition_state);
			}
			else
				delete transition_state;
		}
		delete power_state;
	}
	return false;
}

int SynchronizingTester::get_reset_threshold(IAutomaton* automaton)
{
	if (!automaton->is_dfa())
		return -1;

	/* TODO(andgein) While it's not work for large automata */
	if (automaton->states_count > 64)
		return -1;

	auto all_states = utils::BitSet::create_full_bitset(automaton->states_count);
	auto bfs_queue = new std::queue<utils::BitSet*>();
	auto ibfs_queue = new std::queue<utils::BitSet*>();
	auto reversed_automaton = algorithms::reverse_automaton(automaton);

	/* TODO(andgein): Replace set with own tree for storing visited subsets */
	auto visited = std::map<utils::BitSet::InternalIntT, int>();
	auto ivisited = std::map<utils::BitSet::InternalIntT, int>();
	auto visited_sets = utils::SetStorage(automaton->states_count);
	auto ivisited_sets = utils::SetStorage(automaton->states_count);

	/* Initialize BFS and IBFS queues */
	bfs_queue->push(all_states);
	visited.insert_or_assign(all_states->to_int(), -1);

	for (auto state = 0; state < automaton->states_count; state++)
	{
		auto power_state = new utils::BitSet(automaton->states_count);
		power_state->set(state, true);
		ibfs_queue->push(power_state);
		ivisited.insert_or_assign(power_state->to_int(), -1);
	}
	
	int reset_threshold;
	for (reset_threshold = 0;;)
	{
		reset_threshold++;
		if (_bfs_one_step(automaton, bfs_queue, visited, ivisited, visited_sets, ivisited_sets, false))
			break;
		
		reset_threshold++;
		if (_bfs_one_step(reversed_automaton, ibfs_queue, ivisited, visited, ivisited_sets, visited_sets, true))
			break;
	}

	/* Safe delete object from queue */
	while (! bfs_queue->empty())
	{
		delete bfs_queue->front();
		bfs_queue->pop();
	}
	while (!ibfs_queue->empty())
	{
		delete ibfs_queue->front();
		ibfs_queue->pop();
	}

	delete ibfs_queue;
	delete bfs_queue;
	delete reversed_automaton;

	return reset_threshold;
}
