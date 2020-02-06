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
	int i = 0; int j = 0;
	while (!File.eof()) {
		int symb = File.get();

		if (symb >= 65 && symb <= 90) {
			int k = 0;
			while (!File.eof() && symb != ' ' && symb != ';' && symb != 10 && symb != 13) {
				buffer.push_back(symb);
				k++;
				symb = File.get();
			}
			struct lex temp {tables->add_identificator(buffer), i, j};
			result.push_back(temp);
			i = i + k;
			buffer.clear();
		}
		if (symb == 13 || symb == 10) {
			j++;
			i = -1;
		}
		if ((symb >= 58 && symb <= 62) || (symb >= 33 && symb <= 47)) {
			struct lex temp {symb, i, j};
			result.push_back(temp);
		}

		i++;
	}



	return result;
}

std::string Lexical_Analizer::String_result() const {
	std::string str;
	for (const auto& iter : result) {
		str = str + std::to_string(iter.code) + "\t" + std::to_string(iter.i) + "\t" + std::to_string(iter.j) + "\n";
	}
	return str;
}
