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

bool Synatexer_Analizer::program() {
	if (current->code == 401) {
		tree.add("program");
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
			std::cout << "; expected\n";
			return false;
		}
		tree.add(";");
		tree.backToParent();
		current++;
		if (!block()) {
			return false;
		}
		current++;
		tree.backToParent();
		if (current->code != ';') {
			std::cout << "; expected\n";
			return false;
		}
		tree.add(";");
		tree.backToParent();
	}
	return false;
}

bool Synatexer_Analizer::block() {
	tree.add("block");
	if (!declaration()) { return false; }
	tree.backToParent();
	if (current->code == 402) {
		tree.add("BEGIN");
		current++;
		if (!statements_list()) { return false; }
		tree.backToParent();
		if (current->code == 403) {
			tree.add("END");
			return true;
		}
		else {
			std::cout << "END is expected\n";
			return false;
		}
	}
	std::cout << "BEGIN is expected\n";
	return false;
}

bool Synatexer_Analizer::declaration() {
	tree.add("declaration");
	if (!label_declaration()) { return false; }
	tree.backToParent();
	return true;
}

bool Synatexer_Analizer::label_declaration() {
	tree.add("label-declaration");
	if (current->code == 404) {
		tree.add("label");
		current++;
		if (unsigned_integer()) {
			tree.add("unsigned-integer");
			tree.add(current->name);
			tree.backToParent();
			tree.backToParent();
			current++;
			if (!label_list()) { return false; }
			if (current->code == ';') {
				tree.backToParent();
				tree.add(current->name);
				current++;
				tree.backToParent();
				return true;
			}
			else {
				std::cout << "; is expected\n";
				return false;
			}
		}
		else {
			tree.add("empty");
			tree.backToParent();
			tree.backToParent();
			return true;
		}
	}
	else {
		tree.add("empty");
		tree.backToParent();
		tree.backToParent();
		return true;
	}
}

bool Synatexer_Analizer::label_list() {
	tree.add("label-list");
	if (current->code == ',') {
		tree.add(current->name);
		tree.backToParent();
		current++;
		if (!unsigned_integer()) { return false; }
		else {
			tree.add("unsigned-integer");
			tree.add(current->name);
			current++;
			tree.backToParent();
			tree.backToParent();
		}
		if (!label_list()) { return false; }
		tree.backToParent();
	}
	else {
		tree.add("empty");
		tree.backToParent();
		tree.backToParent();
		return true;
	}
}

bool Synatexer_Analizer::parameters_list() {
	tree.add("parameters-list");
	if (current->code == '(') {
		tree.add("(");
		tree.backToParent();
		current++;
		if (!variable_identifier()) { return false; }
		tree.backToParent();
		if (!identifiers_list()) { return false; }
		tree.backToParent();
		if (current->code == ')') {
			tree.add(")");
			current++;
			tree.backToParent();
			return true;
		}
		else {
			return false;
		}
	}
	tree.add("empty");
	tree.backToParent();
}

bool Synatexer_Analizer::identifiers_list() {
	tree.add("identifiers-list");
	if (current->code == ',') {
		tree.add(current->name);
		tree.backToParent();
		current++;
		if (!variable_identifier()) { return false; }
		tree.backToParent();
		if (!identifiers_list()) { return false; }
		tree.backToParent();
	}
	else {
		tree.add("empty");
		tree.backToParent();
		return true;
	}
}

bool Synatexer_Analizer::statements_list() {
	tree.add("statement-list");
	if (current->code != 403) {
		if (!statement()) { return false; }
		statements_list();
		tree.backToParent();
	}
	else {
		tree.add("empty");
		tree.backToParent();
		tree.backToParent();
		return true;
	}
}

bool Synatexer_Analizer::statement() {
	tree.add("statement");
	if (unsigned_integer()) {
		tree.add("unsigned-integer");
		tree.add(current->name);
		tree.backToParent();
		tree.backToParent();
		current++;
		if (current->code == ':') {
			tree.add(current->name);
			tree.backToParent();
			current++;
			bool s = statement();
			tree.backToParent();
			return s;
		}
		else {
			std::cout << ": is expected\n";
			return false;
		}
	}
	if (current->code == 406) {
		tree.add(current->name);
		tree.backToParent();
		current++;
		if (current->code == ';') {
			tree.add(current->name);
			current++;
			tree.backToParent();
			tree.backToParent();
			return true;
		}
		else {
			std::cout << "; is expected\n";
			return false;
		}
	}
	if (current->code == 405) {
		tree.add(current->name);
		tree.backToParent();
		current++;
		if (!unsigned_integer()) {
			std::cout << "unsigned integer expected\n";
			return false;
		}
		else {
			tree.add("unsigned-integer");
			tree.add(current->name);
			tree.backToParent();
			tree.backToParent();
			current++;
			if (current->code == ';') {
				tree.add(current->name);
				tree.backToParent();
				tree.backToParent();
				current++;
				return true;
			}
			else {
				std::cout << "; is expected\n";
				return false;
			}
		}
	}
	if (asfi()) {	
		tree.backToParent();
		return true; 
	}
	std::cout << "error statement\n";
	return false;
}

bool Synatexer_Analizer::variable_identifier() {
	tree.add("variable-identidier");
	if (identifier()) {
		tree.backToParent();
		return true;
	}
	return false;
}

bool Synatexer_Analizer::procedure_identifier() {
	tree.add("procedure-identifier");
	if (identifier()) {
		tree.backToParent();
		return true;
	}
	std::cout << "procedure-identifier expected\n";
	return false;
}

bool Synatexer_Analizer::asfi() {
	if (current->code == 201) {
		tree.add(current->name);
		tree.backToParent();
		tree.add("assembly-insert-file-identidier");
		current++;
		if (!identifier()) {
			std::cout << "identifier is expected\n";
			return false;
		}
		tree.backToParent();
		tree.backToParent();
		if (current->code == 202) {
			tree.add(current->name);
			tree.backToParent();
			current++;
			return true;
		}
	}
	return false;
}

bool Synatexer_Analizer::identifier() {
	tree.add("identifier");
	if (tables->isIdentifier(current->code)) {
		tree.add(current->name);
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
