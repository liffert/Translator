#include "Synatexer_Analizer.h"

Synatexer_Analizer::Synatexer_Analizer(std::shared_ptr<Tables>& tables, std::vector<Lexical_Analizer::lexStruct>& lex) {
	this->lex = std::make_shared<std::vector<Lexical_Analizer::lexStruct>>(lex);
	this->tables = tables;
	current = this->lex->begin();
}

Tree Synatexer_Analizer::start() {
	program();
	return tree;
}

bool Synatexer_Analizer::isSuccsess() const {
	return succsess;
}

std::string Synatexer_Analizer::getError() const {
	return error.str() + "\n";
}

bool Synatexer_Analizer::program() {
	succsess = false;
	if (current->code == 401) {
		tree.add("<program>");
		if(!currentAdd()) {return false; }
		if (!procedure_identifier()) {
			return false;
		}
		tree.backToParent();
		if (!parameters_list()) {
			return false;
		}
		tree.backToParent();
		if (current->code != ';') {
			error << "; expected [" << current->i << ", " << current->j << "]" << std::endl;
			return false;
		}
		tree.add(current->code, current->i, current->j);
		tree.backToParent();
		if(!currentAdd()) {return false; }
		if (!block()) {
			return false;
		}
		if(!currentAdd()) {return false; }
		tree.backToParent();
		tree.backToParent();
		if (current->code != ';') {
			error << "; expected [" << current->i << ", " << current->j << "]" << std::endl;
			return false;
		}
		tree.add(current->code, current->i, current->j);
		tree.backToParent();
		current++;
		if (current == lex->end()) {
			succsess = true;
			return true;
		}
		else {
			error << "Code after END" << std::endl;
		}
	}
	return false;
}

bool Synatexer_Analizer::block() {
	tree.add("<block>");
	if (!declaration()) { return false; }
	tree.backToParent();
	if (current->code == 402) {
		tree.add(current->code, current->i, current->j);
		if(!currentAdd()) {return false; }
		if (!statements_list()) { 
			error << "statement error at [" << current->i << ", " << current->j << "]" << std::endl;
			return false;
		}
		tree.backToParent();
		if (current->code == 403) {
			tree.add(current->code, current->i, current->j);
			return true;
		}
		else {
			error << "END is expected [" << current->i << ", " << current->j << "]" << std::endl;
			return false;
		}
	}
	error << "BEGIN is expected [" << current->i << ", " << current->j << "]" << std::endl;
	return false;
}

bool Synatexer_Analizer::declaration() {
	tree.add("<declaration>");
	if (!label_declaration()) { return false; }
	tree.backToParent();
	return true;
}

bool Synatexer_Analizer::label_declaration() {
	tree.add("<label-declaration>");
	if (current->code == 404) {
		tree.add(current->code, current->i, current->j);
		if(!currentAdd()) {return false; }
		if (unsigned_integer()) {
			tree.add("<unsigned-integer>");
			tree.add(current->code, current->i, current->j);
			tree.backToParent();
			tree.backToParent();
			if(!currentAdd()) {return false; }
			if (!label_list()) { return false; }
			if (current->code == ';') {
				tree.backToParent();
				tree.add(current->code, current->i, current->j);
				if(!currentAdd()) {return false; }
				tree.backToParent();
				return true;
			}
			else {
				error << "; is expected [" << current->i << ", " << current->j << "]" << std::endl;
				return false;
			}
		}
		else {
			error << "unsigned integer expected [" << current->i << ", " << current->j << "]" << std::endl;
			return false;
		}
	}
	else {
		tree.add("<empty>");
		tree.backToParent();
		return true;
	}
}

bool Synatexer_Analizer::label_list() {
	tree.add("<label-list>");
	if (current->code == ',') {
		tree.add(current->code, current->i, current->j);
		tree.backToParent();
		if(!currentAdd()) {return false; }
		if (!unsigned_integer()) {
			error << "unsigned integer expected [" << current->i << ", " << current->j << "]" << std::endl;
			return false;
		}
		else {
			tree.add("<unsigned-integer>");
			tree.add(current->code, current->i, current->j);
			if(!currentAdd()) {return false; }
			tree.backToParent();
			tree.backToParent();
		}
		if (!label_list()) { return false; }
		tree.backToParent();
		return true;
	}
	else {
		tree.add("<empty>");
		tree.backToParent();
		tree.backToParent();
		return true;
	}
}

bool Synatexer_Analizer::parameters_list() {
	tree.add("<parameters-list>");
	if (current->code == '(') {
		tree.add(current->code, current->i, current->j);
		tree.backToParent();
		if(!currentAdd()) {return false; }
		if (!variable_identifier()) {
			error << "variable identifier expected [" << current->i << ", " << current->j << "]" << std::endl;
			return false;
		}
		tree.backToParent();
		if (!identifiers_list()) { return false; }
		tree.backToParent();
		if (current->code == ')') {
			tree.add(current->code, current->i, current->j);
			if(!currentAdd()) {return false; }
			tree.backToParent();
			return true;
		}
		else {
			error << ") is expected [" << current->i << ", " << current->j << "]" << std::endl;
			return false;
		}
	}
	tree.add("<empty>");
	tree.backToParent();
	return true;
}

