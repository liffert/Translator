#include "Tables.h"
#include "Lexical_Analizer.h"
#include <iostream>
#include <fstream>
#include <sstream>

int main() {
	Tables *tables = new Tables();
	Lexical_Analizer lexer(tables);
	/*std::ofstream f("Test2.txt");
	std::ifstream f2("Test.txt");
	std::stringstream temp; 
	temp << f2.rdbuf();
	f2.close();
	for (int i = 0; i < 100000; i++) {
		f << temp.str() << "\n";
	}
	f.close();*/

	auto res = lexer.start("TestT.txt");
	std::cout << lexer.String_result();
	std::cout << "\n\n\n";
	tables->print_tables();
}