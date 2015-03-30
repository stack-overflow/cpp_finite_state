#ifndef __LEXER_TESTS_H__
#define __LEXER_TESTS_H__

#include "string_token.h"
#include "lexer.h"

#include <string>
#include <iostream>

namespace lexer_tests
{
	enum TOKEN_TYPE : token_id
	{
		TOKEN_ERROR = -1,
		TOKEN_NUMBER = 0,
		TOKEN_OPEN_BRACKET,
		TOKEN_CLOSE_BRACKET,
		TOKEN_OPEN_PAREN,
		TOKEN_CLOSE_PAREN,
		TOKEN_IF,
		TOKEN_WHILE,
		TOKEN_DO,
		TOKEN_SWITCH,
		TOKEN_CASE,
		TOKEN_DELETE,
		TOKEN_RETURN,
		TOKEN_INCLUDE,
		TOKEN_TYPE_DECL,
		TOKEN_OP,
		TOKEN_STAR,
		TOKEN_SEMICOLON,
		TOKEN_STRING_LITERAL,
		TOKEN_ID,
		TOKEN_SPACE,
		TOKEN_NEWLINE
	};

	void simple_c_tokenization()
	{
		lexer le;
		le.add_token(TOKEN_NUMBER, "[0-9]+");
		le.add_token(TOKEN_OPEN_BRACKET, "{");
		le.add_token(TOKEN_CLOSE_BRACKET, "}");
		le.add_token(TOKEN_OPEN_PAREN, "\\(");
		le.add_token(TOKEN_CLOSE_PAREN, "\\)");
		le.add_token(TOKEN_IF, "if");
		le.add_token(TOKEN_WHILE, "while");
		le.add_token(TOKEN_DO, "do");
		le.add_token(TOKEN_SWITCH, "switch");
		le.add_token(TOKEN_CASE, "case");
		le.add_token(TOKEN_DELETE, "delete");
		le.add_token(TOKEN_RETURN, "return");
		le.add_token(TOKEN_INCLUDE, "#include");
		le.add_token(TOKEN_TYPE_DECL, "int|float|char|short|unsigned|long|void|const|bool");
		le.add_token(TOKEN_OP, "++|+|==|!=|=|/|-|<|>|%|!|\\||\\|\\||^");
		le.add_token(TOKEN_STAR, "*");
		le.add_token(TOKEN_SEMICOLON, ";");
		le.add_token(TOKEN_STRING_LITERAL, "\"[a-zA-Z0-9 ]*\"");
		le.add_token(TOKEN_ID, "[A-Za-z_][A-Za-z0-9_]*");
		le.add_token(TOKEN_SPACE, "[ ]+|[	]+|[\n]+");

		le.init();
		le.set_input("while (1) { if(x > 5) { i++; } } \nconst char *str = \"test string literal\" ^ || |");

		std::vector< std::pair<token_id, std::string> > expected = {
			{ TOKEN_WHILE, "while" },
			{ TOKEN_SPACE, " " },
			{ TOKEN_OPEN_PAREN, "(" },
			{ TOKEN_NUMBER, "1" },
			{ TOKEN_CLOSE_PAREN, ")" },
			{ TOKEN_SPACE, " " },
			{ TOKEN_OPEN_BRACKET, "{" },
			{ TOKEN_SPACE, " " },
			{ TOKEN_IF, "if" },
			{ TOKEN_OPEN_PAREN, "(" },
			{ TOKEN_ID, "x" },
			{ TOKEN_SPACE, " " },
			{ TOKEN_OP, ">" },
			{ TOKEN_SPACE, " " },
			{ TOKEN_NUMBER, "5" },
			{ TOKEN_CLOSE_PAREN, ")" },
			{ TOKEN_SPACE, " " },
			{ TOKEN_OPEN_BRACKET, "{" },
			{ TOKEN_SPACE, " " },
			{ TOKEN_ID, "i" },
			{ TOKEN_OP, "++" },
			{ TOKEN_SEMICOLON, ";" },
			{ TOKEN_SPACE, " " },
			{ TOKEN_CLOSE_BRACKET, "}" },
			{ TOKEN_SPACE, " " },
			{ TOKEN_CLOSE_BRACKET, "}" },
			{ TOKEN_SPACE, " " },
			{ TOKEN_SPACE, "\n" },
			{ TOKEN_TYPE_DECL, "const" },
			{ TOKEN_SPACE, " " },
			{ TOKEN_TYPE_DECL, "char" },
			{ TOKEN_SPACE, " " },
			{ TOKEN_STAR, "*" },
			{ TOKEN_ID, "str" },
			{ TOKEN_SPACE, " " },
			{ TOKEN_OP, "=" },
			{ TOKEN_SPACE, " " },
			{ TOKEN_STRING_LITERAL, "\"test string literal\"" },
			{ TOKEN_SPACE, " " },
			{ TOKEN_OP, "^" },
			{ TOKEN_SPACE, " " },
			{ TOKEN_OP, "||" },
			{ TOKEN_SPACE, " " },
			{ TOKEN_OP, "|" },
		};

		size_t i = 0;
		string_token *t = nullptr;
		while ((t = le.get_next_token()) != nullptr)
		{
			assert(t->type == expected[i].first);
			assert(t->name == expected[i].second);

			delete t;
			t = nullptr;

			++i;
		}

		std::cout << "Test OK" << std::endl;
	}
}

#endif