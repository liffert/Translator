#include "Tree.h"
#include <iostream>

Tree::Tree() {
	head = std::make_shared<element>(element(nullptr, "<signal-program>", 0, 0));
	current = head;
}

void Tree::add(std::variant<std::string, int> value, int i, int j) {
	current->childs.push_back(std::make_shared<element>(current, value, i, j));
	current = current->childs.back();
}

void Tree::backToParent() {
	current = current->parent;
}

void Tree::print() {
	print(head, "");
}

void Tree::saveToFile(std::ofstream& file) {
	if (!file.is_open()) {
		std::cout << "ERROR file is not open\n";
		return;
	}
	saveToFile(file, head, "");
}

std::shared_ptr<Tree::element> Tree::get_head() const {
	return head;
}

void Tree::print(const std::shared_ptr<element>& ptr, std::string separator) const {
	if (!ptr) {
		return;
	}
	std::cout << "(" << separator.length() << ")" << separator << pval(ptr) << std::endl;
	for (const auto& iter : ptr->childs) {
		print(iter, separator + "---");
	}
}

void Tree::saveToFile(std::ofstream& file, const std::shared_ptr<element>& ptr, std::string separator) const {
	if (!ptr) {
		return;
	}
	file << "(" << separator.length() << ")" << separator << pval(ptr) << std::endl;
	for (const auto& iter : ptr->childs) {
		saveToFile(file, iter, separator + "---");
	}
}

std::string Tree::pval(const std::shared_ptr<element>& ptr) const {
	if (ptr->value.index() == 1) {
		return std::to_string(std::get<1>(ptr->value));
	}
	else {
		return std::get<0>(ptr->value);
	}
}

Tree::el::el(std::shared_ptr<el> parent, std::variant<std::string, int> value, int i, int j) noexcept : parent(parent), value(value), i(i), j(j) {}
