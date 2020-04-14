#pragma once
#include "Tree.h"
#include "Tables.h"
#include <memory>
#include <string>
#include <map>
#include <sstream>

class CodeGenerate {
private:
	std::shared_ptr<Tree> tree;
	std::shared_ptr<Tables> tables;
	int Proc;
	std::vector<int> declared;
	std::map<int, bool> Lables;

	struct in {
		bool parameter_list = false;
		bool statement = false;
		bool LablesDec = false;
		bool GOTO = false;
	}IN;

	std::map<int, std::string> errors;
	std::stringstream ER;
	
public:
	CodeGenerate(Tables& tables, Tree& tree);
	void start(std::shared_ptr<Tree::element> ptr);

	std::string getError();
};
