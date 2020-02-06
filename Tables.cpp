#include "Tables.h"
#include <iostream>

Tables::Tables() {
	Ident_iter = 1001;
	Keywords_iter = 401;
	Const_iter = 501;
	keywords.insert(std::make_pair("PROCEDURE", Keywords_iter));
	Keywords_iter++;
	keywords.insert(std::make_pair("BEGIN", Keywords_iter));
	Keywords_iter++;
	keywords.insert(std::make_pair("END", Keywords_iter));
	Keywords_iter++;
	keywords.insert(std::make_pair("LABEL", Keywords_iter));
	Keywords_iter++;
	keywords.insert(std::make_pair("GOTO", Keywords_iter));
	Keywords_iter++;
	keywords.insert(std::make_pair("RETURN", Keywords_iter));
	Keywords_iter++;
}

Tables::~Tables() {
}

int Tables::add_identificator(std::string name) {
	auto Kiter = keywords.find(name);
	if (Kiter != keywords.end()) {
		return Kiter->second;
	}
	auto Citer = const_values.find(name);
	if (Citer != const_values.end()) {
		return Citer->second;
	}
	auto Iiter = identificators.find(name);
	if (Iiter != identificators.end()) {
		return Iiter->second;
	}

	identificators.insert(std::make_pair(name, Ident_iter));
	Ident_iter++;
	return Ident_iter - 1;
}

int Tables::add_const(std::string name) {
	auto Kiter = keywords.find(name);
	if (Kiter != keywords.end()) {
		return Kiter->second;
	}
	return 0;
}
