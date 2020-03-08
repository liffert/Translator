#pragma once
#include <iostream>
#include "Tables.h"
#include "Tree.h"
#include "Lexical_Analizer.h"

class Synatexer_Analizer {
private:
	bool program();
	bool block();
	bool declaration();
	bool label_declaration();
	bool label_list();
	bool parameters_list();
	bool identifiers_list();
	bool statements_list();
	bool statement();
	bool variable_identifier();
	bool procedure_identifier();
	bool asfi();
	bool identifier();
	bool unsigned_integer();

	std::shared_ptr<std::vector<struct Lexical_Analizer::lex>> lex;
	std::shared_ptr<Tables> tables;

	Tree tree;
	std::vector<struct Lexical_Analizer::lex>::iterator current;
public:

	Synatexer_Analizer(std::shared_ptr<Tables>& Tables, std::vector<struct Lexical_Analizer::lex>& lex);
	Tree start();
};

