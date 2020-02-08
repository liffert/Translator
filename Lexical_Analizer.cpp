#include "Lexical_Analizer.h"
#include <fstream>
#include <iostream>
#define is_space(symb) symb != ' '

bool Lexical_Analizer::whitespace(int symb) {
	if (symb == 13 || symb == 10) {
		return true;
	}
	else {
		return false;
	}
}

bool Lexical_Analizer::separator(int symb) {
	for (int i = 0; i < sep_count; i++) {
		if (separators[i] == symb) {
			return true;
		}
	}
	return false;
}

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

		if (tables->letter(symb)) {
			int k = 0;
			while (!File.eof() && is_space(symb) && ! whitespace(symb) && !separator(symb)) {
				buffer.push_back(symb);
				k++;
				symb = File.get();
			}
			struct lex temp {tables->add_identificator(buffer), i, j, buffer};
			result.push_back(temp);
			i = i + k;
			buffer.clear();
		}
		if (tables->number(symb)) {
			int k = 0;
			while (!File.eof() && is_space(symb) && !whitespace(symb) && !separator(symb)) {
				if (tables->letter(symb)) {
					std::cout << "Illegal const part in line " << i << " column " << j << ": " << symb << " char(" << static_cast<char>(symb) << ")" << std::endl;
					return result;
				}
				buffer.push_back(symb);
				k++;
				symb = File.get();
			}
			struct lex temp = { tables->add_const(buffer), i, j, buffer};
			result.push_back(temp);
			i = i + k;
			buffer.clear();
		}
		if (whitespace(symb)) {
			j++;
			i = 0;
		}
		if (tables->separator(symb)) {
			struct lex temp { symb, i, j};
			temp.name.push_back(symb);
			result.push_back(temp);
		}
		else if (separator(symb)) {
			std::cout << "Illegal symbol in line " << i << " column " << j << ": " << symb << " char(" << static_cast<char>(symb) << ")" << std::endl;
			return result;
		}
		//60 62
		if (symb == 42) {
			if (!File.eof()) {
				symb = File.get();
				i++;
				if (symb == 60) {
					while (!File.eof()) {
						symb = File.get();
						i++;
						if (symb == 62) {
							symb = File.get();
							i++;
							if (symb == 42) {
								break;
							}
						}
						if (whitespace(symb)) {
							std::cout << "Comment error *< not found" << std::endl;
							return result;
						}
					}
				}
			}
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
