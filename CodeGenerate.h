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
	std::map<int, std::string> declared;
	std::map<int, bool> Lables;

	struct in {
		bool parameter_list = false;
		bool statement = false;
		bool LablesDec = false;
		bool GOTO = false;
	}IN;

	std::map<int, std::string> errors;
	std::stringstream ER;
	std::stringstream OUT;
	std::string path;
	
	
public:
	CodeGenerate(Tables& tables, Tree& tree, std::string path);
	void start(std::shared_ptr<Tree::element> ptr);

	std::string getError();
	std::string getResult();
	void saveToFile(std::ofstream &out);

	bool isSuccess();
};