bool Synatexer_Analizer::identifiers_list() {
	tree.add("<identifiers-list>");
	if (current->code == ',') {
		tree.add(current->code, current->i, current->j);
		tree.backToParent();
		if(!currentAdd()) {return false; }
		if (!variable_identifier()) {
			error << "variable identifier expected [" << current->i << ", " << current->j << "]" << std::endl;
			return false;
		}
		tree.backToParent();
		if (!identifiers_list()) { return false; }
		tree.backToParent();
		return true;
	}
	else {
		tree.add("<empty>");
		tree.backToParent();
		return true;
	}
}

bool Synatexer_Analizer::statements_list() {
	tree.add("<statement-list>");
	bool statementErr = true;
	if (statement(statementErr)) {
		bool s = statements_list();
		tree.backToParent();
		return s;
	}
	else {
		tree.backToParent();
		tree.get_current()->childs.erase(tree.get_current()->childs.end() - 1);
		tree.add("<empty>");
		tree.backToParent();
		tree.backToParent();
		return !statementErr;
	}
}

bool Synatexer_Analizer::statement(bool &err) {
	tree.add("<statement>");
	if (unsigned_integer()) {
		tree.add("<unsigned-integer>");
		tree.add(current->code, current->i, current->j);
		tree.backToParent();
		tree.backToParent();
		if(!currentAdd()) {return false; }
		if (current->code == ':') {
			tree.add(current->code, current->i, current->j);
			tree.backToParent();
			if(!currentAdd()) {return false; }
			bool s = statement(err);
			if (!s) { err = true; }
			tree.backToParent();
			return s;
		}
		else {
			error << ": is expected [" << current->i << ", " << current->j << "] - ";
			return false;
		}
	}
	if (current->code == 406) {
		tree.add(current->code, current->i, current->j);
		tree.backToParent();
		if(!currentAdd()) {return false; }
		if (current->code == ';') {
			tree.add(current->code, current->i, current->j);
			if(!currentAdd()) {return false; }
			tree.backToParent();
			tree.backToParent();
			return true;
		}
		else {
			error << "; is expected [" << current->i << ", " << current->j << "] - ";
			return false;
		}
	}
	if (current->code == 405) {
		tree.add(current->code, current->i, current->j);
		tree.backToParent();
		if(!currentAdd()) {return false; }
		if (!unsigned_integer()) {
			error << "unsigned integer expected [" << current->i << ", " << current->j << "] - ";
			return false;
		}
		else {
			tree.add("<unsigned-integer>");
			tree.add(current->code, current->i, current->j);
			tree.backToParent();
			tree.backToParent();
			if(!currentAdd()) {return false; }
			if (current->code == ';') {
				tree.add(current->code, current->i, current->j);
				tree.backToParent();
				tree.backToParent();
				if(!currentAdd()) {return false; }
				return true;
			}
			else {
				error << "; is expected [" << current->i << ", " << current->j << "] - ";
				return false;
			}
		}
	}
	if (current->code == ';') {
		tree.add(current->code, current->i, current->j);
		tree.backToParent();
		tree.backToParent();
		if(!currentAdd()) {return false; }
		return true;
	}
	if (asfi()) {
		tree.backToParent();
		return true;
	}
	err = false;
	return false;
}

bool Synatexer_Analizer::variable_identifier() {
	tree.add("<variable-identidier>");
	if (identifier()) {
		tree.backToParent();
		return true;
	}
	return false;
}

bool Synatexer_Analizer::procedure_identifier() {
	tree.add("<procedure-identifier>");
	if (identifier()) {
		tree.backToParent();
		return true;
	}
	error << "procedure-identifier expected [" << current->i << ", " << current->j << "]" << std::endl;
	return false;
}

bool Synatexer_Analizer::asfi() {
	if (current->code == 201) {
		tree.add(current->code, current->i, current->j);
		tree.backToParent();
		tree.add("<assembly-insert-file-identifier>");
		if(!currentAdd()) {return false; }
		if (!identifier()) {
			error << "identifier is expected [" << current->i << ", " << current->j << "]" << std::endl;
			return false;
		}
		tree.backToParent();
		tree.backToParent();
		if (current->code == 202) {
			tree.add(current->code, current->i, current->j);
			tree.backToParent();
			if(!currentAdd()) {return false; }
			return true;
		}
	}
	return false;
}

bool Synatexer_Analizer::identifier() {
	tree.add("<identifier>");
	if (tables->isIdentifier(current->code)) {
		tree.add(current->code, current->i, current->j);
		tree.backToParent();
		if(!currentAdd()) {return false; }
		return true;
	}
	return false;
}

bool Synatexer_Analizer::unsigned_integer() {
	if (tables->isConst(current->code)) {
		return true;
	}
	return false;
}

bool Synatexer_Analizer::currentAdd() {
	current++;
	bool ret = (current == lex->end());
	if (ret) {
		error << "Lexer table ended, but synatexer analizer don`t end\n";
	}
	return !ret;
}
