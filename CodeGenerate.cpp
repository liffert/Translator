#include "CodeGenerate.h"
#include <iostream>
#include <algorithm>

CodeGenerate::CodeGenerate(Tables& tables, Tree& tree, std::string path) : path(path) {
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
			if (Proc == temp) { ER << "Redefine proc name [" << ptr->childs.at(0)->i << ", " << ptr->childs.at(0)->j << "]\n"; return; }
			if (IN.parameter_list) {
				if (declared.find(temp) != declared.end()) {
					ER << "Redefine identidier " + tables->getName(temp) + " [" << ptr->childs.at(0)->i << ", " << ptr->childs.at(0)->j << "]\n";
					return;
				}
				else {
					declared.insert({ temp, "" });
					return;
				}
			}
		}
		else if (std::get<std::string>(ptr->value) == "<block>") {
			int val = 8;
			for (auto iter = declared.rbegin(); iter != declared.rend(); iter++) {
				(*iter).second = "[ebp + " + std::to_string(val) + "]";
				val = val + 4;
			}
			IN.parameter_list = false;
			OUT << "DATA SEGMENT\n";
		}
		else if (std::get<std::string>(ptr->value) == "<statement>") {
			IN.statement = true;
		}
		else if (std::get<std::string>(ptr->value) == "<unsigned-integer>") {
			int temp = std::get<int>(ptr->childs.at(0)->value);
			if (IN.LablesDec) {
				if (Lables.find(temp) != Lables.end()) { ER << "Redefine label " + tables->getName(temp) + " [" << ptr->childs.at(0)->i << ", " << ptr->childs.at(0)->j << "]\n"; return; }
				Lables.insert({ temp, false });
				OUT << tables->getName(temp) << " LABEL ?\n";
				return;
			}
			else {
				if (Lables.find(temp) == Lables.end()) { ER << "Label " + tables->getName(temp) + " used but didn`t define [" << ptr->childs.at(0)->i << ", " << ptr->childs.at(0)->j << "]\n"; return; }
				if (Lables[temp]) {
					if (IN.GOTO) {
						OUT << "jmp " << tables->getName(temp) << std::endl;
						IN.GOTO = false;
						return;
					}
					ER << "LABEL REINIT [" << ptr->childs.at(0)->i << ", " << ptr->childs.at(0)->j << "]\n";
					return;
				}
				else {
					if (IN.GOTO) {
						OUT << "jmp " << tables->getName(temp) << std::endl;
						IN.GOTO = false;
						errors.insert({ temp, "Label " + tables->getName(temp) + " used but didn`t init [" + std::to_string(ptr->childs.at(0)->i) + ", " + std::to_string(ptr->childs.at(0)->j) + "]\n" });
						return;
					}
				}
				if (errors.find(temp) != errors.end()) {
					errors.erase(temp);
				}
				Lables[temp] = true;
				OUT << tables->getName(temp) << ":\n";
				return;
			}
		}
		else if (std::get<std::string>(ptr->value) == "<assembly-insert-file-identifier>") {
			std::ifstream file(path + tables->getName(std::get<int>(ptr->childs.at(0)->childs.at(0)->value)));
			if (file.is_open()) {
				std::stringstream buf;
				buf << file.rdbuf();
				OUT << buf.str() << std::endl;
			}
			else {
				ER << "FILE IS NOT EXIST [" << ptr->childs.at(0)->childs.at(0)->i << ", " << ptr->childs.at(0)->childs.at(0)->j << "]\n";;
			}
			return;
		}
		else if (std::get<std::string>(ptr->value) == "<statement-list>") {
			if (ptr->childs.at(0)->value.index() == 0) {
				if (std::get<std::string>(ptr->childs.at(0)->value) == "<empty>") {
					OUT << "nop\n";
				}
			}
		}
		break;
	case 1:
		if (std::get<int>(ptr->value) == 402) {
			OUT << "DATA ENDS\nCODE SEGMENT\nASSUME CS:CODE DS:DATA\n" << tables->getName(Proc) << " proc\npop eax\n";
			for (auto iter = declared.rbegin(); iter != declared.rend(); iter++) {
				OUT << tables->getName((*iter).first) << " DW ?\n" << "POP " << tables->getName((*iter).first) << std::endl;
			}
			IN.LablesDec = false;
		}
		else if (std::get<int>(ptr->value) == 403) {
			OUT << "push eax\nret\n" << tables->getName(Proc) << " ENDP\nCODE ENDS\n";
		}
		else if (std::get<int>(ptr->value) == 404) {
			IN.LablesDec = true;
		}
		else if (std::get<int>(ptr->value) == 405) {
			IN.GOTO = true;
		}
		else if (std::get<int>(ptr->value) == 406) {
			OUT << "push eax\nret\n";
		}
		else if (std::get<int>(ptr->value) == ';') {
			if (IN.statement) {
				IN.statement = false;
				OUT << "nop\n";
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

std::string CodeGenerate::getResult() {
	return OUT.str();
}

void CodeGenerate::saveToFile(std::ofstream &out) {
	out << OUT.str();
}

bool CodeGenerate::isSuccess() {
	if (ER.str().empty()) {
		return true;
	}
	return false;
}
