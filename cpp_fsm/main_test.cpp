#include "nfa.h"

#include "nfa_tests.h"
#include "regex_parser_tests.h"
#include "dfa_tests.h"
#include "lexer_tests.h"

#include "lexer.h"
#include "dfa.h"

#include <iostream>
#include <set>
#include <map>

#include <cassert>

int main_test()
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
	
	lexer_tests::simple_c_tokenization();

	return 0;
}
