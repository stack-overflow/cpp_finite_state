#ifndef __LEXER_H__
#define __LEXER_H__

#include "nfa.h"
#include "regex_parser.h"

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
		machine(nullptr)
    {}

	void add_token(const char *token, const char *regex)
	{
		rules.emplace_back(token, regex);
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
		machine = minimize(d);
		machine->init();
		current = machine->start_state;

		input = text;
		pos = 0;
		
		delete d;
		delete n;
	}

	~lexer()
	{
		delete machine;
	}

	std::string get_next_token()
	{
		int cur = current;
		int last_accept_state = -1;
		int last_accept_pos = -1;

		std::string token;
		token.reserve(8);

		while (pos < (int)input.size() &&
			(current = machine->next(current, input[pos])) != -1)
		{
			if (machine->is_accepting(current))
			{
				last_accept_state = current;
				last_accept_pos = pos;
			}
			token += input[pos];
			++pos;
		}

		if (pos < input.size() && last_accept_state != -1)
		{
			pos = last_accept_pos + 1;
			last_accept_pos = last_accept_state = -1;
			current = machine->start_state;
			token = token.substr(0, pos - last_accept_pos);
		}

		return token;
	}

private:
    std::vector<lexer_entry> rules;
	dfa *machine;
	int current;
	std::string input;
	int pos;

};

#endif
