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
		tree.add(";");
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
		if (!block()) {
			return false;
		}
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
	return false;
}

bool Synatexer_Analizer::declaration() {
	return false;
}

bool Synatexer_Analizer::label_declaration() {
	return false;
}

bool Synatexer_Analizer::label_list() {
	return false;
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
	return false;
}

bool Synatexer_Analizer::statement() {
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
