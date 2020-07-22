#include <iostream>
#include "constant.h"
#include "primitive.h"
#include "camera.h"
#include "l-system.h"

const int WIDTH = 800, HEIGHT = 800;
int mouseButton = -1, iteration = 0;
double mouseX = 0, mouseY = 0;
Camera* camera = nullptr;
char mode = 1;

Primitive** InitObject(int& num)
{
	Primitive** primitives = new Primitive * [1];
	primitives[0] = new Stick(5.0f, 0.1f, 12);
	return primitives;
}

void fractal(Primitive**& primitives, int& num, int& maxIteration, int currentIteration)
{
	//std::cout << "fractal" << std::endl;
	/*float length = 5.0f;
	for (int i = 0; i < currentIteration; i++)length /= 2.0f;*/
	for (int i = 0; i < num; i++)
	{
		primitives[i]->Render(glm::mat4(1.0f));
	}
	//std::cout << maxIteration << std::endl;
	if (currentIteration < maxIteration)
	{
		glPushMatrix();
		glTranslatef(0.0f, 5.0f, 0.0f);
		//glScalef(1.0f, 0.5f, 1.0f);
		glPushMatrix();
		glRotatef(-30.0f, 1.0f, 0.0f, 0.0f);
		glScalef(1.0f, 0.5f, 1.0f);
		fractal(primitives, num, maxIteration, currentIteration + 1);
		glPopMatrix();
		glPushMatrix();
		glRotatef(120.0f, 0.0f, 1.0f, 0.0f);
		glRotatef(-30.0f, 1.0f, 0.0f, 0.0f);
		glScalef(1.0f, 0.5f, 1.0f);
		fractal(primitives, num, maxIteration, currentIteration + 1);
		glPopMatrix();
		glPushMatrix();
		glRotatef(-120.0f, 0.0f, 1.0f, 0.0f);
		glRotatef(-30.0f, 1.0f, 0.0f, 0.0f);
		glScalef(1.0f, 0.5f, 1.0f);
		fractal(primitives, num, maxIteration, currentIteration + 1);
		glPopMatrix();
		glPopMatrix();
	}
}

void lSystem(Primitive** primitives)
{
	LSystem lsystem;
	switch (mode)
	{
	case 0:
		// Koch Island
		/*LSystem lsystem(std::string("F-F-F-F"), primitives[0], 'F', std::string("F-F+F+FF-F-F+F"), 90.0f);
		lsystem.Iteration(iteration);*/
		break;
	case 1: {
		// Branching structures a
		Product productsA[1] = {
			Product('F', std::string("F[+F]F[-F]F"))
		};
		lsystem = LSystem(std::string("F"), primitives[0], productsA, 1, 25.7f);
		lsystem.Iteration(iteration);
		break;
	}
	case 2: {
		// Branching structures b
		Product productsB[1] = {
			Product('F', std::string("F[+F]F[-F][F]"))
		};
		lsystem = LSystem(std::string("F"), primitives[0], productsB, 1, 20.0f);
		lsystem.Iteration(iteration);
		break;
	}
	case 3: {
		// Branching structures c
		Product productsC[1] = {
			Product('F', std::string("FF-[-F+F+F]+[+F-F-F]"))
		};
		lsystem = LSystem(std::string("F"), primitives[0], productsC, 1, 22.5f);
		lsystem.Iteration(iteration);
		break;
	}
	case 4: {
		// Branching structures d
		Product productsD[2] = {
			Product('X', std::string("F[+X]F[-X]+X")),
			Product('F', std::string("FF"))
		};
		lsystem = LSystem(std::string("X"), primitives[0], productsD, 2, 20.0f);
		lsystem.Iteration(iteration);
		break;
	}
	case 5: {
		// Branching structures e
		Product productsE[2] = {
			Product('X', std::string("F[+X][-X]FX")),
			Product('F', std::string("FF"))
		};
		lsystem = LSystem(std::string("X"), primitives[0], productsE, 2, 25.7f);
		lsystem.Iteration(iteration);
		break;
	}
	case 6: {
		// Branching structures f
		Product productsF[2] = {
			Product('X', std::string("F-[[X]+X]+F[+FX]-X")),
			Product('F', std::string("FF"))
		};
		lsystem = LSystem(std::string("X"), primitives[0], productsF, 2, 22.5f);
		lsystem.Iteration(iteration);
		break;
	}
	default:
		std::cout << "Undefined mode:" << mode << std::endl;
	}
	//std::cout <<  << std::endl;
}

