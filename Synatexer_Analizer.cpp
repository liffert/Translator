#include "Synatexer_Analizer.h"

Synatexer_Analizer::Synatexer_Analizer(std::shared_ptr<Tables>& tables, std::vector<struct Lexical_Analizer::lex>& lex) {
	this->lex = std::make_shared<std::vector<struct Lexical_Analizer::lex>>(lex);
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

void Synatexer_Analizer::outError() const {
	std::cout << "\n\nERROR:\n" << error.str() << std::endl;
}

bool Synatexer_Analizer::program() {
	succsess = false;
	if (current->code == 401) {
		tree.add("<program>");
		current++;
		if (!procedure_identifier()) {
			return false;
		}
		tree.backToParent();
		tree.backToParent();
		if (!parameters_list()) {
			return false;
		}
		tree.backToParent();
		if (current->code != ';') {
			error << "; expected [" << current->i << ", " << current->j << "]" << std::endl;
			return false;
		}
		tree.add(current->name + "     " + std::to_string(current->code));
		tree.backToParent();
		current++;
		if (!block()) {
			return false;
		}
		current++;
		tree.backToParent();
		if (current->code != ';') {
			error << "; expected [" << current->i << ", " << current->j << "]" << std::endl;
			return false;
		}
		tree.add(current->name + "     " + std::to_string(current->code));
		tree.backToParent();
		current++;
		if (current == lex->end()){
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
		tree.add(current->name + "     " + std::to_string(current->code));
		current++;
		if (!statements_list()) { return false; }
		tree.backToParent();
		if (current->code == 403) {
			tree.add(current->name + "     " + std::to_string(current->code));
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
		tree.add(current->name + "     " + std::to_string(current->code));
		current++;
		if (unsigned_integer()) {
			tree.add("<unsigned-integer>");
			tree.add(current->name + "     " + std::to_string(current->code));
			tree.backToParent();
			tree.backToParent();
			current++;
			if (!label_list()) { return false; }
			if (current->code == ';') {
				tree.backToParent();
				tree.add(current->name + "     " + std::to_string(current->code));
				current++;
				tree.backToParent();
				return true;
			}
			else {
				error << "; is expected [" << current->i << ", " << current->j << "]" << std::endl;
				return false;
			}
		}
		else {
			tree.add("<empty>");
			tree.backToParent();
			tree.backToParent();
			return true;
		}
	}
	else {
		tree.add("<empty>");
		tree.backToParent();
		tree.backToParent();
		return true;
	}
}

bool Synatexer_Analizer::label_list() {
	tree.add("<label-list>");
	if (current->code == ',') {
		tree.add(current->name + "     " + std::to_string(current->code));
		tree.backToParent();
		current++;
		if (!unsigned_integer()) { 
			error << "unsigned integer expected [" << current->i << ", " << current->j << "]" << std::endl;
			return false; 
		}
		else {
			tree.add("<unsigned-integer>");
			tree.add(current->name + "     " + std::to_string(current->code));
			current++;
			tree.backToParent();
			tree.backToParent();
		}
		if (!label_list()) { return false; }
		tree.backToParent();
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
		tree.add(current->name + "     " + std::to_string(current->code));
		tree.backToParent();
		current++;
		if (!variable_identifier()) {
			error << "variable identifier expected [" << current->i << ", " << current->j << "]" << std::endl;
			return false; 
		}
		tree.backToParent();
		if (!identifiers_list()) { return false; }
		tree.backToParent();
		if (current->code == ')') {
			tree.add(current->name + "     " + std::to_string(current->code));
			current++;
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
		tree.add(current->name + "     " + std::to_string(current->code));
		tree.backToParent();
		current++;
		if (!variable_identifier()) { 
			error << "variable identifier expected [" << current->i << ", " << current->j << "]" << std::endl; 
			return false; 
		}
		tree.backToParent();
		if (!identifiers_list()) { return false; }
		tree.backToParent();
	}
	else {
		tree.add("<empty>");
		tree.backToParent();
		return true;
	}
}

bool Synatexer_Analizer::statements_list() {
	tree.add("<statement-list>");
	if (current->code != 403) {
		if (!statement()) { return false; }
		bool s = statements_list();
		tree.backToParent();
		return s;
	}
	else {
		tree.add("<empty>");
		tree.backToParent();
		tree.backToParent();
		return true;
	}
}

bool Synatexer_Analizer::statement() {
	tree.add("<statement>");
	if (unsigned_integer()) {
		tree.add("<unsigned-integer>");
		tree.add(current->name + "     " + std::to_string(current->code));
		tree.backToParent();
		tree.backToParent();
		current++;
		if (current->code == ':') {
			tree.add(current->name + "     " + std::to_string(current->code));
			tree.backToParent();
			current++;
			bool s = statement();
			tree.backToParent();
			return s;
		}
		else {
			error << ": is expected [" << current->i << ", " << current->j << "]" << std::endl;
			return false;
		}
	}
	if (current->code == 406) {
		tree.add(current->name + "     " + std::to_string(current->code));
		tree.backToParent();
		current++;
		if (current->code == ';') {
			tree.add(current->name + "     " + std::to_string(current->code));
			current++;
			tree.backToParent();
			tree.backToParent();
			return true;
		}
		else {
			error << "; is expected [" << current->i << ", " << current->j << "]" << std::endl;
			return false;
		}
	}
	if (current->code == 405) {
		tree.add(current->name + "     " + std::to_string(current->code));
		tree.backToParent();
		current++;
		if (!unsigned_integer()) {
			error << "unsigned integer expected [" << current->i << ", " << current->j << "]" << std::endl;
			return false;
		}
		else {
			tree.add("<unsigned-integer>");
			tree.add(current->name + "     " + std::to_string(current->code));
			tree.backToParent();
			tree.backToParent();
			current++;
			if (current->code == ';') {
				tree.add(current->name + "     " + std::to_string(current->code));
				tree.backToParent();
				tree.backToParent();
				current++;
				return true;
			}
			else {
				error << "; is expected [" << current->i << ", " << current->j << "]" << std::endl;
				return false;
			}
		}
	}
	if (current->code == ';') {
		tree.add(current->name + "     " + std::to_string(current->code));
		tree.backToParent();
		tree.backToParent();
		current++;
		return true;
	}
	if (asfi()) {	
		tree.backToParent();
		return true; 
	}
	error << "error statement [" << current->i << ", " << current->j << "]" << std::endl;
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
		tree.add(current->name + "     " + std::to_string(current->code));
		tree.backToParent();
		tree.add("<assembly-insert-file-identifier>");
		current++;
		if (!identifier()) {
			error << "identifier is expected [" << current->i << ", " << current->j << "]" << std::endl;
			return false;
		}
		tree.backToParent();
		tree.backToParent();
		if (current->code == 202) {
			tree.add(current->name + "     " + std::to_string(current->code));
			tree.backToParent();
			current++;
			return true;
		}
	}
	return false;
}

bool Synatexer_Analizer::identifier() {
	tree.add("<identifier>");
	if (tables->isIdentifier(current->code)) {
		tree.add(current->name + "     " + std::to_string(current->code));
		tree.backToParent();
		current++;
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