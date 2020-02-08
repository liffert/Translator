#include "Lexical_Analizer.h"
#include <fstream>
#include <iostream>

Lexical_Analizer::Lexical_Analizer(Tables*& obj) {
	tables = obj;
}

Lexical_Analizer::~Lexical_Analizer() {
}

std::vector<struct Lexical_Analizer::lex> Lexical_Analizer::start(std::string file) {
	std::ifstream File(file);
	if (!File.is_open()) {
		std::cout << "ERROR";
		return result;
	}

	std::string buffer;
	int i = 1; int j = 1;
	while (!File.eof()) {
		int symb = File.get();

		if (tables->symb_type(symb) == Tables::states::WORD) {
			int k = 0;
			while (!File.eof() && tables->symb_type(symb) != Tables::states::SPACE
				&& tables->symb_type(symb) != Tables::states::WHITESPACE
				&& tables->symb_type(symb) != Tables::states::ERROR
				&& tables->symb_type(symb) != Tables::states::SEPARATOR
				&& tables->symb_type(symb) != Tables::states::COMMENT) {

				buffer.push_back(symb);
				k++;
				symb = File.get();
			}
			struct lex temp { tables->add_identificator(buffer), i, j, buffer };
			result.push_back(temp);
			i = i + k;
			buffer.clear();
		}
		if (tables->symb_type(symb) == Tables::states::NUMBER) {
			int k = 0;
			while (!File.eof() && tables->symb_type(symb) != Tables::states::SPACE
				&& tables->symb_type(symb) != Tables::states::WHITESPACE
				&& tables->symb_type(symb) != Tables::states::ERROR
				&& tables->symb_type(symb) != Tables::states::SEPARATOR
				&& tables->symb_type(symb) != Tables::states::COMMENT) {

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
			i = i + k;
			buffer.clear();
		}
		if (tables->symb_type(symb) == Tables::states::WHITESPACE) {
			j++;
			i = 0;
		}
		if (tables->symb_type(symb) == Tables::states::SEPARATOR) {
			struct lex temp { symb, i, j };
			temp.name.push_back(symb);
			result.push_back(temp);
		}
		if (tables->symb_type(symb) == Tables::states::COMMENT) {
			if (!File.eof()) {
				symb = File.get();
				i++;
				if (symb == 42) {
					while (!File.eof()) {
						symb = File.get();
						i++;
						if (symb == 42) {
							symb = File.get();
							i++;
							if (symb == 41) {
								break;
							}
						}
						if (tables->symb_type(symb) == Tables::states::WHITESPACE) {
							std::cout << "Comment error '*)' not found in line " << i << " column " << j << ": " << symb << " char(" << static_cast<char>(symb) << ")" << std::endl;
							return result;
						}
					}
				}
				else {
					std::cout << "Illegal symbol, after '(' expected '*' in line " << i << " column " << j << ": " << symb << " char(" << static_cast<char>(symb) << ")" << std::endl;
					return result;
				}
			}
		}
		else if (tables->symb_type(symb) == Tables::states::ERROR) {
			std::cout << "Illegal symbol in line " << i << " column " << j << ": " << symb << " char(" << static_cast<char>(symb) << ")" << std::endl;
			return result;
		}

		i++;
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
