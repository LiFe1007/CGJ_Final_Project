////////////////////////////////////////////////////////////////////////////////
//
// Basic Shaders
//
// Copyright (c) 2013-23 by Carlos Martinho
//
// INTRODUCES:
// Basic shaders
//
////////////////////////////////////////////////////////////////////////////////

#include <iomanip>
#include <iostream>

#include "../mgl/mgl.hpp"

/////////////////////////////////////////////////////////////////////////// BASE

class MyApp_Base : public mgl::ViewerApp {
private:
	void createScenegraph() override;
};

void MyApp_Base::createScenegraph() {
	mgl::SceneNode* obj = Scene->createNode();
	obj->setMesh(mgl::MeshManager::getInstance().get("model"));
	obj->setMatrix(glm::scale(glm::vec3(3.0f)));
	obj->setShaderProgram(mgl::ShaderProgramManager::getInstance().get("shader"));
}

//////////////////////////////////////////////////////////////////////// MINIMUM

class MyApp_Minimum : public MyApp_Base {
private:
	void createMeshes() override;
	void createShaderPrograms() override;
};

void MyApp_Minimum::createMeshes() {
	mgl::Mesh* mesh = new mgl::Mesh();
	mesh->create("../assets/models/torus-vtn-flat.obj");
	mgl::MeshManager::getInstance().add("model", mesh);
}

void MyApp_Minimum::createShaderPrograms() {
	mgl::ShaderProgram* program = new mgl::ShaderProgram();
	program->addShader(GL_VERTEX_SHADER, "basic-minimum-vs.glsl");
	program->addShader(GL_FRAGMENT_SHADER, "basic-minimum-fs.glsl");
	program->addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);
	program->addUniform(mgl::MODEL_MATRIX);
	program->addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);
	program->create();
	mgl::ShaderProgramManager::getInstance().add("shader", program);
}

///////////////////////////////////////////////////////////////////// PROJECTION

class MyApp_Projection : public MyApp_Base {
private:
	void createMeshes() override;
	void createShaderPrograms() override;
};

void MyApp_Projection::createMeshes() {
	mgl::Mesh* mesh = new mgl::Mesh();
	mesh->create("../assets/models/torus-vtn-flat.obj");
	mgl::MeshManager::getInstance().add("model", mesh);
}

void MyApp_Projection::createShaderPrograms() {
	mgl::ShaderProgram* program = new mgl::ShaderProgram();
	program->addShader(GL_VERTEX_SHADER, "basic-projection-vs.glsl");
	program->addShader(GL_FRAGMENT_SHADER, "basic-projection-fs.glsl");
	program->addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);
	program->addUniform(mgl::MODEL_MATRIX);
	program->addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);
	program->create();
	mgl::ShaderProgramManager::getInstance().add("shader", program);
}
/////////////////////////////////////////////////////////////////////// VSPARAMS

class MyApp_VSParams : public MyApp_Base {
private:
	void createMeshes() override;
	void createShaderPrograms() override;
	void updateScenegraph(double elapsed) override;

	float ModelScale = 1.0f;
	const float ModelScaleDelta = 0.05f;
};

void MyApp_VSParams::createMeshes() {
	mgl::Mesh* mesh = new mgl::Mesh();
	mesh->create("../assets/models/torus-vtn-flat.obj");
	mgl::MeshManager::getInstance().add("model", mesh);
}

void MyApp_VSParams::createShaderPrograms() {
	mgl::ShaderProgram* program = new mgl::ShaderProgram();
	program->addShader(GL_VERTEX_SHADER, "basic-vsparams-vs.glsl");
	program->addShader(GL_FRAGMENT_SHADER, "basic-vsparams-fs.glsl");
	program->addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);
	program->addUniform(mgl::MODEL_MATRIX);
	program->addUniform("ModelScale");
	program->addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);
	program->create();
	mgl::ShaderProgramManager::getInstance().add("shader", program);

	program->bind();
	glUniform1f(program->Uniforms["ModelScale"].index, ModelScale);
	program->unbind();
}

