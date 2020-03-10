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
	bool READ_NEXT = true;
	bool succsess = false;

public:
	//(symb >= 58 && symb <= 62) || (symb >= 33 && symb <= 47)
	Lexical_Analizer(Tables *& obj);
	~Lexical_Analizer();
	std::vector<struct lex> start(const std::string& file);
	std::string String_result() const;
	bool isSuccsess() const;
};

