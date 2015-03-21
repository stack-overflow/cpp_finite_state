#ifndef __REGEX_AST_H__
#define __REGEX_AST_H__

#include "nfa.h"
#include <list>

struct regex
{
	virtual state_pair to_nfa(nfa *n) = 0;
	virtual ~regex() {};
};

struct atom : public regex
{
	unsigned char value;
	atom(unsigned char c) : value(c) {}
    atom(char c) : value(*(unsigned char *)&c) {}

	virtual state_pair to_nfa(nfa *n)
	{
		int start = n->new_state();
		int end = n->new_state();
		n->add_transition(start, end, value);

		return { start, end };
	}

	virtual ~atom() {}
};

struct sequence : public regex
{
	std::list<regex *> seq;

	virtual state_pair to_nfa(nfa *n)
	{
		int start = -1;
		int last = -1;

		for (regex *re : seq)
		{
			state_pair sp = re->to_nfa(n);
			if (start == -1)
			{
				start = sp.first;
			}
			else
			{
				n->add_epsilon_transition(last, sp.first);
				last = sp.second;
			}
			last = sp.second;
		}
		return{ start, last };
	}

	virtual ~sequence()
	{
		for (regex *re : seq)
		{
			delete re;
		}
	}
};

struct alternative : public regex
{
	std::list<regex *> alt;

	virtual state_pair to_nfa(nfa *n)
	{
		int start = n->new_state();
		int last = n->new_state();

		for (regex *re : alt)
		{
			state_pair sp = re->to_nfa(n);
			n->add_epsilon_transition(start, sp.first);
			n->add_epsilon_transition(sp.second, last);
		}

		return { start, last };
	}

	virtual ~alternative()
	{
		for (regex *re : alt)
		{
			delete re;
		}
	}
};

struct repetition_plus : public regex
{
	regex *base;

	repetition_plus(regex *b) : base(b) {}

	virtual state_pair to_nfa(nfa *n)
	{
		int start = n->new_state();
		int end = n->new_state();
		
		state_pair base_sp = base->to_nfa(n);

		n->add_epsilon_transition(start, base_sp.first);
		n->add_epsilon_transition(base_sp.second, end);
		n->add_epsilon_transition(base_sp.second, base_sp.first);

		return { start, end };
	}

	virtual ~repetition_plus()
	{
		delete base;
	}
};

struct repetition_star : public repetition_plus
{
	repetition_star(regex *b) : repetition_plus(b) {}

	virtual state_pair to_nfa(nfa *n)
	{
		state_pair rep_sp = repetition_plus::to_nfa(n);
		n->add_epsilon_transition(rep_sp.first, rep_sp.second);
		return rep_sp;
	}
};

#endif
