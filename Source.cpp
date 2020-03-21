#include "Tables.h"
#include "Lexical_Analizer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Synatexer_Analizer.h"

struct path {
	std::string project = "./";
	std::string folder = "Test1/";
	std::string input = "input.sig";
	std::string output = "generated.txt";
	bool state;
	std::string get_output_path() const;
	std::string get_input_path() const;
	path(bool state) : state(state) {}
};


std::string path::get_output_path() const {
	std::string ret;
	if (state) { ret = project + "True/" + folder + output; }
	else { ret = project + "False/" + folder + output; }
	return ret;
}

std::string path::get_input_path() const {
	std::string ret;
	if (state) { ret = project + "True/" + folder + input; }
	else { ret = project + "False/" + folder + input; }
	return ret;

}


int main() {
	using path_t = struct path;
	path_t Path(true);
	
	Tables* tables = new Tables();
	Lexical_Analizer lexer(tables);

	std::ofstream out(Path.get_output_path());
	
	auto res = lexer.start(Path.get_input_path());
	out << lexer.String_result();
	std::cout << lexer.String_result();
	std::cout << "\n\n\n";
	tables->print_tables();

	if (!lexer.isSuccsess()) {
		std::cout << "Lexer had an error\n";
		out << "Lexer had an error\n";
		return 1;
	}
	std::shared_ptr<Tables> t = std::make_shared<Tables>(*tables);
	Synatexer_Analizer temp(t, res);
	auto tree = temp.start();
	tree.print();

	tables->saveTofile(out);
	tree.saveToFile(out);
	if (!temp.isSuccsess()) {
		std::cout << temp.getError();
		out << temp.getError();
	}
	out.close();
}
