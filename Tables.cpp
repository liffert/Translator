#include "Tables.h"
#include <iostream>
#include <iomanip>

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

	ASCI[8] = states::WHITESPACE;
	ASCI[9] = states::WHITESPACE;
	ASCI[10] = states::WHITESPACE;
	ASCI[13] = states::WHITESPACE;
	ASCI[32] = states::WHITESPACE;
	ASCI[40] = states::SEPARATOR;
	ASCI[41] = states::SEPARATOR;

	multisymbol_separators.insert(std::make_pair("($", 201));
	multisymbol_separators.insert(std::make_pair("$)", 202));
	multisymbol_separators.insert(std::make_pair("(*", 203));
	//multisymbol_separators.insert(std::make_pair("-->", 203));
	//ASCI['-'] = states::MULTI_SEPARATORS;
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

int Tables::get_multisep(const std::string& name) const {
	auto a = multisymbol_separators.find(name);
	if (a == multisymbol_separators.end()) {
		return -1;
	}
	else{
		return a->second;
	}
}

int Tables::symb_type(const int symb) const{
	return ASCI[symb];
}

bool Tables::maybeMultiSep(const std::string sep) const {
	for (const auto& iter : multisymbol_separators) {
		std::string F;
		if (iter.first.length() < sep.length()) {
			continue;
		}
		if (iter.first.length() == sep.length()) {
			if (iter.first == sep) {
				return true;
			}
			continue;
		}
		for (int i = 0; i < sep.length(); i++) {
			F = F + iter.first[i];
		}
		if (F == sep) {
			return true;
		}
	}
	return false;
}

void Tables::print_tables() {
	std::cout << "Identificators\n";
	for (const auto& iter : identificators) {
		std::cout << std::setw(15) << iter.first << "\t" << std::setw(15) << iter.second << std::endl;
	}
	std::cout << "\n\nConstants\n";
	for (const auto& iter : const_values) {
		std::cout << std::setw(15) << iter.first << "\t" << std::setw(15) << iter.second << std::endl;
	}
}

bool Tables::isIdentifier(int code) const {
	for (const auto& iter : identificators) {
		if (iter.second == code) {
			return true;
		}
	}
	return false;
}

bool Tables::isConst(int code) const {
	for (const auto& iter : const_values) {
		if (iter.second == code) {
			return true;
		}
	}
	return false;
}

