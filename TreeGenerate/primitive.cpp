#include "primitive.h"
#include <math.h>
Stick::Stick(float l, float r, int t)
{
	length = l;
	radius = r;
	tessellation = t;
}

void Stick::Render(glm::mat4 topologicalMatrix)
{
	glPushMatrix();
	glMultMatrixf(glm::value_ptr(topologicalMatrix));
	const float thetaStep = RD * 360.0f / tessellation;
	glBegin(GL_QUADS);
	for (float iTheta = RD * 0.0f; iTheta < RD * 360.0f; iTheta += thetaStep)
	{
		glVertex3f(radius * sin(iTheta), 0.0f, radius * cos(iTheta));
		glVertex3f(radius * sin(iTheta), length, radius * cos(iTheta));
		glVertex3f(radius * sin(iTheta + thetaStep), length, radius * cos(iTheta + thetaStep));
		glVertex3f(radius * sin(iTheta + thetaStep), 0.0f, radius * cos(iTheta + thetaStep));
	}
	glEnd();
	glPopMatrix();
}