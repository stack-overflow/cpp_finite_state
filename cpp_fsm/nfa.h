#ifndef __NFA_H__
#define __NFA_H__

#include "string_token.h"

#include <map>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <vector>

typedef std::set<int> state_set;
typedef std::map<unsigned char, state_set> transition_map;
typedef std::pair<int, int> state_pair;

struct nfa
{
	nfa();

	int num_states;
	int start_state;
	std::map<int, transition_map> transitions;
	std::map<int, state_set> epsilon_transitions;
	std::unordered_map<int, token_id> tokens;
	state_set accept;
	state_set current;

	bool next(unsigned char c);
	bool run_on_word(const std::string &word);
	bool is_accepting();

	std::set<token_id> get_tokens()
    {
        std::set<int> ret;
        for (int sub : current)
        {
            auto found = tokens.find(sub);
            if (found != tokens.end())
            {
                ret.insert(found->second);
            }
        }
        return ret;
    }

	int new_state();
	void add_transition(int from, int to, unsigned char input);
	void add_epsilon_transition(int from, int to);
	void set_accepting(int state);
	void add_to_current(int state);
	state_set closure(const state_set &nfa_state);
	void init();
};

#endif
