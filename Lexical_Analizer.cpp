#include "Lexical_Analizer.h"
#include <fstream>
#include <iostream>

Lexical_Analizer::Lexical_Analizer(Tables*& obj) {
	tables = obj;
}

Lexical_Analizer::~Lexical_Analizer() {
}

std::vector<struct Lexical_Analizer::lex> Lexical_Analizer::start(const std::string& file) {
	succsess = false;
	std::ifstream File(file);
	if (!File.is_open()) {
		std::cout << "ERROR file does not exist\n";
		return result;
	}

	std::string buffer;
	int i = 1; int j = 0;
	int symb;
	while (!File.eof()) {
		bool COMMENT_FLAG = false;
		bool ASSEMBLY_INSERT_FILE = false;
		bool MULTI_SEPARATOR = false;
		if (READ_NEXT) { symb = File.get(); j++; }
		else { READ_NEXT = true; }

		if (tables->symb_type(symb) == Tables::states::WORD) {
			int k = 0;
			while (!File.eof()
				&& (tables->symb_type(symb) == Tables::states::WORD || tables->symb_type(symb) == Tables::states::NUMBER)) {

				buffer.push_back(symb);
				k++;
				symb = File.get();
			}
			result.push_back({ tables->add_identificator(buffer), i, j, buffer });
			j = j + k;
			buffer.clear();
		}
		if (tables->symb_type(symb) == Tables::states::NUMBER) {
			int k = 0;
			while (!File.eof()
				&& tables->symb_type(symb) == Tables::states::NUMBER) {

				buffer.push_back(symb);
				k++;
				symb = File.get();
			}
			if (tables->symb_type(symb) == Tables::states::WORD) {
				std::cout << "Illegal const part in line " << i << " column " << j << ": " << symb << " char(" << static_cast<char>(symb) << ")" << std::endl;
				File.close();
				return result;
			}
			result.push_back({ tables->add_const(buffer), i, j, buffer });
			j = j + k;
			buffer.clear();
		}
		if (tables->symb_type(symb) == Tables::states::WHITESPACE) {
			if (symb == 9) {
				j = j + 4;
			}
			else if (symb == 10 || symb == 13) {
				i++;
				j = 0;
			}
		}
		if (tables->symb_type(symb) == Tables::states::SEPARATOR) {
			std::string MaybeMultiSymbol;
			MaybeMultiSymbol.push_back(symb);
			if (tables->maybeMultiSep(MaybeMultiSymbol)) {
				MULTI_SEPARATOR = true;
			}
			else {
				struct lex temp { symb, i, j };
				temp.name.push_back(symb);
				result.push_back(temp);
			}
		}
		if (MULTI_SEPARATOR || tables->symb_type(symb) == Tables::states::MULTI_SEPARATORS) {
			std::string MaybeMultiSymbol;
			MaybeMultiSymbol.push_back(symb);
			if (!File.eof()) {
				symb = File.get();
				MaybeMultiSymbol.push_back(symb);
				if (MaybeMultiSymbol == "(*") {
					COMMENT_FLAG = true;
					j++;
				}
				else if (MaybeMultiSymbol == "($") {
					struct lex temp = { tables->get_multisep(MaybeMultiSymbol), i, j, MaybeMultiSymbol };
					result.push_back(temp);
					ASSEMBLY_INSERT_FILE = true;
					j = j + 2;
				}
				else {
					int k = 1;
					while (tables->maybeMultiSep(MaybeMultiSymbol)) {
						if (!File.eof()) {
							symb = File.get();
						}
						else {break;}
						MaybeMultiSymbol.push_back(symb);
						k++;
					}
					MaybeMultiSymbol.pop_back();
					if (MaybeMultiSymbol.length() > 1) {
						int code = tables->get_multisep(MaybeMultiSymbol);
						if (code == -1) {
							std::cout << "Illegal symbol in line " << i << " column " << j << ": " << *MaybeMultiSymbol.begin() << " char(" << static_cast<char>(*MaybeMultiSymbol.begin()) << ")" << std::endl;
							File.close();
							return result;
						}
						else {
							struct lex temp { code, i, j, MaybeMultiSymbol };
							result.push_back(temp);
							j = j + k;
						}
					}
					else {
						if (tables->symb_type(*MaybeMultiSymbol.begin()) == Tables::states::SEPARATOR) {
							MaybeMultiSymbol = *MaybeMultiSymbol.begin();
							struct lex temp { *MaybeMultiSymbol.begin(), i, j, MaybeMultiSymbol };
							result.push_back(temp);
							READ_NEXT = false;
							j++;
						}
						else {
							std::cout << "Illegal symbol in line " << i << " column " << j << ": " << *MaybeMultiSymbol.begin() << " char(" << static_cast<char>(*MaybeMultiSymbol.begin()) << ")" << std::endl;
							File.close();
							return result;
						}
					}
				}
			}
		}
		if (ASSEMBLY_INSERT_FILE) {
			std::string word;
			int k = 0;
			symb = File.get();
			if (tables->symb_type(symb) != Tables::states::WORD) {
				std::cout << "Assembly insert file error " << i << " column " << j << ": " << symb << " char(" << static_cast<char>(symb) << ") at first" << std::endl;
				File.close();
				return result;
			}
			word.push_back(symb);
			k++;
			while (!File.eof()) {
				symb = File.get();
				if (tables->symb_type(symb) == Tables::states::WORD || tables->symb_type(symb) == Tables::states::NUMBER) {
					word.push_back(symb);
				}
				else {
					if (symb == '$') {
						if (!File.eof()) {
							symb = File.get();
						}
						else { break; }
						if (symb == ')') {
							break;
						}
						else {
							File.close();
							std::cout << "Assembly insert file error '$)' not found in line " << i << " column " << j << ": " << symb << " char(" << static_cast<char>(symb) << ")" << std::endl;
							return result;
						}
					}
					else{
						std::cout << "Assembly insert file error '$)' not found in line " << i << " column " << j << ": " << symb << " char(" << static_cast<char>(symb) << ")" << std::endl;
						File.close();
						return result;
					}
				}
				k++;
			}
			if (File.eof()) {
				std::cout << "Assembly insert file error '$)' not found in line " << i << " column " << j << ": " << symb << " char(" << static_cast<char>(symb) << ")" << std::endl;
				File.close();
				return result;
			}
			else {
				result.push_back({ tables->add_identificator(word), i, j, word });
				j = j + k;
				result.push_back({ tables->get_multisep("$)"), i, j, "$)" });
			}
		}
		if (COMMENT_FLAG) {
			std::string maybeSep;
			while (!File.eof()) {
				symb = File.get();
				j++;
				maybeSep.push_back(symb);
				if (maybeSep == "*)") {
					break;
				}
				else {
					maybeSep = maybeSep.back();
				}
				if (symb == 10 || symb == 13) {
					j = 0;
					i++;
				}
			}
			if (File.eof()) {
				std::cout << "Comment error '*)' not found in line " << i << " column " << j << ": " << symb << " char(" << static_cast<char>(symb) << ")" << std::endl;
				File.close();
				return result;
			}
		}
		if (tables->symb_type(symb) == Tables::states::ERROR) {
			std::cout << "Illegal symbol in line " << i << " column " << j << ": " << symb << " char(" << static_cast<char>(symb) << ")" << std::endl;
			File.close();
			return result;
		}
	}
	File.close();
	succsess = true;
	return result;
}

std::string Lexical_Analizer::String_result() const {
	std::string str;
	for (const auto& iter : result) {
		str = str + std::to_string(iter.code) + "\t" + std::to_string(iter.i) + "\t" + std::to_string(iter.j) + "\t" + iter.name + "\n";
	}
	return str;
}

bool Lexical_Analizer::isSuccsess() const {
	return succsess;
}

