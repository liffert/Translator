#pragma once
#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include <variant>

class Tree {
private:
	struct el {
		std::variant<std::string, int> value;
		std::vector<std::shared_ptr<el>> childs;
		std::shared_ptr<el> parent = nullptr;
		el(std::shared_ptr<el> parent, std::variant<std::string, int> value, int i, int j) noexcept;
		int j = 0;
		int i = 0;
	};

public:
	using element = struct el;
private:
	std::shared_ptr<element> head = nullptr;
	std::shared_ptr<element> current = nullptr;
	void print(const std::shared_ptr<element> &ptr, std::string separator) const;
	void saveToFile(std::ofstream& file, const std::shared_ptr<element>& ptr, std::string separator) const;
	std::string pval(const std::shared_ptr<element>& ptr) const;

public:
	Tree();
	void add(std::variant<std::string, int> value, int i = 0, int j = 0);
	void backToParent();
	void print();
	void saveToFile(std::ofstream &file);
	std::shared_ptr<element> get_head() const;
};