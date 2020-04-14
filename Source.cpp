#include "Tables.h"
#include "Lexical_Analizer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Synatexer_Analizer.h"
#include "CodeGenerate.h"

struct path {
	std::string project = "./";
	std::string folder = "Test1/";
	std::string input = "input.sig";
	std::string output = "generated.txt";
	bool state;
	std::string get_output_path() const;
	std::string get_input_path() const;
	std::string get_folder_path() const;
	path(bool state) : state(state) {}
};


std::string path::get_output_path() const {
	std::string ret;
	if (state) { ret = project + "Tests/True/" + folder + output; }
	else { ret = project + "Tests/False/" + folder + output; }
	return ret;
}

std::string path::get_input_path() const {
	std::string ret;
	if (state) { ret = project + "Tests/True/" + folder + input; }
	else { ret = project + "Tests/False/" + folder + input; }
	return ret;

}

std::string path::get_folder_path() const {
	std::string ret;
	if (state) { ret = project + "Tests/True/" + folder; }
	else { ret = project + "Tests/False/" + folder; }
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
		return 1;
	}
	CodeGenerate code(*tables, tree, Path.get_folder_path());
	code.start(tree.get_head());

	std::cout << "\n\n\nErrors:\n";
	std::cout << code.getError();
	out.close();
}
