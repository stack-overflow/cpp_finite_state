#include "lexer.h"

#include <iostream>
#include <unordered_map>
#include <map>
#include <string>

enum TOKEN_TYPE : token_id
{
	T_NUMBER = string_token::token_first_valid,
	T_SPACE,
	T_OPEN_BRACKET,
	T_CLOSE_BRACKET,
	T_OPEN_PAREN,
	T_CLOSE_PAREN,
	T_IF,
	T_WHILE,
	T_DO,
	T_SWITCH,
	T_CASE,
	T_DELETE,
	T_RETURN,
	T_INCLUDE,
	T_TYPE_DECL,
	T_OP,
	T_STAR,
	T_DOT,
	T_COMMA,
	T_COLON,
	T_DOUBLE_COLON,
	T_SEMICOLON,
	T_STRING_LITERAL,
	T_ID,
	T_NEWLINE,
	T_OTHER,
	T_UNKNOWN = string_token::token_unknown,
};

std::string t_to_string(token_id t)
{
	switch (t)
	{
	case T_NUMBER: return "T_NUMBER";
	case T_OPEN_BRACKET: return "T_OPEN_BRACKET";
	case T_CLOSE_BRACKET: return "T_CLOSE_BRACKET";
	case T_OPEN_PAREN: return "T_OPEN_PAREN";
	case T_CLOSE_PAREN: return "T_CLOSE_PAREN";
	case T_IF: return "T_IF";
	case T_WHILE: return "T_WHILE";
	case T_DO: return "T_DO";
	case T_SWITCH: return "T_SWITCH";
	case T_CASE: return "T_CASE";
	case T_DELETE: return "T_DELETE";
	case T_RETURN: return "T_RETURN";
	case T_INCLUDE: return "T_INCLUDE";
	case T_TYPE_DECL: return "T_TYPE_DECL";
	case T_OP: return "T_OP";
	case T_STAR: return "T_STAR";
	case T_DOT: return "T_DOT";
	case T_COMMA: return "T_COMMA";
	case T_COLON: return "T_COLON";
	case T_DOUBLE_COLON: return "T_DOUBLE_COLON";
	case T_SEMICOLON: return "T_SEMICOLON";
	case T_STRING_LITERAL: return "T_STRING_LITERAL";
	case T_ID: return "T_ID";
	case T_SPACE: return "T_SPACE";
	case T_NEWLINE: return "T_NEWLINE";
	case T_OTHER: return "T_OTHER";
	case T_UNKNOWN: return "T_UNKNOWN";
	default:
		return "T_UNKNOWN";
	}
}

template <typename ElemType>
class bag
{
public:
	int increase(const ElemType &elem)
	{
		return ++storage[elem];
	}
	bool exists(const ElemType &elem) const
	{
		return storage.find(elem) != storage.end();
	}
	int count(const ElemType &elem)
	{
		storage_type::const_iterator found = storage.find(elem);
		if (found != storage.end())
		{
			return found->second;
		}
		else
		{
			return 0;
		}
	}
private:
	typedef std::unordered_map<ElemType, int> storage_type;
	storage_type storage;
};

int main()
{
	lexer le;
	le.add_token(T_NUMBER, "[0-9]+");
	le.add_token(T_OPEN_BRACKET, "{");
	le.add_token(T_CLOSE_BRACKET, "}");
	le.add_token(T_OPEN_PAREN, "\\(");
	le.add_token(T_CLOSE_PAREN, "\\)");
	le.add_token(T_IF, "if");
	le.add_token(T_WHILE, "while");
	le.add_token(T_DO, "do");
	le.add_token(T_SWITCH, "switch");
	le.add_token(T_CASE, "case");
	le.add_token(T_DELETE, "delete");
	le.add_token(T_RETURN, "return");
	le.add_token(T_INCLUDE, "#include");
	le.add_token(T_TYPE_DECL, "int|float|char|short|unsigned|long|void|const|bool");
	le.add_token(T_OP, "++|+|+=|*=|/=|^=|&=|\\|=|==|!=|=|/|-|<|>|%|!|\\||\\|\\||^|&&|&");
	le.add_token(T_STAR, "*");
	le.add_token(T_DOT, "\\.");
	le.add_token(T_COMMA, ",");
	le.add_token(T_DOUBLE_COLON, "::");
	le.add_token(T_COLON, ":");
	le.add_token(T_SEMICOLON, ";");
	le.add_token(T_STRING_LITERAL, "\"([\\].|[^\"])*\"");
	le.add_token(T_ID, "[A-Za-z_][A-Za-z0-9_]*");
	le.add_token(T_SPACE, "[ ]+|[	]+|[\n]+");

	le.init();

	std::cout << "Built the lexer" << std::endl;

	le.set_input("while (1) { if(x > 5) { i++; } } \nconst char *str = \"test string literal\" ^ || | std::string my_string; my_string.length(); \",\\\".,.,&.,\"~fds");
	//le.set_input("And here I sit - next to an overflowing dumpster screened partially from view but not in the least from aroma by cheap latticework from Home Depot. Or I’m curbside on a city street where every few minutes a bus passes and emits a great sooty plume of diesel exhaust, which gently alights upon my meal like finely ground pepper. Or, perhaps in the saddest tableaux of all, I’m sitting");
	//le.set_input("nadssad dsaddsadad jonjranfj dassadvvvv vdasdsasad");
	bag<std::string> b;

	string_token *t = nullptr;
	while ((t = le.get_next_token()) != nullptr)
	{
		if (t && t->type != T_SPACE)
		{
			std::cout << "Token(" << t_to_string(t->type) << "): '" << t->name << "'" << " start_pos: " << t->start_pos << " end_pos: " << t->end_pos << std::endl;
			b.increase(t->name);
		}
			
		delete t;
		t = nullptr;
	}
	return 0;
}