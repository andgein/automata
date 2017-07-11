#pragma once

#include <list>
#if DEBUG
	#include <cassert>
#endif

typedef short AutomatonStateT;
typedef unsigned char AutomatonSymbolT;

class IAutomaton
{
public:
	int states_count;
	int alphabet_size;

	virtual ~IAutomaton()
	{
	}

	virtual void add_transition(AutomatonStateT from, AutomatonSymbolT symbol, AutomatonStateT to) = 0;
	virtual void add_transitions(AutomatonStateT from, AutomatonSymbolT symbol, std::list<AutomatonStateT> to, int add_to_each_state = 0) = 0;
	virtual std::list<AutomatonStateT> get_transitions(AutomatonStateT from, AutomatonSymbolT symbol) = 0;	
	virtual bool is_dfa() = 0;

	void print_as_dot()
	{
		printf("digraph automaton {\n");
		for (auto from = 0; from < states_count; from++)
			for (auto symbol = 0; symbol < alphabet_size; symbol++)
			{
				auto to = get_transitions(from, symbol);
				for (auto t: to)
				{
					printf("%d -> %d [label=\"%c\"];\n", from, t, symbol + 'a');
				}
			}
		printf("}\n");
	}

	void print_for_testas()
	{
		if (!is_dfa())
			return;

		printf("%d %d\n", alphabet_size, states_count);
		for (auto state = 0; state < states_count; state++)
		{
			for (auto symbol = 0; symbol < alphabet_size; symbol++)
			{
				auto to_states = get_transitions(state, symbol);
				printf("%d ", *(to_states.begin()));
			}
			printf("\n");
		}
	}
};

class Automaton : public IAutomaton
{
public:
	std::list<AutomatonStateT>** transitions;

	Automaton(int states_count, int alphabet_size)
	{	
#if DEBUG
		assert(states_count > 0);
		assert(alphabet_size > 0);
#endif
		this->states_count = states_count;
		this->alphabet_size = alphabet_size;

		transitions = new std::list<AutomatonStateT>*[states_count];
		for (auto state = 0; state < states_count; state++)
			transitions[state] = new std::list<AutomatonStateT>[alphabet_size];
	}

	virtual ~Automaton()
	{
		for (auto state = 0; state < states_count; state++)
			delete[] transitions[state];
		delete[] transitions;
	}

	void add_transition(AutomatonStateT from, AutomatonSymbolT symbol, AutomatonStateT to) override
	{
#if DEBUG
		assert(from >= 0 && from < states_count);
		assert(to >= 0 && to < states_count);
		assert(symbol >= 0 && symbol < alphabet_size);
#endif
		transitions[from][symbol].push_back(to);
	}
	
	void add_transitions(AutomatonStateT from, AutomatonSymbolT symbol, std::list<AutomatonStateT> to, int add_to_each_state=0) override
	{
#if DEBUG
		assert(from >= 0 && from < states_count);
		assert(symbol >= 0 && symbol < alphabet_size);
#endif
		for (auto t: to)
		{
			#if DEBUG
				assert(t + add_to_each_state >= 0 && t + add_to_each_state < states_count);
			#endif
			transitions[from][symbol].push_back(t + add_to_each_state);
		}
	}

	std::list<AutomatonStateT> get_transitions(AutomatonStateT from, AutomatonSymbolT symbol) override
	{
#if DEBUG
		assert(from >= 0 && from < states_count);
		assert(symbol >= 0 && symbol < alphabet_size);
#endif
		return transitions[from][symbol];
	}

	bool is_dfa() override
	{
		for (auto state = 0; state < states_count; state++)
			for (auto symbol = 0; symbol < alphabet_size; symbol++)
				if (transitions[state][symbol].size() != 1)
					return false;
		return true;
	}
private:

};