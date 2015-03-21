#include "nfa.h"

#include <cassert>

nfa::nfa() :
	num_states(0),
	start_state(num_states++)
{}

int nfa::new_state()
{
	return num_states++;
}

bool nfa::next(unsigned char c)
{
	state_set next_state;
	for (int cur : current)
	{
		// Get transitions for give sub-state of current NFA state
		const state_set closure_substates = closure(transitions[cur][c]);
		next_state.insert(closure_substates.begin(), closure_substates.end());
	}
	current = next_state;

	return current.size() > 0;
}

void nfa::add_transition(int from, int to, unsigned char input)
{
	assert(from >= 0 && from < num_states);
	assert(to >= 0 && to < num_states);
	assert(input >= 0);

	transitions[from][input].insert(to);
}

void nfa::add_epsilon_transition(int from, int to)
{
	assert(from >= 0 && from < num_states);
	assert(to >= 0 && to < num_states);

	epsilon_transitions[from].insert(to);
}

void nfa::set_accepting(int state)
{
	accept.insert(state);
}

void nfa::add_to_current(int state)
{
	state_set closure_current = closure(epsilon_transitions[state]);
	current.insert(state);
	current.insert(closure_current.begin(), closure_current.end());
}

state_set nfa::closure(const state_set &nfa_states)
{
	state_set closure_states;
	for (int sub : nfa_states)
	{
		const state_set &closure_sub = epsilon_transitions[sub];
		state_set closure_moves = closure(closure_sub);

		closure_states.insert(sub);
		closure_states.insert(closure_moves.begin(), closure_moves.end());
	}
	return closure_states;
}

bool nfa::is_accepting()
{
	for (int sub : current)
	{
		if (accept.find(sub) != accept.end())
		{
			return true;
		}
	}
	return false;
}

bool nfa::run_on_word(const std::string &word)
{
	init();
	for (unsigned char c : word)
	{
		if (!next(c))
		{
			return false;
		}
	}
	return is_accepting();
}

void nfa::init()
{
	current = closure({ start_state });
}
