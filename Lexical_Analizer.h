#pragma once
#include "Tables.h"

class Lexical_Analizer {
private:
	Tables* tables;

	struct lex {
		int code;
		int i;
		int j;
		std::string name;
	};
	std::vector<struct lex> result;
	bool whitespace(int symb);
	static const int sep_count = 17;
	char separators[sep_count] = { 58, 59, 61, 33, 34, 35, 36, 37, 38, 39, 40, 41, 43, 44, 45, 46, 47 };
	bool separator(int symb);

public:
	//(symb >= 58 && symb <= 62) || (symb >= 33 && symb <= 47)
	Lexical_Analizer(Tables *& obj);
	~Lexical_Analizer();
	std::vector<struct lex> start(std::string file);
	std::string String_result() const;
};