void MyApp_VSParams::updateScenegraph(double elapsed) {
	bool model_scale_changed = false;
	if (mgl::KeyBuffer::getInstance().isKeyDown(GLFW_KEY_W)) {
		ModelScale += ModelScaleDelta;
		model_scale_changed = true;
	}
	if (mgl::KeyBuffer::getInstance().isKeyDown(GLFW_KEY_S)) {
		ModelScale -= ModelScaleDelta;
		model_scale_changed = true;
	}
	if (model_scale_changed) {
		mgl::ShaderProgram* program =
			mgl::ShaderProgramManager::getInstance().get("shader");
		program->bind();
		glUniform1f(program->Uniforms["ModelScale"].index, ModelScale);
		program->unbind();
	}
}

//////////////////////////////////////////////////////////////////////// DYNAMIC

class MyApp_Dynamic : public MyApp_Base {
private:
	void createMeshes() override;
	void createShaderPrograms() override;
	void updateScenegraph(double elapsed) override;
};

void MyApp_Dynamic::createMeshes() {
	mgl::Mesh* mesh = new mgl::Mesh();
	mesh->create("../assets/models/torus-vtn-flat.obj");
	mgl::MeshManager::getInstance().add("model", mesh);
}

void MyApp_Dynamic::createShaderPrograms() {
	mgl::ShaderProgram* program = new mgl::ShaderProgram();
	program->addShader(GL_VERTEX_SHADER, "basic-dynamic-vs.glsl");
	program->addShader(GL_FRAGMENT_SHADER, "basic-dynamic-fs.glsl");
	program->addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);
	program->addUniform(mgl::MODEL_MATRIX);
	program->addUniform("Time0_1");
	program->addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);
	program->create();
	mgl::ShaderProgramManager::getInstance().add("shader", program);
}

void MyApp_Dynamic::updateScenegraph(double elapsed) {
	float k = static_cast<float>(fmod(mgl::Simulation::getInstance().Time, 1.0));

	mgl::ShaderProgram* program =
		mgl::ShaderProgramManager::getInstance().get("shader");
	program->bind();
	glUniform1f(program->Uniforms["Time0_1"].index, k);
	program->unbind();
}

/////////////////////////////////////////////////////////////////////// FSPARAMS

class MyApp_FsParams : public MyApp_Base {
private:
	void createMeshes() override;
	void createShaderPrograms() override;
	void updateScenegraph(double elapsed) override;
	float Red = 0.0f, Green = 0.0f, Blue = 0.0f;
	const float ColorDelta = 0.01f;
};

void MyApp_FsParams::createMeshes() {
	mgl::Mesh* mesh = new mgl::Mesh();
	mesh->create("../assets/models/torus-vtn-flat.obj");
	mgl::MeshManager::getInstance().add("model", mesh);
}

void MyApp_FsParams::createShaderPrograms() {
	mgl::ShaderProgram* program = new mgl::ShaderProgram();
	program->addShader(GL_VERTEX_SHADER, "basic-fsparams-vs.glsl");
	program->addShader(GL_FRAGMENT_SHADER, "basic-fsparams-fs.glsl");
	program->addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);
	program->addUniform(mgl::MODEL_MATRIX);
	program->addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);
	program->addUniform("Color");
	program->create();
	mgl::ShaderProgramManager::getInstance().add("shader", program);

	Red = 1.0f;
	Green = 0.5f;
	Blue = 0.7f;
	program->bind();
	glUniform4f(program->Uniforms["Color"].index, Red, Green, Blue, 1.0f);
	program->unbind();
}

void MyApp_FsParams::updateScenegraph(double elapsed) {
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

		std::cout << std::setprecision(2);
		std::cout << "(" << Red << "," << Green << "," << Blue << ")" << std::endl;

		mgl::ShaderProgram* program =
			mgl::ShaderProgramManager::getInstance().get("shader");
		program->bind();
		glUniform4f(program->Uniforms["Color"].index, Red, Green, Blue, 1.0f);
		program->unbind();
	}
}

//////////////////////////////////////////////////////////////////// VS2FSPARAMS

class MyApp_Vs2FsParams : public MyApp_Base {
private:
	void createMeshes() override;
	void createShaderPrograms() override;
};

void MyApp_Vs2FsParams::createMeshes() {
	mgl::Mesh* mesh = new mgl::Mesh();
	mesh->create("../assets/models/cube-vtn.obj");
	mgl::MeshManager::getInstance().add("model", mesh);
}

