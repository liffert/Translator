#include "CodeGenerate.h"
#include <iostream>
#include <algorithm>

CodeGenerate::CodeGenerate(Tables& tables, Tree& tree) {
	this->tables = std::make_shared<Tables>(tables);
	this->tree = std::make_shared<Tree>(tree);
}

void CodeGenerate::start(std::shared_ptr<Tree::element> ptr) {
	if (ptr == nullptr) { return; }
	switch (ptr->value.index()) {
	case 0:
		if (std::get<std::string>(ptr->value) == "<procedure-identifier>") {
			Proc = std::get<int>(ptr->childs.at(0)->childs.at(0)->value);
			return;
		}
		else if (std::get<std::string>(ptr->value) == "<parameters-list>") {
			IN.parameter_list = true;
		}
		else if (std::get<std::string>(ptr->value) == "<identifier>") {
			int temp = std::get<int>(ptr->childs.at(0)->value);
			if (Proc == temp) { ER << "Redefine proc name\n"; return; }
			if (IN.parameter_list) {
				if (std::find(declared.begin(), declared.end(), temp) != declared.end()) {
					ER << "Redefine identidier\n";
					return;
				}
				else {
					declared.push_back(temp);
					return;
				}
			}
		}
		else if (std::get<std::string>(ptr->value) == "<block>") {
			IN.parameter_list = false;
			std::cout << "DATA SEGMENT\n";
		}
		else if (std::get<std::string>(ptr->value) == "<statement>") {
			IN.statement = true;
		}
		else if (std::get<std::string>(ptr->value) == "<unsigned-integer>") {
			int temp = std::get<int>(ptr->childs.at(0)->value);
			if (IN.LablesDec) {
				if (Lables.find(temp) != Lables.end()) { ER << "Redefine lable\n"; return; }
				Lables.insert({ temp, false });
				std::cout << tables->getName(temp) << " LABEL ?\n";
				return;
			}
			else {
				if (Lables.find(temp) == Lables.end()) { ER << "Label used but not declared\n"; return; }
				if (Lables[temp]) {
					if (IN.GOTO) {
						std::cout << "jmp " << tables->getName(temp) << std::endl;
						IN.GOTO = false;
						return;
					}
					ER << "LABEL REINIT\n";
					return;
				}
				else {
					if (IN.GOTO) {
						std::cout << "jmp " << tables->getName(temp) << std::endl;
						IN.GOTO = false;
						errors.insert({ temp, "IS used but didn`t init\n" });
						return;
					}
				}
				if (errors.find(temp) != errors.end()) {
					errors.erase(temp);
				}
				Lables[temp] = true;
				std::cout << tables->getName(temp) << ":\n";
				return;
			}
		}
		else if (std::get<std::string>(ptr->value) == "<assembly-insert-file-identifier>") {
			std::ifstream file(tables->getName(std::get<int>(ptr->childs.at(0)->childs.at(0)->value)));
			if (file.is_open()) {
				std::stringstream buf;
				buf << file.rdbuf();
				std::cout << buf.str() << std::endl;
			}
			else {
				ER << "FILE IS NOT EXIST\n";
			}
			return;
		}
		break;
	case 1:
		if (std::get<int>(ptr->value) == 402) {
			std::cout << "DATA ENDS\nCODE SEGMENT\nASSUME CS:CODE DS:DATA\nproc " << tables->getName(Proc) << std::endl;
			IN.LablesDec = false;
		}
		else if (std::get<int>(ptr->value) == 403) {
			std::cout << "leave\n" << tables->getName(Proc) << " ENDP\nCODE ENDS\n";
		}
		else if (std::get<int>(ptr->value) == 404) {
			IN.LablesDec = true;
		}
		else if (std::get<int>(ptr->value) == 405) {
			IN.GOTO = true;
			IN.statement = false;
		}
		else if (std::get<int>(ptr->value) == 406) {
			std::cout << "ret\n";
			IN.statement = false;
		}
		else if (std::get<int>(ptr->value) == ';') {
			if (IN.statement) {
				IN.statement = false;
				std::cout << "nop\n";
			}
		}
		break;
	}

	if (ptr->childs.empty()) { return; }
	for (auto& iter : ptr->childs) {
		start(iter);
	}
}

std::string CodeGenerate::getError() {
	for (auto& iter : errors) {
		ER << iter.second << std::endl;
	}
	return ER.str();
}
