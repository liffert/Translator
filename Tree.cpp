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

std::shared_ptr<Tree::element> Tree::get_head() const {
	return head;
}

void Tree::print(const std::shared_ptr<element>& ptr, std::string separator) const {
	if (!ptr) {
		return;
	}
	std::cout << separator << ptr->value << std::endl;
	for (const auto& iter : ptr->childs) {
		print(iter, separator + "---");
	}
}

Tree::el::el(std::shared_ptr<el> parent, std::string value) noexcept : parent(parent), value(value) {}
