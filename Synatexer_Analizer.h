#pragma once
#include <iostream>
#include "Tables.h"
#include "Tree.h"
#include "Lexical_Analizer.h"
#include <sstream>

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
	bool statement(bool& err);
	bool variable_identifier();
	bool procedure_identifier();
	bool asfi();
	bool identifier();
	bool unsigned_integer();

    std::shared_ptr<std::vector<Lexical_Analizer::lexStruct>> lex;
	std::shared_ptr<Tables> tables;

	Tree tree;
    std::vector<Lexical_Analizer::lexStruct>::iterator current;

	std::stringstream error;

	bool succsess = false;
public:

    Synatexer_Analizer(std::shared_ptr<Tables>& Tables, std::vector<Lexical_Analizer::lexStruct>& lex);
	Tree start();

	bool isSuccsess() const;
	std::string getError() const;
};

