#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <string>

typedef int token_id;

struct string_token
{
	string_token(token_id &&in_type, std::string &&in_name) :
		type(std::move(in_type)),
		name(std::move(in_name)),
		start_pos(-1),
		end_pos(-1)
	{}

	string_token(const token_id &in_type, const std::string &in_name) :
		type(in_type),
		name(in_name),
		start_pos(-1),
		end_pos(-1)
	{}
	string_token(token_id &&in_type, std::string &&in_name, int in_start_pos, int in_end_pos) :
		type(std::move(in_type)),
		name(std::move(in_name)),
		start_pos(in_start_pos),
		end_pos(in_end_pos)
	{}

	string_token(const token_id &in_type, const std::string &in_name, int in_start_pos, int in_end_pos) :
		type(in_type),
		name(in_name),
		start_pos(in_start_pos),
		end_pos(in_end_pos)
	{}

	token_id type;
	std::string name;
	int start_pos;
	int end_pos;

	static const int token_unknown = -1;
	static const int token_first_valid = 0;
};

#endif