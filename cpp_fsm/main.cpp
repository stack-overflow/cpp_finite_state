#include "nfa.h"

#include "nfa_tests.h"
#include "regex_parser_tests.h"
#include "dfa_tests.h"

#include "lexer.h"

#include <iostream>
#include <set>
#include <map>

#include <cassert>

void test_run(dfa *n, const std::string &word)
{
    std::cout << word << ": " << n->run_on_word(word) << std::endl;
    std::cout << "Tokenized:" << std::endl;
    for (const std::string &token : n->get_tokens())
    {
        std::cout << token << std::endl;
    }
    std::cout << "---" << std::endl;
}

#include "dfa.h"

int main()
{
    std::ios_base::sync_with_stdio(false);

	nfa_tests::test_abd();
	nfa_tests::test_rerun_ala();
	nfa_tests::test_negative();
	nfa_tests::test_closure();
	nfa_tests::test_run_on_word();
	nfa_tests::test_run_on_word_negative();
	nfa_tests::test_run_on_word_closure_accept();
	nfa_tests::test_double_closure();

	dfa_tests::test_simple_not_minimal();
	dfa_tests::test_simple_minimal();
	dfa_tests::test_from_nfa();

	regex_parser_tests::test();
	regex_parser_tests::test_alternative();

	{
		lexer le;
		le.add_token("a-d", "[a-d]+");
		le.add_token("e-h", "[e-h]+"); 
		nfa *n = le.build_nfa();
		dfa *d = convert_to_dfa(n);
		dfa *min_d = minimize(d);

		test_run(min_d, "aabbccdd");
		test_run(min_d, "efgh");

		std::cout << "Not minimal num states: " << d->num_states << std::endl;
		std::cout << "Minimal num states: " << min_d->num_states << std::endl;

		delete min_d;
		delete d;
	}
	{
		lexer le;
		le.add_token("single a-d", "[a-d]");
		le.add_token("single e-h", "[e-h]");
		le.add_token("multiple a-d", "[a-d]+");
		le.add_token("multiple e-h", "[e-h]+");
		nfa *n = le.build_nfa();
		dfa *d = convert_to_dfa(n);
		dfa *min_d = minimize(d);

		test_run(min_d, "aaa");
		test_run(min_d, "eef");

		std::cout << "Not minimal num states: " << d->num_states << std::endl;
		std::cout << "Minimal num states: " << min_d->num_states << std::endl;

		delete min_d;
		delete d;
	}
	{
		lexer le;
		le.add_token("space", " ");
		le.add_token("single a-d", "[a-d]");
		le.add_token("single e-h", "[e-h]");
		le.add_token("multiple a-d", "[a-d]+");
		le.add_token("multiple e-h", "[e-h]+");
		le.init("a aabb e eeff");

		std::cout << "Token: " << le.get_next_token() << std::endl;
		std::cout << "Token: " << le.get_next_token() << std::endl;
		std::cout << "Token: " << le.get_next_token() << std::endl;
		std::cout << "Token: " << le.get_next_token() << std::endl;
		std::cout << "Token: " << le.get_next_token() << std::endl;
		std::cout << "Token: " << le.get_next_token() << std::endl;
		std::cout << "Token: " << le.get_next_token() << std::endl;

	}

	return 0;
}
