#ifndef __LEXER_H__
#define __LEXER_H__

#include "nfa.h"
#include "regex_parser.h"
#include "string_token.h"

#include <vector>

struct lexer_entry
{
    lexer_entry(const char *in_name, const char *in_pattern) :
        name(in_name),
        pattern(regex_parser(in_pattern).parse_regex())
    {}

    const char *name;
    regex *pattern;
};

class lexer
{
public:
    lexer() {}
    lexer(const std::vector<lexer_entry> &in_rules) :
        rules(in_rules),
		machine(nullptr),
		num_priority(0)
    {}

	void add_token(const char *token, const char *regex)
	{
		rules.emplace_back(token, regex);
		priority.insert(std::make_pair(token, num_priority++));
	}

    nfa *build_nfa()
    {
        nfa *n = new nfa;
        std::list<regex *> regexes;

        int final_state = n->new_state();
        n->set_accepting(final_state);

        /* Make an alternative of all entries
         * and set the name of the tokens
         */
        for (lexer_entry &entry : rules)
        {
            state_pair sp = entry.pattern->to_nfa(n);
            n->add_epsilon_transition(n->start_state, sp.first);
            n->add_epsilon_transition(sp.second, final_state);
            n->tokens[sp.second] = entry.name;
        }

        n->init();

        return n;
    }

	void init(const std::string &text)
	{
		nfa *n = build_nfa();
		dfa *d = convert_to_dfa(n);
		machine = std::unique_ptr<dfa>(minimize(d));
		machine->init();
		current = machine->start_state;

		input = text;
		pos = 0;
		
		delete d;
		delete n;
	}

	/*
		Greedy scan algorithm.
		Take the longest matching token.
	*/
	string_token *get_next_token()
	{
		int cur = current;
		int last_accept_state = -1;
		int last_accept_pos = -1;

		std::string lexeme;
		lexeme.reserve(8);

		while (pos < (int)input.size() &&
			(current = machine->next(current, input[pos])) != -1)
		{
			if (machine->is_accepting(current))
			{
				last_accept_state = current;
				last_accept_pos = pos;
			}
			lexeme += input[pos];
			++pos;
		}

		string_token *token = nullptr;

		if (last_accept_state != -1)
		{
			std::string final_type = get_token(last_accept_state);
			pos = last_accept_pos + 1;
			last_accept_pos = last_accept_state = -1;
			current = machine->start_state;
			lexeme = lexeme.substr(0, pos - last_accept_pos);

			token = new string_token(std::move(final_type), std::move(lexeme));
		}

		return token;
	}

	std::string get_token(int state)
	{
		auto types = machine->get_tokens(state);
		std::string final_type = "";
		int min_priority = INT_MAX;
		for (auto type : types)
		{
			auto p = priority[type];
			if (p < min_priority)
			{
				min_priority = p;
				final_type = type;
			}
		}

		return final_type;
	}

private:
    std::vector<lexer_entry> rules;
	std::unique_ptr<dfa> machine;
	std::unordered_map<std::string, int> priority;
	int current;
	std::string input;
	int pos;
	int num_priority;
};

#endif
