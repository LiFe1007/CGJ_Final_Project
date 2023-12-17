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
	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void scrollCallback(GLFWwindow* win, double xoffset, double yoffset);

private:
	const GLuint POSITION = 0, COLOR = 1, UBO_BP = 0;
	GLuint VaoId;

	mgl::SceneGraph* SceneGraph = nullptr;
	mgl::ShaderProgram* Shaders = nullptr;
	mgl::Camera* Camera = nullptr;
	GLint ModelMatrixId, ColorId;
	mgl::Mesh* Mesh = nullptr;
	mgl::Texture2D* Texture = nullptr;

	mgl::Mesh* createMeshes(std::string mesh_obj);
	mgl::ShaderProgram* createShaderProgram(std::string vs, std::string fs);
	mgl::Camera* createCamera(glm::mat4 viewMatrix);
	void CreateSceneGraph();
	void getObject();
	glm::vec3 getLookAt();
	//void updateScenegraph(double elapsed) override;
	void drawScene();
	void animate();
	void saveSceneGraph();

	float lastX, lastY;		//Mouse

	glm::mat4 ViewMatrix;			//Camera
	glm::vec3 ViewMatrix_position;	//Position
	glm::quat ViewMatrix_rotation;	//Rotation
	glm::vec3 ViewMatrix_center;	//Center


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
	Mesh = createMeshes("stonetable.obj");
	Shaders = createShaderProgram("./assets/shaders/marble-vs.glsl", "./assets/shaders/marble-fs.glsl");
	Shaders->bind();
	glUniformMatrix4fv(ModelMatrixId, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
	glUniform3fv(ColorId, 1, glm::value_ptr(glm::vec3(0.7f, 0.7f, 0.7f)));
	Shaders->unbind();
	Texture = new mgl::Texture2D;
	Texture->load("./assets/textures/image.jpg");
	TableNode->create(RootNode, Texture, Mesh, Shaders);
	RootNode->add(TableNode);


	Shaders = nullptr;
	Mesh = nullptr;
	BallNode = new mgl::SceneNode("Ball");
	Mesh = createMeshes("tennisball.obj");
	Shaders = createShaderProgram("./assets/shaders/wood-vs.glsl", "./assets/shaders/wood-fs.glsl");
	Shaders->bind();
	glUniform3fv(ColorId, 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));
	glUniformMatrix4fv(ModelMatrixId, 1, GL_FALSE, glm::value_ptr(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 7.0f, 0.0f))));
	Shaders->unbind();
	Texture = new mgl::Texture2D;
	Texture->load("./assets/textures/image.jpg");
	BallNode->create(TableNode, Texture, Mesh, Shaders);
	TableNode->add(BallNode);


	/** /
	mgl::scenenode = scene->createnode()
	setmesh
	setmatrix
	setshaderprogram
	settexture
	/**/

	//mgl::SceneNode* obj = Scene->createNode();
}



/*
void MyApp::updateScenegraph(double elapsed) {
	float Red = 0.0f, Green = 0.0f, Blue = 0.0f;
	const float ColorDelta = 0.01f;
	bool color_changed = false;
	if (mgl::KeyBuffer::getInstance().isKeyDown(GLFW_KEY_Q)) {
		Red += ColorDelta;
		color_changed = true;
	}
	if (mgl::KeyBuffer::getInstance().isKeyDown(GLFW_KEY_A)) {
		Red -= ColorDelta;
		color_changed = true;
	}
	if (mgl::KeyBuffer::getInstance().isKeyDown(GLFW_KEY_W)) {
		Green += ColorDelta;
		color_changed = true;
	}
	if (mgl::KeyBuffer::getInstance().isKeyDown(GLFW_KEY_S)) {
		Green -= ColorDelta;
		color_changed = true;
	}
	if (mgl::KeyBuffer::getInstance().isKeyDown(GLFW_KEY_E)) {
		Blue += ColorDelta;
		color_changed = true;
	}
	if (mgl::KeyBuffer::getInstance().isKeyDown(GLFW_KEY_D)) {
		Blue -= ColorDelta;
		color_changed = true;
	}
	if (color_changed) {
		Red = Red > 1.0f ? 1.0f : Red < 0.0f ? 0.0f : Red;
		Green = Green > 1.0f ? 1.0f : Green < 0.0f ? 0.0f : Green;
		Blue = Blue > 1.0f ? 1.0f : Blue < 0.0f ? 0.0f : Blue;

		std::cout << "(" << Red << "," << Green << "," << Blue << ")" << std::endl;

		mgl::ShaderProgram* program =
			mgl::ShaderProgramManager::getInstance().get("shader");
		program->bind();
		glUniform4f(program->Uniforms["Color"].index, Red, Green, Blue, 1.0f);
		program->unbind();
	}
}
*/


///////////////////////////////////////////////////////////////////////// MESHES

mgl::Mesh* MyApp::createMeshes(std::string mesh_obj) {
	std::string mesh_dir = "./assets/models/";

	std::string mesh_obj_fullname = mesh_dir + mesh_obj;

	mgl::Mesh* m = nullptr;
	m = new mgl::Mesh();
	m->generateNormals();

	m->joinIdenticalVertices();
	m->create(mesh_obj_fullname);
	return m;
	//mgl::MeshManager::getinstance.add("ball_mesh", Mesh);
}


///////////////////////////////////////////////////////////////////////// SHADER