void MyApp_Vs2FsParams::createShaderPrograms() {
	mgl::ShaderProgram* program = new mgl::ShaderProgram();
	program->addShader(GL_VERTEX_SHADER, "basic-vs2fsparams-vs.glsl");
	program->addShader(GL_FRAGMENT_SHADER, "basic-vs2fsparams-fs.glsl");
	program->addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);
	program->addUniform(mgl::MODEL_MATRIX);
	program->addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);
	program->create();
	mgl::ShaderProgramManager::getInstance().add("shader", program);
}

///////////////////////////////////////////////////////////////////////// NCOLOR

class MyApp_NColor : public MyApp_Base {
private:
	void createMeshes() override;
	void createShaderPrograms() override;
};

void MyApp_NColor::createMeshes() {
	mgl::Mesh* mesh = new mgl::Mesh();
	mesh->generateSmoothNormals();
	mesh->create("../assets/models/cube-vtn.obj");
	// mesh->create("../assets/models/sphere-vtn-flat.obj");
	// mesh->create("../assets/models/sphere-vtn-smooth.obj");
	// mesh->create("../assets/models/suzanne-vtn-flat.obj");
	// mesh->create("../assets/models/teapot-vn-smooth.obj");
	// mesh->create("../assets/models/elephant-flat.obj");
	// mesh->create("../assets/models/ninja-flat.obj");
	// mesh->create("../assets/models/bunny-vn-flat.obj");
	mgl::MeshManager::getInstance().add("model", mesh);
}

void MyApp_NColor::createShaderPrograms() {
	mgl::ShaderProgram* program = new mgl::ShaderProgram();
	program->addShader(GL_VERTEX_SHADER, "basic-ncolor-vs.glsl");
	program->addShader(GL_FRAGMENT_SHADER, "basic-ncolor-fs.glsl");
	program->addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);
	program->addAttribute(mgl::NORMAL_ATTRIBUTE, mgl::Mesh::NORMAL);
	program->addUniform(mgl::MODEL_MATRIX);
	program->addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);
	program->create();
	mgl::ShaderProgramManager::getInstance().add("shader", program);
}

////////////////////////////////////////////////////////////////////////// FINAL

class MyApp_Final : public MyApp_Base {
private:
	void createMeshes() override;
	void createShaderPrograms() override;
	void updateScenegraph(double elapsed) override;
};

void MyApp_Final::createMeshes() {
	mgl::Mesh* mesh = new mgl::Mesh();
	mesh->create("../assets/models/torus-vtn-flat.obj");
	// mesh->create("../assets/models/sphere-vtn-flat.obj");
	mgl::MeshManager::getInstance().add("model", mesh);
}

void MyApp_Final::createShaderPrograms() {
	mgl::ShaderProgram* program = new mgl::ShaderProgram();
	program->addShader(GL_VERTEX_SHADER, "basic-final-vs.glsl");
	program->addShader(GL_FRAGMENT_SHADER, "basic-final-fs.glsl");
	program->addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);
	program->addAttribute(mgl::NORMAL_ATTRIBUTE, mgl::Mesh::NORMAL);
	program->addUniform(mgl::MODEL_MATRIX);
	program->addUniform("Time0_1");
	program->addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);
	program->create();
	mgl::ShaderProgramManager::getInstance().add("shader", program);
}

void MyApp_Final::updateScenegraph(double elapsed) {
	float k = static_cast<float>(fmod(mgl::Simulation::getInstance().Time, 1.0));

	mgl::ShaderProgram* program =
		mgl::ShaderProgramManager::getInstance().get("shader");
	program->bind();
	glUniform1f(program->Uniforms["Time0_1"].index, k);
	program->unbind();
}

/////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char* argv[]) {
	mgl::Engine& engine = mgl::Engine::getInstance();
	engine.setApp(new MyApp_Minimum());
	// engine.setApp(new MyApp_Projection());
	// engine.setApp(new MyApp_VSParams());
	// engine.setApp(new MyApp_Dynamic());
	// engine.setApp(new MyApp_FsParams());
	// engine.setApp(new MyApp_Vs2FsParams());
	// engine.setApp(new MyApp_NColor());
	// engine.setApp(new MyApp_Final());
	engine.setOpenGL(4, 6);
	engine.setWindow(800, 600, "Basic shaders", 0, 1);
	engine.init();
	engine.run();
	exit(EXIT_SUCCESS);
}

////////////////////////////////////////////////////////////////////////////////
