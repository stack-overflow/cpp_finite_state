#ifndef __LEXER_H__
#define __LEXER_H__

#include "nfa.h"
#include "regex_parser.h"
#include "string_token.h"

#include <vector>
#include <algorithm>

struct lexer_entry
{
	lexer_entry(token_id in_name, const char *in_pattern) :
        name(in_name),
        pattern(regex_parser(in_pattern).parse_regex())
    {}

	token_id name;
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

	void add_token(token_id token, const char *regex)
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

	void init()
	{
		nfa *n = build_nfa();
		dfa *d = dfa::convert_to_dfa(n);
		machine = std::unique_ptr<dfa>(dfa::minimize(d));

		current = machine->start_state;

		delete d;
		delete n;
	}

	void set_input(const std::string &text)
	{
		input = text;
		pos = 0;
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
		int started_matching = -1;

		std::string lexeme;
		lexeme.reserve(8);
		bool been_in_loop = false;

		while (pos < (int)input.size() &&
			(current = machine->next(current, input[pos])) != -1)
		{
			if (started_matching == -1)
			{
				started_matching = pos;
			}
			if (machine->is_accepting(current))
			{
				last_accept_state = current;
				last_accept_pos = pos;
			}
			lexeme += input[pos];
			++pos;
			been_in_loop = true;
		}

		string_token *token = nullptr;

		if (last_accept_state != -1)
		{
			token_id final_type = get_token(last_accept_state);
			/* Note that pos is greater by 1 that lexeme's last pos.
			 * This comes from the fact that there is ++pos in aboves loop.
			*/

			int overflow = ((pos - 1) - last_accept_pos);
			int valid_length = lexeme.length() - overflow;

			int end_pos = (pos - 1) - overflow;
			int start_pos = end_pos - (valid_length - 1);

			lexeme = lexeme.substr(0, valid_length);
			token = new string_token(std::move(final_type), std::move(lexeme), start_pos, end_pos);

			current = machine->start_state;
			pos = last_accept_pos + 1;
		}
		else if (started_matching != -1)
		{
			/* There were transition in the DFA and the loop was visited. */
			lexeme = lexeme.substr(0, 1);
			token = new string_token(string_token::token_unknown, std::move(lexeme), started_matching, started_matching);
			
			pos = started_matching + 1;
			current = machine->start_state;
		}
		else if (pos < (int) input.length())
		{
			/* There was no transition on input[pos]. */
			lexeme += input[pos];
			token = new string_token(string_token::token_unknown, std::move(lexeme), pos, pos);

			pos += 1;
			current = machine->start_state;
		}

		return token;
	}

	token_id get_token(int state)
	{
		auto tokens = machine->get_tokens(state);
		if (!tokens.empty())
		{
			return *std::min_element(tokens.begin(), tokens.end());
		}
		else
		{
			return string_token::token_unknown;
		}
	}

private:
    std::vector<lexer_entry> rules;
	std::unique_ptr<dfa> machine;
	int current;
	std::string input;
	int pos;
	int num_priority;
};

#endif
