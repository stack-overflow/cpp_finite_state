#ifndef __NFA_TESTS_H__
#define __NFA_TESTS_H__

#include "nfa.h"

#include <iostream>
#include <string>
#include <cassert>

namespace nfa_tests
{
	void test_abd()
	{
		nfa n;

		int s0 = n.start_state;
		int s1 = n.new_state();
		int s2 = n.new_state();
		int s3 = n.new_state();
		n.add_transition(s0, s1, 'a');
		n.add_transition(s1, s2, 'l');
		n.add_transition(s2, s3, 'a');
		n.set_accepting(s3);

		int s4 = n.new_state();
		int s5 = n.new_state();
		int s6 = n.new_state();
		n.add_transition(s0, s4, 'a');
		n.add_transition(s4, s5, 'b');
		n.add_transition(s5, s6, 'd');
		n.set_accepting(s6);

		// Initial state
		n.current = state_set();
		n.add_to_current(s0);

		bool r1 = n.next('a');
		bool r2 = n.next('b');
		bool r3 = n.next('d');

		assert(r1 == true);
		assert(r2 == true);
		assert(r3 == true);
		assert(n.is_accepting() == true);

		std::cout << "Test OK" << std::endl;
	}
	void test_rerun_ala()
	{
		nfa n;

		int s0 = n.new_state();
		int s1 = n.new_state();
		int s2 = n.new_state();
		int s3 = n.new_state();
		n.add_transition(s0, s1, 'a');
		n.add_transition(s1, s2, 'l');
		n.add_transition(s2, s3, 'a');
		n.set_accepting(s3);

		int s4 = n.new_state();
		int s5 = n.new_state();
		int s6 = n.new_state();
		n.add_transition(s0, s4, 'a');
		n.add_transition(s4, s5, 'b');
		n.add_transition(s5, s6, 'd');
		n.set_accepting(s6);

		// Initial state
		n.current = state_set();
		n.add_to_current(s0);

		bool r1 = n.next('a');
		bool r2 = n.next('b');
		bool r3 = n.next('d');

		assert(r1 == true);
		assert(r2 == true);
		assert(r3 == true);
		assert(n.is_accepting() == true);

		n.current = state_set();
		n.add_to_current(s0);

		r1 = n.next('a');
		r2 = n.next('l');
		r3 = n.next('a');

		assert(r1 == true);
		assert(r2 == true);
		assert(r3 == true);
		assert(n.is_accepting() == true);

		std::cout << "Test OK" << std::endl;
	}
	void test_negative()
	{
		nfa n;

		int s0 = n.new_state();
		int s1 = n.new_state();
		int s2 = n.new_state();
		int s3 = n.new_state();
		n.add_transition(s0, s1, 'a');
		n.add_transition(s1, s2, 'l');
		n.add_transition(s2, s3, 'a');
		n.set_accepting(s3);

		int s4 = n.new_state();
		int s5 = n.new_state();
		int s6 = n.new_state();
		n.add_transition(s0, s4, 'a');
		n.add_transition(s4, s5, 'b');
		n.add_transition(s5, s6, 'd');
		n.set_accepting(s6);

		// Initial state
		n.current = state_set();
		n.add_to_current(s0);

		bool r1 = n.next('a');
		bool r2 = n.next('a');
		bool r3 = n.next('a');
		assert(n.is_accepting() == false);

		assert(r1 == true);
		assert(r2 == false);
		assert(r3 == false);

		std::cout << "Test OK" << std::endl;
	}

	void test_closure()
	{
		nfa n;

		int s0 = n.new_state();
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

		// Initial state
		n.current = state_set();
		n.add_to_current(s0);

		bool r1 = n.next('a');
		bool r2 = n.next('b');
		bool r3 = n.next('d');

		assert(r1 == true);
		assert(r2 == true);
		assert(r3 == true);
		assert(n.is_accepting() == true);

		n.current = state_set();
		n.add_to_current(s0);

		r1 = n.next('a');
		r2 = n.next('l');
		r3 = n.next('a');

		assert(r1 == true);
		assert(r2 == true);
		assert(r3 == true);
		assert(n.is_accepting() == true);

		std::cout << "Test OK" << std::endl;
	}

	void test_run_on_word()
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

		// Initial state
		assert(n.run_on_word("ala") == true);
		n.current = state_set();
		n.add_to_current(s0);
		assert(n.run_on_word("abd") == true);

		std::cout << "Test OK" << std::endl;
	}
	void test_run_on_word_negative()
	{
		nfa n;

		int s0 = n.new_state();
		int s1 = n.new_state();
		int s2 = n.new_state();
		int s3 = n.new_state();
		int s4 = n.new_state();
		n.add_epsilon_transition(s0, s1);
		n.add_transition(s1, s2, 'a');
		n.add_transition(s2, s3, 'l');
		n.add_transition(s3, s4, 'a');
		n.set_accepting(s4);

		// Initial state
		n.current = state_set();
		n.add_to_current(s0);

		assert(n.run_on_word("vvv") == false);

		std::cout << "Test OK" << std::endl;
	}
	void test_run_on_word_closure_accept()
	{
		nfa n;

		int s0 = n.start_state;
		int s1 = n.new_state();
		int s2 = n.new_state();
		int s3 = n.new_state();
		int s4 = n.new_state();
		int s5 = n.new_state();
		n.add_epsilon_transition(s0, s1);
		n.add_transition(s1, s2, 'a');
		n.add_transition(s2, s3, 'l');
		n.add_transition(s3, s4, 'a');
		n.add_epsilon_transition(s4, s5);
		n.set_accepting(s5);

		// Initial state
		n.current = state_set();
		n.add_to_current(s0);

		assert(n.run_on_word("ala") == true);

		std::cout << "Test OK" << std::endl;
	}
	void test_double_closure()
	{
		nfa n;

		int s0 = n.start_state;
		int s1 = n.new_state();
		int s2 = n.new_state();
		int s3 = n.new_state();
		int s4 = n.new_state();
		int s5 = n.new_state();
		int s6 = n.new_state();
		n.add_epsilon_transition(s0, s1);
		n.add_transition(s1, s2, 'a');
		n.add_transition(s2, s3, 'l');
		n.add_transition(s3, s4, 'a');
		n.add_epsilon_transition(s4, s5);
		n.add_epsilon_transition(s5, s6);
		n.set_accepting(s6);

		// Initial state
		n.current = state_set();
		n.add_to_current(s0);

		assert(n.run_on_word("ala") == true);

		std::cout << "Test OK" << std::endl;
	}
}

#endif
