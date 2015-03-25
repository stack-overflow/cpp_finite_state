#include "regex_parser.h"

regex_parser::regex_parser(const std::string &in) :
	input(in),
	pos(0)
{}

std::unique_ptr<dfa> regex_parser::compile()
{
	nfa *n = new nfa;

	regex *re = parse_regex();
	state_pair compiled_re = re->to_nfa(n);
	n->add_epsilon_transition(n->start_state, compiled_re.first);
	n->set_accepting(compiled_re.second);
	n->init();

	dfa *d = convert_to_dfa(n);
	dfa *min_d = minimize(d);

	delete d;
	delete re;
	delete n;

	return std::unique_ptr<dfa>(min_d);
}

regex *regex_parser::parse_regex()
{
	regex *term = parse_term();
	regex *next_regex = nullptr;

	if (has_next() && check() == '|') {
		eat('|');
		next_regex = parse_regex();
	}

	regex *ret = nullptr;;

	if (next_regex)
	{
		alternative *alt = new alternative();
		alt->alt.push_back(term);
		alt->alt.push_back(next_regex);
		ret = alt;
	}
	else
	{
		ret = term;
	}

	return ret;
}

regex *regex_parser::parse_term()
{
	sequence *factors = new sequence;

	while (has_next() && check() != '|' && check() != ')')
	{
		regex *next_factor = parse_factor();
		factors->seq.push_back(next_factor);
	}

	return factors;
}

regex *regex_parser::parse_factor()
{
	regex *factor = parse_base();
	char c = check();
	if (c == '*')
	{
		eat('*');
		factor = new repetition_star(factor);
	}
	else if (c == '+')
	{
		eat('+');
		factor = new repetition_plus(factor);
	}

	return factor;
}

regex *regex_parser::parse_base()
{
	char c = check();
	regex *re = nullptr;

	switch (c)
	{
	case '(':
		eat('(');
		re = parse_regex();
		eat(')');
		break;
	case '\\':
		c = next();
		if (has_next())
		{
			c = next();
		}
		re = new atom(c);
		break;
	case '[':
		eat('[');
		re = parse_char_alternative();
		eat(']');
		break;
	default:
		re = parse_atom();
		break;
	}
	return re;
}

regex *regex_parser::parse_char_alternative()
{
	alternative *alt = new alternative;
	std::string str;

	while (has_next() && check() != ']')
	{
		unsigned char c = next();
		if (c == '-' && has_next() && !str.empty())
		{
			unsigned char end = check();
			eat(end);

			for (unsigned char alt_c = str.back() + 1; alt_c <= end; ++alt_c)
			{
				str += alt_c;
			}
		}
		else
		{
			str += c;
		}
	}

	for (char c : str)
	{
		regex *at = new atom(c);
		alt->alt.push_back(at);
	}

	return alt;
}

regex *regex_parser::parse_atom()
{
    unsigned char c = next();
	atom *ret = new atom(c);
	return ret;
}
