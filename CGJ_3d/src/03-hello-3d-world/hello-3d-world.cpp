////////////////////////////////////////////////////////////////////////////////
//
// Adding a camera and moving to the 3rd dimension.
// A "Hello 3D World" of Modern OpenGL.
//
// Copyright (c) 2013-23 by Carlos Martinho
//
// INTRODUCES:
// VIEW PIPELINE, UNIFORM BUFFER OBJECTS, mglCamera.hpp
//
////////////////////////////////////////////////////////////////////////////////

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext/matrix_relational.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

#include <mgl.hpp>

////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App {
public:
	void initCallback(GLFWwindow* win) override;
	void displayCallback(GLFWwindow* win, double elapsed) override;
	void windowCloseCallback(GLFWwindow* win) override;
	void windowSizeCallback(GLFWwindow* win, int width, int height) override;
	void cursorCallback(GLFWwindow* win, double xpos, double ypos) override;
	void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) override;
	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void scrollCallback(GLFWwindow* win, double xoffset, double yoffset);

private:
	const GLuint POSITION = 0, COLOR = 1, UBO_BP = 0;
	GLuint VaoId;

	mgl::SceneGraph* SceneGraph = nullptr;
	mgl::Camera* Camera = nullptr;
	mgl::Texture2D* Texture = nullptr;

	mgl::Camera* createCamera(glm::mat4 viewMatrix);
	void CreateSceneGraph();
	void getObject();
	void drawScene();
	void animate();

	float lastX = 0.0f, lastY = 0.0f;	//Mouse
	float clickX = 0.0f, clickY = 0.0f;	//Mouse

	glm::mat4 ViewMatrix;			//Camera
	glm::vec3 ViewMatrix_position;	//Position
	glm::quat ViewMatrix_rotation;	//Rotation
	glm::vec3 ViewMatrix_center;	//Center

	bool moveObject = false;
	mgl::SceneNode* selectedObject = nullptr;

	const float THRESHOLD = static_cast<float>(1.0e-5);
};


void MyApp::CreateSceneGraph() {
	ViewMatrix_center = glm::vec3(0.0f, 7.0f, 0.0f);
	glm::quat ViewMatrix_rotation = glm::quat(1.0, 0.0, 0.0, 0.0);

	//check if it exists if yes load

	//////////////////////////////////////////////////////CAMERA
	// Eye(0,20,40) Center(ViewMatrix_center) Up(0,1,0)
	ViewMatrix_position = glm::vec3(0.0f, 20.0f, 40.0f);

	ViewMatrix = glm::lookAt(ViewMatrix_position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	Camera = createCamera(ViewMatrix);
	//////////////////////////////////////////////////////

	SceneGraph = new mgl::SceneGraph;
	mgl::SceneNode* RootNode = new mgl::SceneNode("Root");
	mgl::SceneNode* TableNode;
	mgl::SceneNode* BallNode;

	SceneGraph->create(RootNode, Camera, ViewMatrix, ViewMatrix_position, ViewMatrix_rotation);


	TableNode = new mgl::SceneNode("Table");

	Texture = new mgl::Texture2D;
	TableNode->create(RootNode, Texture, "stonetable.obj", "./assets/shaders/marble-vs.glsl", "./assets/shaders/marble-fs.glsl");
	TableNode->changeShaderColor(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
	RootNode->add(TableNode);


	BallNode = new mgl::SceneNode("Ball");
	Texture = new mgl::Texture2D;
	BallNode->create(TableNode, Texture, "tennisball.obj", "./assets/shaders/wood-vs.glsl", "./assets/shaders/wood-fs.glsl");
	BallNode->move(glm::vec3(0.0f, 7.0f, 0.0f));
	BallNode->changeShaderColor(glm::vec4(0.0f, 0.7f, 0.0f, 0.99f));
	TableNode->add(BallNode);

}

///////////////////////////////////////////////////////////////////////// CAMERA

mgl::Camera* MyApp::createCamera(glm::mat4 ViewMatrix) {
	// Perspective Fovy(30) Aspect(640/480) NearZ(1) FarZ(50)
	float fov = 30.0;
	glm::mat4 ProjectionMatrix =
		glm::perspective(glm::radians(fov), 640.0f / 480.0f, 1.0f, 100.0f);

	mgl::Camera* c = nullptr;
	c = new mgl::Camera(UBO_BP);
	c->setViewMatrix(glm::translate(ViewMatrix * glm::mat4(ViewMatrix_rotation), -ViewMatrix_center));
	c->setProjectionMatrix(ProjectionMatrix);
	return c;
}


void MyApp::drawScene() {
	SceneGraph->RootNode->draw();
}


////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow* win) {
	CreateSceneGraph();
}


void MyApp::windowCloseCallback(GLFWwindow* win) { }

void MyApp::windowSizeCallback(GLFWwindow* win, int winx, int winy) {
	glViewport(0, 0, winx, winy);
}

void MyApp::displayCallback(GLFWwindow* win, double elapsed) { drawScene(); }


void MyApp::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	MyApp* myApp = static_cast<MyApp*>(glfwGetWindowUserPointer(window));

	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		if (action == GLFW_REPEAT) {
			//std::cout << "Being pressed" << "\n";
		}
		if (selectedObject != nullptr) { //CHECK IF OBJECT IS SELECTED
			if (key == GLFW_KEY_UP || key == GLFW_KEY_DOWN || key == GLFW_KEY_RIGHT || key == GLFW_KEY_LEFT) {
				glm::mat4 c = glm::inverse(Camera->getViewMatrix());
				glm::vec3 cameraRight = glm::vec3(c[0]);
				glm::vec3 cameraForward = glm::vec3(c[2]);

				cameraForward.y = 0;
				cameraRight.y = 0;

				glm::vec3 right = cameraRight * (0.2f * (int(key == GLFW_KEY_RIGHT) - int(key == GLFW_KEY_LEFT)));
				glm::vec3 depth = -cameraForward * (0.2f * (int(key == GLFW_KEY_UP) - int(key == GLFW_KEY_DOWN)));

				selectedObject->move(depth + right);
			}
		}
	}
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT_CONTROL) {
			moveObject = true;
		}
		if (key == GLFW_KEY_S) {
			SceneGraph->ViewMatrix = ViewMatrix;
			SceneGraph->ViewMatrix_position = ViewMatrix_position;
			SceneGraph->ViewMatrix_rotation = ViewMatrix_rotation;
			SceneGraph->save("./assets/saves/save.txt");

		}
		if (key == GLFW_KEY_L) {
			SceneGraph->load("./assets/saves/save.txt");

			ViewMatrix = SceneGraph->ViewMatrix;
			ViewMatrix_position = SceneGraph->ViewMatrix_position;
			ViewMatrix_rotation = SceneGraph->ViewMatrix_rotation;

			Camera->setViewMatrix(glm::translate(ViewMatrix * glm::mat4(ViewMatrix_rotation), -ViewMatrix_center));
		}
	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_LEFT_CONTROL) {
			moveObject = false;
		}
	}
}


