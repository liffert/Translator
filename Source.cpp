#include "Tables.h"
#include "Lexical_Analizer.h"
#include "Tables.h"
#include <iostream>

int main() {
	Tables *tables = new Tables();
	Lexical_Analizer lexer(tables);

	auto res = lexer.start("Test.txt");
	for (const auto& iter : res) {
		std::cout << iter.code << "\t" << iter.i << "\t" << iter.j << std::endl;
	}
}