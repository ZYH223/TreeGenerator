#pragma once
#include "constant.h"

class Primitive {
public:
	virtual void Render(glm::mat4 topologicalMatrix) = 0;
};

class Stick: public Primitive {
public:
	float length, radius;
	int tessellation;
	Stick(float l, float r, int t);
	void Render(glm::mat4 topologicalMatrix);
};