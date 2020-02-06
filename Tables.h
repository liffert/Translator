#pragma once
#include <map>
#include <string>
#include <vector>

class Tables {
public:
	Tables();
	~Tables();

	int add_identificator(std::string name);
	int add_const(std::string name);


private:

	int Ident_iter = 1001;
	
	int Keywords_iter = 401;
	const int Keywords_rangeEnd = 500;

	int Const_iter = 501;
	const int Const_rangeEnd = 1000;

	std::map <std::string, int> const_values;
	std::map <std::string, int> identificators;
	std::map <std::string, int> keywords;

};