#ifndef __DFA_H__
#define __DFA_H__

#include "nfa.h"
#include "string_token.h"

#include <vector>
#include <string>

struct dfa
{
	int num_states;
	int start_state;

	std::vector< std::vector<int> > transitions;
	std::vector<int> accept;
	std::unordered_map< int, std::set<token_id> > tokens;

	dfa() :
		num_states(0),
		start_state(-1)
	{}

	std::set<int> get_tokens(int state)
	{
		if (state >= 0)
		{
			return tokens[state];
		}
		else
		{
			return {};
		}
	}

	bool run_on_word(const std::string &word)
	{
		size_t i = 0;
		int current = start_state;

		while (i < word.length() && current != -1)
		{
            unsigned char c = *(unsigned char *)&word[i++];
			current = transitions[current][c];
		}

		return is_accepting(current);
	}

	bool is_accepting(int state)
	{
		return state != -1 && state < (int)accept.size() && accept[state];
	}

	int next(int state, unsigned char c)
	{
		if (state < (int)transitions.size())
		{
			return transitions[state][c];
		}
		return -1;
	}

	int new_state() { return num_states++; }

	static dfa *convert_to_dfa(nfa *n);
	static dfa *minimize(const dfa *const d);
};

#endif