void display(GLFWwindow* window)
{
	camera->glInit(WIDTH, HEIGHT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera->glPlaceCamera();

	glColor3f(0.75f, 0.5f, 0.3f);
	int num = 1;
	Primitive** primitives = InitObject(num);

	//fractal(primitives, num, iteration, 0);
	lSystem(primitives);

	if (primitives != nullptr) {
		while (num > 0)delete primitives[--num];
		delete primitives;
	}
}

void mouse(GLFWwindow* window, int button, int action, int mods) {
	// Save the current state of the mouse.  This will be
	// used by the 'motion' function
	if (action == GLFW_PRESS)
	{
		mouseButton = button;
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		mouseX = x;
		mouseY = y;
	}
	else if (action == GLFW_RELEASE)
	{
		mouseButton = -1;
		mouseX = 0;
		mouseY = 0;
	}

}

void motion(GLFWwindow* window, double x, double y) {
	// Left button = rotation
	// (rotate camera around the up and horizontal vectors)
	if (mouseButton == GLFW_MOUSE_BUTTON_LEFT) {
		camera->rotateCamera(0.005 * (mouseX - x), 0.005 * (mouseY - y));
		/*glMatrixMode(GL_PROJECTION);
		glRotatef((mouseX - x) * 0.005f, 0.0f, 1.0f, 0.0f);
		glRotatef((mouseY - y) * 0.005f, 1.0f, 0.0f, 0.0f);*/
		mouseX = x;
		mouseY = y;
	}
	// Middle button = translation
	// (move camera perpendicular to the direction vector)
	else if (mouseButton == GLFW_MOUSE_BUTTON_MIDDLE) {
		camera->truckCamera((mouseX - x) * 0.005, (y - mouseY) * 0.005);
		/*glMatrixMode(GL_PROJECTION);
		glTranslatef((mouseX - x) * 0.005f, (y - mouseY) * 0.005f, 0.0f);*/
		mouseX = x;
		mouseY = y;
	}
	// Right button = zoom
	// (move camera along the direction vector)
	else if (mouseButton == GLFW_MOUSE_BUTTON_RIGHT) {
		camera->dollyCamera((x - mouseX) * 0.05);
		/*glMatrixMode(GL_PROJECTION);
		glTranslatef(0.0f, 0.0f, (x - mouseX) * 0.05f);*/
		mouseX = x;
		mouseY = y;
	}

	// Redraw the scene with the new camera parameters
	//glutPostRedisplay();
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action != GLFW_PRESS)return;
	switch (key) {
	case GLFW_KEY_1:
		mode = 1;
		break;
	case GLFW_KEY_2:
		mode = 2;
		break;
	case GLFW_KEY_3:
		mode = 3;
		break;
	case GLFW_KEY_4:
		mode = 4;
		break;
	case GLFW_KEY_5:
		mode = 5;
		break;
	case GLFW_KEY_6:
		mode = 6;
		break;
	case GLFW_KEY_ESCAPE:
	case GLFW_KEY_Q:
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_MINUS:
	case GLFW_KEY_KP_SUBTRACT:
		if(iteration>0)iteration--;
		break;
	case GLFW_KEY_EQUAL:
	case GLFW_KEY_KP_ADD:
		iteration++;
		break;
	default:
		printf("UNKNOWN KEYBOARD INPUT  '%c'\n", key);
	}
}

void scroll(GLFWwindow* window, double xoffset, double yoffset)
{
	float ratio = yoffset < 0 ? 1.25f : 0.8f;
	camera->scaleCamera(ratio);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Viewer", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glViewport(0, 0, WIDTH, HEIGHT);

	GLfloat ambient[] = { 0.5f, 0.7f, 0.5f, 1.0f };
	
	

	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll);
	glfwSetKeyCallback(window, keyboard);
	glfwSetMouseButtonCallback(window, mouse);
	glfwSetCursorPosCallback(window, motion);
	glfwMakeContextCurrent(window);

	camera = new OrthographicCamera(Vec3f(0.0f, 0.0f, 10.0f), Vec3f(0.0f, 0.0f, -1.0f), Vec3f(0.0f, 1.0f, 0.0f), 20.0f);
	
	//std::cout << "???" << std::endl;
	//std::cout<< str.length() <<std::endl;
	/*display(window);
	glfwSwapBuffers(window);*/
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(ambient[0], ambient[1], ambient[2], ambient[3]);
		glClear(GL_COLOR_BUFFER_BIT);
		display(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	delete camera;
	return 0;
}

