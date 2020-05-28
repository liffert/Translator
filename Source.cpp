#include "Tables.h"
#include "Lexical_Analizer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Synatexer_Analizer.h"
#include "CodeGenerate.h"

struct path {
	std::string folderPath;
	std::string testPath;
	explicit path(std::string testPath);
	std::string get_output_path() const;
	std::string get_input_path() const;
	std::string get_folder_path() const;
};

path::path(std::string testPath) : testPath(testPath) {
	folderPath = testPath;
	char c = folderPath.back();
	while (c != '/' && c != '\\') {
		folderPath.pop_back();
		c = folderPath.back();
	}
}

std::string path::get_output_path() const {
	return folderPath + "generated.txt";
}

std::string path::get_input_path() const {
	return testPath;
}

std::string path::get_folder_path() const {
	return folderPath;
}

int main(int argc, char **args) {

	if (argc == 1) {
		std::cout << "You do not input args return\n";
		return 1;
	}
	using path_t = struct path;
	path_t Path(args[1]);

	Tables* tables = new Tables();
	Lexical_Analizer lexer(tables);

	std::ofstream out(Path.get_output_path());
	auto res = lexer.start(Path.get_input_path());
	//out << lexer.String_result();
	//std::cout << lexer.String_result();
	//std::cout << "\n\n\n";
	tables->print_tables();
	tables->saveTofile(out);

	if (!lexer.isSuccsess()) {
		std::cout << "Lexer had an errors:\n";
		out << "Lexer had an error\n";
		return 1;
	}
	std::shared_ptr<Tables> t = std::make_shared<Tables>(*tables);
	Synatexer_Analizer temp(t, res);
	auto tree = temp.start();
	tree.pretty_print(*tables);

	tree.prettySaveToFile(out, *tables);
	if (!temp.isSuccsess()) {
		std::cout << "Syntaxer had an error:\n";
		std::cout << temp.getError();
		out << "Syntaxer had an error:\n" << temp.getError();
		return 1;
	}
	CodeGenerate code(*tables, tree, Path.get_folder_path());
	code.start(tree.get_head());
	std::cout << code.getResult();
	code.saveToFile(out);

	if (!code.isSuccess()) {
		std::cout << "\n\n\nCode generator had an errors:\n";
		std::string err = code.getError();
		std::cout << err;
		out << "\n\n\nCode generator had an errors:\n";
		out << err;
		return 1;
	}
	out.close();
}
