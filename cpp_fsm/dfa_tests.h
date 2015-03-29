#ifndef __DFA_TESTS___
#define __DFA_TESTS___

#include "dfa.h"

#include <cassert>

namespace dfa_tests
{
	inline
	dfa *create_fie_fee_not_minimal_dfa()
	{
		dfa *d = new dfa;
		d->accept.resize(6, 0);
		d->transitions.resize(6, std::vector<int>(UCHAR_MAX, -1));

		d->start_state = d->new_state();
		int s1 = d->new_state();
		int s2 = d->new_state();
		int s3 = d->new_state();
		int s4 = d->new_state();
		int s5 = d->new_state();
		d->accept[s3] = 1;
		d->accept[s5] = 1;

		d->transitions[d->start_state][(unsigned char)'f'] = s1;
		d->transitions[s1][(unsigned char)'e'] = s2;
		d->transitions[s1][(unsigned char)'i'] = s4;
		d->transitions[s2][(unsigned char)'e'] = s3;
		d->transitions[s4][(unsigned char)'e'] = s5;

		return d;
	}

	void test_simple_not_minimal()
	{
		dfa *d = create_fie_fee_not_minimal_dfa();

		assert(d->run_on_word("fie") == true);
		assert(d->run_on_word("fie") == true);
		assert(d->run_on_word("fei") == false);
		assert(d->run_on_word("fii") == false);

		delete d;
		std::cout << "Test OK" << std::endl;
	}

	void test_simple_minimal()
	{
		dfa *d = create_fie_fee_not_minimal_dfa();
		dfa *min_d = dfa::minimize(d);

		assert(min_d->run_on_word("fie") == true);
		assert(min_d->run_on_word("fie") == true);
		assert(min_d->run_on_word("fei") == false);
		assert(min_d->run_on_word("fii") == false);

		delete min_d;
		delete d;
		std::cout << "Test OK" << std::endl;
	}

	void test_from_nfa()
	{
		nfa n;

		int s0 = n.start_state;
		int s1 = n.new_state();
		int s2 = n.new_state();
		int s3 = n.new_state();
		int s4 = n.new_state();
		n.add_epsilon_transition(s0, s1);
		n.add_transition(s1, s2, 'a');
		n.add_transition(s2, s3, 'l');
		n.add_transition(s3, s4, 'a');
		n.set_accepting(s4);

		int s5 = n.new_state();
		int s6 = n.new_state();
		int s7 = n.new_state();
		n.add_transition(s1, s5, 'a');
		n.add_transition(s5, s6, 'b');
		n.add_transition(s6, s7, 'd');
		n.set_accepting(s7);

		// Init current to start state
		n.init();

		dfa *d = dfa::convert_to_dfa(&n);

		assert(d->run_on_word("ala") == true);
		assert(d->run_on_word("abd") == true);
		assert(d->run_on_word("aaa") == false);
		assert(d->run_on_word("abb") == false);

		delete d;
		std::cout << "Test OK" << std::endl;
	}
}

#endif