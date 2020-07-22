#pragma once
#include <string>
#include "constant.h"
#include "primitive.h"

struct Product {
	char left;
	std::string right;
	Product() {}
	Product(char l, std::string r) : left(l), right(r) {}
};

class LSystem {
public:
	/*LSystem(std::string s, Primitive* p, char pl, std::string pr, float d);
	LSystem(std::string s, Primitive* p, char pl, std::string pr, float dl, float dr);*/
	LSystem() {}
	LSystem(std::string s, Primitive* p, Product* ps, int psn, float d);
	LSystem(std::string s, Primitive* p, Product* ps, int psn, float dl, float dr);
	std::string Iteration(int n);
	void glDraw(std::string str, int iteration);
private:
	std::string start;
	Product* products;
	int productsNum;
	/*char productLeft;
	std::string productRight;*/

	float deltaMinus, deltaPlus;
	Primitive* primitive;
};