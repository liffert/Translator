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

	for (int i = 0; i < ASCI_count; i++) {
		ASCI[i] = states::ERROR;
	}

	for (int i = 0; i < sep_count; i++) {
		ASCI[separators[i]] = states::SEPARATOR;
	}
	for (int i = 0; i < let_count; i++) {
		ASCI[letters[i]] = states::WORD;
	}
	for (int i = 0; i < num_count; i++) {
		ASCI[numbers[i]] = states::NUMBER;
	}
	for (int i = 8; i <= 13; i++) {
		ASCI[i] = states::WHITESPACE;
	}

	ASCI[32] = states::WHITESPACE;
	ASCI[40] = states::SEPARATOR;
	ASCI[41] = states::SEPARATOR;
}

Tables::~Tables() {
}

int Tables::add_identificator(const std::string& name) {
	auto Kiter = keywords.find(name);
	if (Kiter != keywords.end()) {
		return Kiter->second;
	}
	auto Iiter = identificators.find(name);
	if (Iiter != identificators.end()) {
		return Iiter->second;
	}

	identificators.insert(std::make_pair(name, Ident_iter));
	Ident_iter++;
	return Ident_iter - 1;
}

int Tables::add_const(const std::string& name) {
	auto Citer = const_values.find(name);
	if (Citer != const_values.end()) {
		return Citer->second;
	}
	const_values.insert(std::make_pair(name, Const_iter));
	Const_iter++;
	return Const_iter - 1;
}

int Tables::symb_type(const int symb) const{
	return ASCI[symb];
}
