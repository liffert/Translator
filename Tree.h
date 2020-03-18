#pragma once
#include <memory>
#include <vector>
#include <string>
#include <fstream>
class Tree {
private:
	struct el {
		std::string value;
		std::vector<std::shared_ptr<el>> childs;
		std::shared_ptr<el> parent = nullptr;
		el(std::shared_ptr<el> parent, std::string value) noexcept;
	};
	using element = struct el;
	std::shared_ptr<element> head = nullptr;
	std::shared_ptr<element> current = nullptr;
	void print(const std::shared_ptr<element> &ptr, std::string separator) const;
	void saveToFile(std::ofstream& file, const std::shared_ptr<element>& ptr, std::string separator) const;

public:
	Tree();
	void add(std::string value);
	void backToParent();
	void print();
	void saveToFile(std::ofstream &file);
};