void MyApp::getObject() {
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	unsigned char pixelColor[4];
	glReadPixels((GLint)lastX, viewport[3] - (GLint)lastY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixelColor);
	glm::vec4 pixel(pixelColor[0] / 255.0f, pixelColor[1] / 255.0f, pixelColor[2] / 255.0f, pixelColor[3] / 255.0f);

	if (!(glm::all(glm::equal(pixel, glm::vec4(0.1f, 0.1f, 0.3f, 1.0f), 0.01f)))) {
		//std::cout << glm::to_string(pixel) << "\n";
		int intAlpha = (int)(pixel[3] * 100 + .5);
		float floatAlpha = intAlpha / 100.0;			//round to 2 places decimal
		selectedObject = SceneGraph->RootNode->getSceneAlpha(floatAlpha);
		std::cout << selectedObject->NodeName << "\n";
	}
}

void MyApp::cursorCallback(GLFWwindow* win, double xpos, double ypos) {
	if (!moveObject && glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		float deltaX = float(xpos) - lastX;
		float deltaY = lastY - float(ypos);

		glm::mat4 c = glm::inverse(Camera->getViewMatrix());

		glm::quat rotX = glm::angleAxis(glm::radians(deltaX), glm::vec3(c[1]));
		glm::quat rotY = glm::angleAxis(glm::radians(deltaY), -glm::vec3(c[0]));

		ViewMatrix_rotation = glm::normalize(ViewMatrix_rotation * rotY * rotX);


		ViewMatrix = glm::lookAt(ViewMatrix_position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		Camera->setViewMatrix(glm::translate(ViewMatrix * glm::mat4(ViewMatrix_rotation), -ViewMatrix_center));

	}
	lastX = float(xpos);
	lastY = float(ypos);
}

void MyApp::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (moveObject && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		lastX = (float)mouseX;
		lastY = (float)mouseY;
		getObject();
	}
}


void MyApp::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	float offset = -(float)yoffset;
	ViewMatrix_position = glm::inverse(ViewMatrix)[3];
	float dist = glm::length(ViewMatrix_position - ViewMatrix_center);

	if (dist > 10.0f && offset < 1 || offset > 1 && dist < 50.0f || (dist > 10.0f && dist < 50.0f)) {
		ViewMatrix_position += (ViewMatrix_position)*offset * 0.05f;

		clamp(ViewMatrix_position, 30.0f, 40.0f);

		ViewMatrix = glm::lookAt(ViewMatrix_position, glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));
		Camera->setViewMatrix(glm::translate(ViewMatrix * glm::mat4(ViewMatrix_rotation), -ViewMatrix_center));
	}
}


/////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char* argv[]) {
	mgl::Engine& engine = mgl::Engine::getInstance();
	engine.setApp(new MyApp());
	engine.setOpenGL(3, 3);
	engine.setWindow(800, 600, "Hello Modern 3D World", 0, 1);
	engine.init();
	engine.run();
	exit(EXIT_SUCCESS);
}

////////////////////////////////////////////////////////////////////////////////
