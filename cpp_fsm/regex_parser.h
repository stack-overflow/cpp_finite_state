#ifndef __regex_parser_H__
#define __regex_parser_H__

#include "regex_ast.h"
#include "dfa.h"

#include <list>
#include <memory>

class regex_parser
{
public:
	regex_parser(const std::string &in);
	std::unique_ptr<dfa> compile();
    regex *parse_regex();

private:
	regex *parse_term();
	regex *parse_base();
	regex *parse_atom();
	regex *parse_factor();
	regex *parse_sequence();
	regex *parse_char_alternative();
	regex *parse_dot_alternative();

	bool has_next() { return pos < input.length(); }
    unsigned char next() { return *(unsigned char *)&input[pos++]; }
    unsigned char check() { return *(unsigned char *)&input[pos]; }
    void eat(unsigned char c) {
        unsigned char actual = check();
		if (actual != c)
		{
			throw std::runtime_error("Input does not match\n");
		}
		next();
	}

private:
	std::string input;
	size_t pos;
	nfa *n;
};

#endif
