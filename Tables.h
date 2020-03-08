#pragma once
#include <map>
#include <string>
#include <vector>

class Tables {
public:
	Tables();
	~Tables();

	int add_identificator(const std::string &name);
	int add_const(const std::string &name);
	int get_multisep(const std::string &name) const;

	enum states {
		WORD,
		NUMBER,
		SEPARATOR,
		WHITESPACE,
		MULTI_SEPARATORS,
		ERROR
	};
	int symb_type(const int symb) const;
	bool maybeMultiSep(const std::string sep) const;
	void print_tables();

	bool isIdentifier(int code) const;

private:

	int Ident_iter = 1001;
	
	int Keywords_iter = 401;
	const int Keywords_rangeEnd = 500;

	int Const_iter = 501;
	const int Const_rangeEnd = 1000;


	std::map <std::string, int> const_values;
	std::map <std::string, int> identificators;
	std::map <std::string, int> keywords;
	std::map <std::string, int> multisymbol_separators;
	static const int sep_count = 3;
	static const int let_count = 26;
	static const int num_count = 10;
	const char separators[sep_count] = {59, 58, 44};
	const char letters[let_count] = { 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90};
	const char numbers[num_count] = { 48, 49, 50, 51, 52, 53, 54, 55, 56, 57 };

	static const int ASCI_count = 128;
	int ASCI[ASCI_count];
};