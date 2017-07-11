#include <iostream>

// #define DEBUG 1

#include "algorithms/Automaton.h"
#include "algorithms/SynchronizingTester.h"
#include "samples/CernyAutomaton.h"
#include "samples/AnanichevBravilovAutomaton.h"

#include <vector>

void print_reset_threshold(IAutomaton* automaton)
{
	if (! automaton->is_dfa())
	{
		printf("Automaton is not DFA\n");
		return;
	}

	auto tester = SynchronizingTester();
	auto is_synchronizing = tester.is_automaton_synchronizing(automaton);

	printf(is_synchronizing ? "Synchronized\n" : "Not synchronized\n");
	if (is_synchronizing)
		printf("Reset threshold is %d\n", tester.get_reset_threshold(automaton));
}

void find_cerny_automaton_reset_threshold()
{
	auto automaton = CernyAutomaton(15);
	print_reset_threshold(automaton);
	delete automaton;
}

std::vector<IAutomaton*> ananichevBravilovBlocks = {
	AnanichevBravilovBlock(1),
	AnanichevBravilovBlock(4),
	AnanichevBravilovBlock(7),
	AnanichevBravilovBlock(10),
	AnanichevBravilovBlock(13),
};

auto tester = SynchronizingTester();

void _bruteforce_ananichev_bravilov_automaton(std::list<IAutomaton*>& blocks, int states_left, int& best_reset_threshold)
{
	if (states_left == 0)
	{
		/* TODO(andgein): Don't build automaton again each time, use previous version, 
						  just remove old last block and append new one */
		printf("Try ");
		for (auto block : blocks)
			printf("%d ", block->states_count);
		printf("...\n");

		auto automaton = AnanichevBravilovAutomaton(blocks);
		auto reset_threshold = tester.get_reset_threshold(automaton);
		if (reset_threshold >= best_reset_threshold)
		{
			printf("Found automaton of size %d (%u blocks) with reset threshold %d:\n",
				automaton->states_count,
				blocks.size(),
				reset_threshold
			);
			for (auto block : blocks)
				printf("%d ", block->states_count);
			printf("\n");

			best_reset_threshold = reset_threshold;
		}
		delete automaton;
		return;
	}
	for (auto block: ananichevBravilovBlocks)
	{
		if (block->states_count <= states_left)
		{
			blocks.push_back(block);
			_bruteforce_ananichev_bravilov_automaton(blocks, states_left - block->states_count, best_reset_threshold);
			blocks.pop_back();
		}
		else
			break;
	}
}

void find_best_ananichev_bravilov_automaton(int states_count)
{
	std::list<IAutomaton*> blocks = {
		ananichevBravilovBlocks[0], // 1
		ananichevBravilovBlocks[0], // 1
		ananichevBravilovBlocks[2], // 7
		ananichevBravilovBlocks[1], // 4
	};
	auto already_states = 0;
	for (auto block : blocks)
		already_states += block->states_count;

	auto best_reset_threshold = 0;
	_bruteforce_ananichev_bravilov_automaton(blocks, states_count - 1 - already_states, best_reset_threshold);
}

#include <string>
#include <cstring>
#include "utils/BitSet.h"
#include "algorithms/PowerAutomaton.h"
utils::BitSet* get_word_transition(IAutomaton* automaton, std::string word)
{
	auto current = utils::BitSet::create_full_bitset(automaton->states_count);
	for (auto c: word)
	{
		auto new_current = algorithms::get_power_automaton_transition(automaton, current, c - 'a');
		delete current;
		current = new_current;
	}
	return current;
}

int main()
{
	// find_cerny_automaton_reset_threshold();	

	/*
	// Get reset threshold of Ananichev-Bravilov automaton built by blocks
	auto automaton = AnanichevBravilovAutomaton({blocks[0], blocks[0], blocks[2], blocks[0], blocks[0]});
	print_reset_threshold(automaton);
	delete automaton;
	*/

	
	///*
	// Find bests Ananichev-Bravilov automatons by states number
	auto automaton_size = 24;
	find_best_ananichev_bravilov_automaton(automaton_size);		
	//*/

	/*
	// Check AnanichevBravilovAutomaton from Braviov's paper
	auto automaton = AnanichevBravilovAutomaton({
		ananichevBravilovBlocks[0], // 1
		ananichevBravilovBlocks[0], // 1
		ananichevBravilovBlocks[2], // 7
		ananichevBravilovBlocks[1], // 4
		ananichevBravilovBlocks[0], // 1
		ananichevBravilovBlocks[0], // 1
		ananichevBravilovBlocks[0], // 1
		ananichevBravilovBlocks[0], // 1
		ananichevBravilovBlocks[1], // 4
	
	});
	automaton->print_as_dot();
	char* answer = "ababaaababaabbabaabbababaabbabaabbababaabbababbbababbabbbababbabbbababbaababaaababa";
	printf("%d\n", get_word_transition(automaton, answer)->count());
	printf("Reset threshold is %d\n", tester.get_reset_threshold(automaton));
	printf("Should be %d\n", strlen(answer));
	*/
}
