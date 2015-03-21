#ifndef __regex_parser_TESTS_H__
#define __regex_parser_TESTS_H__

#include "regex_parser.h"

#include <memory>
#include <iostream>

namespace regex_parser_tests
{
	void print()
	{
		regex_parser parser("a+ (mit|stanford)*[ tyui]*");
		std::unique_ptr<dfa> n = parser.compile();

		std::cout << "Run on ' stanford tyui': " << n->run_on_word(" stanford tyui") << std::endl;
		std::cout << "Run on 'a stanford cool': " << n->run_on_word("a stanford cool") << std::endl;
		std::cout << "Run on 'test': " << n->run_on_word("test") << std::endl;
		std::cout << "Run on 'a mit': " << n->run_on_word("a mit") << std::endl;
		std::cout << "Run on 'a stanford': " << n->run_on_word("a stanford") << std::endl;
		std::cout << "Run on 'a stanfordmit': " << n->run_on_word("a stanfordmit") << std::endl;
		std::cout << "Run on 'a ': " << n->run_on_word("a ") << std::endl;
		std::cout << "Run on 'a t': " << n->run_on_word("a t") << std::endl;
		std::cout << "Run on 'a stanford tyui': " << n->run_on_word("a stanford tyui") << std::endl;
		std::cout << "Run on 'aaaa stanford tyui': " << n->run_on_word("aaaa stanford tyui") << std::endl;

		regex_parser parser2("あ");
        std::unique_ptr<dfa> n2 = parser2.compile();
        std::cout << "あ: " << n2->run_on_word("あ") << std::endl;

		std::cout << "Test OK" << std::endl;
	}

	void test()
	{
		regex_parser parser("a+ (mit|stanford)*[ tyui]*");
		std::unique_ptr<dfa> n = parser.compile();

		assert(n->run_on_word(" stanford tyui") == false);
		assert(n->run_on_word("a stanford cool") == false);
		assert(n->run_on_word("test") == false);
		assert(n->run_on_word("a mit") == true);
		assert(n->run_on_word("a stanford") == true);
		assert(n->run_on_word("a stanfordmit") == true);
		assert(n->run_on_word("a ") == true);
		assert(n->run_on_word("a t") == true);
		assert(n->run_on_word("a stanford tyui") == true);
		assert(n->run_on_word("aaaa stanford tyui") == true);

		regex_parser parser2("あ");
		std::unique_ptr<dfa> n2 = parser2.compile();
		assert(n2->run_on_word("あ") == true);

		std::cout << "Test OK" << std::endl;
	}

	void test_alternative()
	{
		std::cout << "Alternative test" << std::endl;

		regex_parser parser1("[abcd]+");
		std::unique_ptr<dfa> n1 = parser1.compile();
		assert(n1->run_on_word("aabb") == true);
		assert(n1->run_on_word("eeff") == false);

		regex_parser parser2("[e-h]+");
		std::unique_ptr<dfa> n2 = parser2.compile();
		assert(n2->run_on_word("efgh") == true);
		assert(n2->run_on_word("efgheeff") == true);
		assert(n2->run_on_word("aabb") == false);
		assert(n2->run_on_word("aefgh") == false);

		std::cout << "Test OK" << std::endl;
	}
}

#endif
