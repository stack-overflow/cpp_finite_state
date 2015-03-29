#include "dfa.h"

#include <queue>
#include <map>
#include <unordered_map>
#include <functional>
#include <vector>

#include <climits>

template<typename TKey, typename TValue, typename TFunc>
TValue get_or_create_with_func(std::map<TKey, TValue>& src, TKey key, TFunc init_func) {
	auto found_range = src.equal_range(key);
	if (found_range.first == found_range.second) {
		return src.insert(found_range.second, { key, init_func() })->second;
	}
	return found_range.first->second;
}

dfa *dfa::convert_to_dfa(nfa *n)
{
	n->init();

	state_set q0 = n->current;
	std::queue<state_set> work_queue;
	std::unordered_set<int> dfa_states;
	std::unordered_set<int> dfa_accept;
	std::unordered_map< int, std::unordered_map<unsigned char, int> > dfa_table;
	std::map<state_set, int> to_dfa_state;

	std::unordered_map< int, std::set<token_id> > dfa_tokens;

	dfa *d = new dfa;

	std::function<int()> new_state = [&]() { return d->new_state(); };

	/* This is mapping of start state. */
	int dfa_start_state = get_or_create_with_func(to_dfa_state, q0, new_state);

	dfa_states.insert(dfa_start_state);
	work_queue.push(q0);

	// Special case, when first state is also accepting
	if (n->is_accepting())
	{
		dfa_accept.insert(dfa_start_state);
	}

	if (!n->get_tokens().empty())
	{
		dfa_tokens[dfa_start_state] = n->get_tokens();
	}

	while (!work_queue.empty())
	{
		state_set q = work_queue.front();
		work_queue.pop();

		for (unsigned char c = 0; c < UCHAR_MAX; ++c)
		{
			n->current = q;
			n->next(c);
			if (!n->current.empty())
			{
				int src = get_or_create_with_func(to_dfa_state, q, new_state);
				int dst = get_or_create_with_func(to_dfa_state, n->current, new_state);
				dfa_table[src][c] = dst;

				if (dfa_states.find(dst) == dfa_states.end())
				{
					dfa_states.insert(dst);
					work_queue.push(n->current);

					if (n->is_accepting())
					{
						dfa_accept.insert(dst);
					}

					auto current_tokens = n->get_tokens();
					if (!current_tokens.empty())
					{
						dfa_tokens[dst] = current_tokens;
					}
				}
			}
		}
	}

	/* Resize DFA's transition and accept tables */
	size_t num_states = dfa_states.size();
	d->transitions.resize(num_states, std::vector<int>(UCHAR_MAX, -1));
	d->accept.resize(num_states, 0);

	/**
	* Construct transition table from std::unordered_map<int, std::unordered_map<unsigned char, int>>
	  It is a map with state as key, and another map as value.
	  It maps state and input character to another state.
	*/
	for (auto &kv : dfa_table)
	{
		for (unsigned char c = 0; c < UCHAR_MAX; ++c)
		{
			auto found = kv.second.find(c);
			if (found != kv.second.end())
			{
				d->transitions[kv.first][c] = found->second;
			}
			else
			{
				d->transitions[kv.first][c] = -1;
			}
		}
	}

	/* Set proper start state */
	d->start_state = dfa_start_state;

	/* Construct accept table. */
	for (size_t s = 0; s < d->accept.size(); ++s)
	{
		if (dfa_accept.find(s) != dfa_accept.end())
		{
			d->accept[s] = 1;
		}
		else
		{
			d->accept[s] = 0;
		}
	}

	/* Construct tokens table */
	for (auto &kv : dfa_tokens)
	{
		d->tokens[kv.first].insert(kv.second.begin(), kv.second.end());
	}

	return d;
}

void extract_initial_partitions(const dfa *const d, std::vector<int> &t)
{
	state_set accept, regular;
	for (size_t i = 0; i < d->accept.size(); ++i)
	{
		t[i] = !d->accept[i];
	}
}

void state_set_split(const dfa *const d, std::vector<int> &t, std::vector<int> &p, size_t &num_part, size_t part)
{
	for (unsigned char c = 0; c < UCHAR_MAX; ++c)
	{
		bool first = true;
		bool c_splits = false;
		int last, last_part;
		std::set<int> last_token;

		for (int s = 0; s < (int)t.size(); ++s)
		{
			if (p[s] == part)
			{
				int next_on_c = d->transitions[s][c];
				if (first == true)
				{
					first = false;
					last = next_on_c;
					last_part = last != -1 ? p[last] : -1;
					auto tokens = d->tokens.find(s);
					if (tokens != d->tokens.end())
						last_token = tokens->second;
				}
				else
				{
					int next_part = next_on_c != -1 ? p[next_on_c] : -1;
					auto tokens = d->tokens.find(s);

					std::set<int> current_token;
					if (tokens != d->tokens.end())
						current_token = tokens->second;

					if ((next_part != last_part) || (current_token != last_token))
					{
						t[s] = num_part;
						c_splits = true;
					}
				}
			}
		}

		if (c_splits)
		{
			++num_part;
			return;
		}
	}
}

dfa *compress_dfa_states(const dfa *const d, const std::vector<int> &new_states, size_t num_new_states)
{
	dfa *new_d = new dfa;
	new_d->transitions.resize(num_new_states, std::vector<int>(UCHAR_MAX, -1));
	new_d->accept.resize(num_new_states, 0);

	for (size_t cur = 0; cur < new_states.size(); ++cur)
	{
		for (unsigned char c = 0; c < UCHAR_MAX; ++c)
		{
			int next_on_c = d->transitions[cur][c];
			int new_cur = new_states[cur];
			if (next_on_c > -1)
			{
				int new_next_on_c = new_states[next_on_c];
				new_d->transitions[new_cur][c] = new_next_on_c;
			}
			if (d->start_state == cur)
			{
				new_d->start_state = new_cur;
			}
			if (d->accept[cur] == 1)
			{
				new_d->accept[new_cur] = 1;
			}

			auto current_tokens = d->tokens.find(cur);
			if (current_tokens != d->tokens.end())
			{
				new_d->tokens[new_cur].insert(current_tokens->second.begin(), current_tokens->second.end());
			}
		}
	}

	new_d->num_states = num_new_states;

	return new_d;
}

dfa *dfa::minimize(const dfa *const d)
{
	std::vector<int> t(d->accept.size(), -1);
	std::vector<int> p(d->accept.size(), -1);
	extract_initial_partitions(d, t);
	size_t num_part = 2;

	while (t != p)
	{
		p = t;

		size_t tmp_num_part = num_part;
		for (size_t i = 0; i < num_part; ++i)
		{
			state_set_split(d, t, p, tmp_num_part, i);
		}
		num_part = tmp_num_part;
	}

	return compress_dfa_states(d, t, num_part);
}