mgl::ShaderProgram* MyApp::createShaderProgram(std::string vs, std::string fs) {
	mgl::ShaderProgram* s = new mgl::ShaderProgram();
	s->addShader(GL_VERTEX_SHADER, vs);
	s->addShader(GL_FRAGMENT_SHADER, fs);

	s->addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);

	if (Mesh->hasNormals()) {
		s->addAttribute(mgl::NORMAL_ATTRIBUTE, mgl::Mesh::NORMAL);
	}
	if (Mesh->hasTexcoords()) {
		s->addAttribute(mgl::TEXCOORD_ATTRIBUTE, mgl::Mesh::TEXCOORD);
	}
	if (Mesh->hasTangentsAndBitangents()) {
		s->addAttribute(mgl::TANGENT_ATTRIBUTE, mgl::Mesh::TANGENT);
	}

	s->addUniform(mgl::MODEL_MATRIX);
	s->addUniform(mgl::COLOR_ATTRIBUTE);
	s->addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);
	s->create();

	ModelMatrixId = s->Uniforms[mgl::MODEL_MATRIX].index;
	ColorId = s->Uniforms[mgl::COLOR_ATTRIBUTE].index;
	return s;
}

///////////////////////////////////////////////////////////////////////// CAMERA

// Perspective Fovy(30) Aspect(640/480) NearZ(1) FarZ(50)
float fov = 30.0;
glm::mat4 ProjectionMatrix =
glm::perspective(glm::radians(fov), 640.0f / 480.0f, 1.0f, 100.0f);

mgl::Camera* MyApp::createCamera(glm::mat4 ViewMatrix) {
	mgl::Camera* c = nullptr;
	c = new mgl::Camera(UBO_BP);
	c->setViewMatrix(glm::translate(ViewMatrix * glm::mat4(ViewMatrix_rotation), -ViewMatrix_center));
	c->setProjectionMatrix(ProjectionMatrix);
	return c;
}


const float THRESHOLD = static_cast<float>(1.0e-3);


void MyApp::drawScene() {

	//glBindVertexArray(VaoId);
	for (mgl::SceneNode* node : SceneGraph->RootNode->Nodes) {
		if (!(node->Nodes.empty()))
			node->draw();
	}
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

bool moveObject = false;
mgl::SceneNode* selectedObject = nullptr;
void MyApp::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	MyApp* myApp = static_cast<MyApp*>(glfwGetWindowUserPointer(window));

	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		if (action == GLFW_REPEAT) {
			//std::cout << "Being pressed" << "\n";
		}
		if (selectedObject != nullptr) { //CHECK IF OBJECT IS SELECTED
			glm::mat4 c = glm::inverse(Camera->getViewMatrix());
			glm::vec3 cameraRight = glm::vec3(c[0]);
			glm::vec3 cameraForward = glm::vec3(c[2]);

			cameraForward.y = 0;
			cameraRight.y = 0;

			glm::vec3 right = cameraRight * (0.2f * (int(key == GLFW_KEY_RIGHT) - int(key == GLFW_KEY_LEFT)));
			glm::vec3 depth = -cameraForward * (0.2f * (int(key == GLFW_KEY_UP) - int(key == GLFW_KEY_DOWN)));

			selectedObject->move(depth + right);
			for (mgl::SceneNode* node : selectedObject->Nodes) {
				node->move(depth + right);
			}

		}
		if (key == GLFW_KEY_LEFT_CONTROL) {
			moveObject = true;
		}
		if (key == GLFW_KEY_S) {
			saveSceneGraph();
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

	//NEED CORRECT OBJECT
	selectedObject = SceneGraph->RootNode;

	unsigned char pixelColor[3];
	glReadPixels((GLint)lastX, viewport[3] - (GLint)lastY, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixelColor);
	glm::vec3 pixel(pixelColor[0] / 255.0f, pixelColor[1] / 255.0f, pixelColor[2] / 255.0f);

	if (!(glm::all(glm::equal(pixel, glm::vec3(0.1f, 0.1f, 0.3f), 0.01f)))) {
		std::cout << glm::to_string(pixel) << "\n";
	}
}

glm::vec3 MyApp::getLookAt() {
	mgl::SceneNode* ball = SceneGraph->RootNode->getNode("Ball");

	GLfloat modelMatrix[16];
	glGetUniformfv(ball->Shader->ProgramId, 0, modelMatrix);

	return glm::vec3(glm::make_mat4(modelMatrix)[3]);
}

void MyApp::cursorCallback(GLFWwindow* win, double xpos, double ypos) {
	if (moveObject && glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		getObject();
	}
	else if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
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

void MyApp::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	float offset = -(float)yoffset;
	ViewMatrix_position = glm::inverse(ViewMatrix)[3];
	float dist = glm::length(ViewMatrix_position - ViewMatrix_center);

	if (dist > 10.0f && offset < 1 || offset > 1 && dist < 50.0f || (dist > 10.0f && dist < 50.0f)) {
		ViewMatrix_position += (ViewMatrix_position)*offset * 0.05f;

		ViewMatrix = glm::lookAt(ViewMatrix_position, glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));
		Camera->setViewMatrix(glm::translate(ViewMatrix * glm::mat4(ViewMatrix_rotation), -ViewMatrix_center));
	}
}

void MyApp::saveSceneGraph() {
	//SAVE MATRIXES
	// 
	// 
	//SAVETEXTURES
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
