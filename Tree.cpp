#include "Tree.h"
#include <iostream>

Tree::Tree() {
	head = std::make_shared<element>(element(nullptr, "signal-program"));
	current = head;
}

void Tree::add(std::string value) {
	current->childs.push_back(std::make_shared<element>(current, value));
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

void Tree::print(const std::shared_ptr<element>& ptr, std::string separator) const {
	if (!ptr) {
		return;
	}
	std::cout << "(" << separator.length() << ")" << separator << ptr->value << std::endl;
	for (const auto& iter : ptr->childs) {
		print(iter, separator + "---");
	}
}

void Tree::saveToFile(std::ofstream& file, const std::shared_ptr<element>& ptr, std::string separator) const {
	if (!ptr) {
		return;
	}
	file << "(" << separator.length() << ")" << separator << ptr->value << std::endl;
	for (const auto& iter : ptr->childs) {
		saveToFile(file, iter, separator + "---");
	}
}

Tree::el::el(std::shared_ptr<el> parent, std::string value) noexcept : parent(parent), value(value) {}
