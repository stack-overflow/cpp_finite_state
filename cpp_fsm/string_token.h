#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <string>

struct string_token
{
	string_token(std::string &&in_type, std::string &&in_name) :
		type(std::move(in_type)),
		name(std::move(in_name))
	{}

	string_token(const std::string &in_type, const std::string &in_name) :
		type(in_type),
		name(in_name)
	{}

	std::string type;
	std::string name;
};

#endif