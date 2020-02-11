#include "Lexical_Analizer.h"
#include <fstream>
#include <iostream>

Lexical_Analizer::Lexical_Analizer(Tables*& obj) {
	tables = obj;
}

Lexical_Analizer::~Lexical_Analizer() {
}

std::vector<struct Lexical_Analizer::lex> Lexical_Analizer::start(const std::string& file) {
	std::ifstream File(file);
	if (!File.is_open()) {
		std::cout << "ERROR";
		return result;
	}

	std::string buffer;
	int i = 1; int j = 1;
	int symb;
	while (!File.eof()) {
		bool COMMENT_FLAG = false;
		if (READ_NEXT) { symb = File.get(); }
		else { READ_NEXT = true; }

		if (tables->symb_type(symb) == Tables::states::WORD) {
			int k = 0;
			while (!File.eof()
				&& (tables->symb_type(symb) == Tables::states::WORD || tables->symb_type(symb) == Tables::states::NUMBER)) {

				buffer.push_back(symb);
				k++;
				symb = File.get();
			}
			struct lex temp { tables->add_identificator(buffer), i, j, buffer };
			result.push_back(temp);
			j = j + k;
			buffer.clear();
		}
		if (tables->symb_type(symb) == Tables::states::NUMBER) {
			int k = 0;
			while (!File.eof()
				&& tables->symb_type(symb) == Tables::states::NUMBER) {

				if (tables->symb_type(symb) == Tables::states::WORD) {
					std::cout << "Illegal const part in line " << i << " column " << j << ": " << symb << " char(" << static_cast<char>(symb) << ")" << std::endl;
					return result;
				}
				buffer.push_back(symb);
				k++;
				symb = File.get();
			}
			struct lex temp = { tables->add_const(buffer), i, j, buffer };
			result.push_back(temp);
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
			if (!File.eof()) {
				int buf = symb;
				symb = File.get();
				if (symb == 42) {
					COMMENT_FLAG = true;
					j++;
				}
				//else if (symb == '$') {}
				else {
					struct lex temp { buf, i, j };
					temp.name.push_back(buf);
					result.push_back(temp);
					READ_NEXT = false;
				}
			}
		}
		if (COMMENT_FLAG) {
			if (!File.eof()) {
				symb = File.get();
				j++;
				while (!File.eof()) {
					symb = File.get();
					j++;
					if (symb == 42) {
						symb = File.get();
						j++;
						if (symb == 41) {
							break;
						}
					}
					if (symb == 13 || symb == 10) {
						std::cout << "Comment error '*)' not found in line " << i << " column " << j << ": " << symb << " char(" << static_cast<char>(symb) << ")" << std::endl;
						return result;
					}
				}
				if (File.eof()) {
					std::cout << "Comment error '*)' not found in line " << i << " column " << j << ": " << symb << " char(" << static_cast<char>(symb) << ")" << std::endl;
					return result;
				}
			}
		}
		else if (tables->symb_type(symb) == Tables::states::ERROR) {
			std::cout << "Illegal symbol in line " << i << " column " << j << ": " << symb << " char(" << static_cast<char>(symb) << ")" << std::endl;
			return result;
		}
		j++;
	}



	return result;
}

std::string Lexical_Analizer::String_result() const {
	std::string str;
	for (const auto& iter : result) {
		str = str + std::to_string(iter.code) + "\t" + std::to_string(iter.i) + "\t" + std::to_string(iter.j) + "\t" + iter.name + "\n";
	}
	return str;
}
