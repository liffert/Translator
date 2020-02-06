#pragma once
#include "Tables.h"

class Lexical_Analizer {
private:
	Tables* tables;

	struct lex {
		int code;
		int i;
		int j;
	};
	std::vector<struct lex> result;

public:

	Lexical_Analizer(Tables *& obj);
	~Lexical_Analizer();
	std::vector<struct lex> start(std::string file);
	std::string String_result() const;
};

