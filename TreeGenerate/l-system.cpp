#include <iostream>
#include "l-system.h"

//LSystem::LSystem(std::string s, Primitive* p, char pl, std::string pr, float d = 90.0f)
//{
//	start = s;
//	productLeft = pl;
//	productRight = pr;
//	deltaMinus = -d;
//	deltaPlus = d;
//	primitive = p;
//}
//
//
//LSystem::LSystem(std::string s, Primitive* p, char pl, std::string pr, float dm, float dp)
//{
//	start = s;
//	productLeft = pl;
//	productRight = pr;
//	deltaMinus = dm;
//	deltaPlus = dp;
//	primitive = p;
//}

LSystem::LSystem(std::string s, Primitive* p, Product* ps, int psn, float d = 90.0f)
{
	start = s;
	products = ps;
	productsNum = psn;
	deltaMinus = -d;
	deltaPlus = d;
	primitive = p;
}

LSystem::LSystem(std::string s, Primitive* p, Product* ps, int psn, float dm, float dp)
{
	start = s;
	products = ps;
	productsNum = psn;
	deltaMinus = dm;
	deltaPlus = dp;
	primitive = p;
}

std::string LSystem::Iteration(int n)
{
	std::string result = start;
	for (int j = 0; j < n; j++)
	{
		std::string temp = "";
		for (int i = 0; i < result.length(); i++)
		{
			//std::cout << result.at(i) << std::endl;
			bool found = false;
			for (int j = 0; j < productsNum; j++)
			{
				if (result.at(i) == (products+j)->left)
				{
					temp += (products + j)->right;
					found = true;
					break;
				}
			}
			if(!found)
			{
				temp += result.at(i);
			}
		}
		/*for (auto it = result.begin(); it != result.end(); ++it)
		{
			if (*it == productLeft)
			{
				temp += productRight;
			}
			else
			{
				std::cout << char(*it) << std::endl;
				temp += (*it);
			}
		}*/
		result = temp;
		//std::cout << result << std::endl;
	}
	glDraw(result, n);
	return result;
}

void LSystem::glDraw(std::string str, int iteration)
{
	float lengthRatio = 5.0f, widthRatio = 1.0f;
	while (iteration-- > 0) {
		lengthRatio /= 2.0f;
		widthRatio /= 1.0f;
	}
	for (auto it = str.begin(); it != str.end(); ++it)
	{
		switch (*it)
		{
		case 'F':
			glm::mat4 mat = glm::mat4(1.0f);
			mat = glm::scale(mat, glm::vec3(widthRatio, lengthRatio, widthRatio));
			primitive->Render(mat);
			//glTranslatef(0.0f, 5.0f, 0.0f);
			glTranslatef(0.0f, 5.0f * lengthRatio, 0.0f);
			break;
		case '-':
			glRotatef(deltaMinus, 0.0f, 0.0f, 1.0f);
			break;
		case '+':
			glRotatef(deltaPlus, 0.0f, 0.0f, 1.0f);
			break;
		case '[':
			glPushMatrix();
			break;
		case ']':
			glPopMatrix();
			break;
		case 'X':
			break;
		default:
			std::cout << "Undefined character:" << *it << std::endl;
			break;
		}
	}
}