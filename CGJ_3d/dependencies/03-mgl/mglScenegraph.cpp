////////////////////////////////////////////////////////////////////////////////
//
// Shader Program Class
//
// Copyright (c)2022-23 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#include "./mglScenegraph.hpp"

#include <mgl.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cassert>
#include <fstream>
#include <sstream>
#include "stb_image.h"




namespace mgl {

	////////////////////////////////////////////////////////////////// SceneNode    
	SceneNode::SceneNode() {}
	SceneNode::SceneNode(std::string name) { NodeName = name; }
	SceneNode::~SceneNode() {}

	SceneNode* SceneNode::getInstance(void) {
		;
		return this;
	}

	void SceneNode::create(mgl::SceneNode* parent, mgl::Texture2D* texture, std::string mesh, std::string shadervs, std::string shaderfs) {

		Parent = parent;
		Texture2D = texture;
		Mesh = createMeshes(mesh);
		Shader = createShaderProgram(shadervs, shaderfs);
	}

	void SceneNode::add(mgl::SceneNode* node) {
		Nodes.push_back(node);
	}

	void SceneNode::draw() {
		if (Shader != nullptr && Mesh != nullptr) {
			Shader->bind();
			/* Read the framebuffer into our storage */

			Mesh->draw();

			Shader->unbind();
		}
		for (SceneNode* node : Nodes) {
			node->draw();
		}
	}

	void SceneNode::move(glm::vec3 movement) {
		if (Shader != nullptr && Mesh != nullptr) {

			// Variable to store retrieved matrix
			GLfloat modelMatrix[16];

			// Retrieve uniform value
			glGetUniformfv(Shader->ProgramId, 0, modelMatrix);

			glm::mat4 new_position = glm::make_mat4(modelMatrix) * glm::translate(glm::mat4(1.0f), movement);

			Shader->bind();
			glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(new_position));
			Shader->unbind();
		}
		for (SceneNode* node : Nodes) {
			node->move(movement);
		}
	}

	SceneNode* SceneNode::getNode(std::string name) {
		if (NodeName == name) {
			return getInstance();
		}
		for (SceneNode* node : Nodes) {
			SceneNode* foundNode = node->getNode(name);
			if (foundNode != nullptr) {
				return foundNode;
			}
		}
		return nullptr;
	}

	void SceneNode::saveTexture(const std::string& filename) {

		// Read texture data
		std::vector<unsigned char> buffer(Texture2D->texWidth * Texture2D->texHeight * 4);
		glBindTexture(GL_TEXTURE_2D, Texture2D->id);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer.data());

		// Save file
		FILE* file;
		if (fopen_s(&file, filename.c_str(), "wb") == 0) {
			fwrite(buffer.data(), 1, buffer.size(), file);
			fclose(file);
		}
	}

	///////////////////////////////////////////////////////////////////////// MESHES

	mgl::Mesh* SceneNode::createMeshes(std::string mesh_obj) {
		MeshLocation = mesh_obj;
		std::string mesh_dir = "./assets/models/";

		std::string mesh_obj_fullname = mesh_dir + mesh_obj;

		mgl::Mesh* m = nullptr;
		m = new mgl::Mesh();
		m->generateNormals();

		m->joinIdenticalVertices();
		m->create(mesh_obj_fullname);
		return m;
	}

	///////////////////////////////////////////////////////////////////////// SHADER

	mgl::ShaderProgram* SceneNode::createShaderProgram(std::string vs, std::string fs) {
		ShaderVS = vs;
		ShaderFS = fs;
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

		s->bind();
		glUniformMatrix4fv(ModelMatrixId, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
		s->unbind();

		return s;
	}

	void SceneNode::changeShaderColor(glm::vec3 color) {
		Shader->bind();
		glUniform3fv(ColorId, 1, glm::value_ptr(color));
		Shader->unbind();
	}



	//////////////////////////////////////////////////////////////////// GraphNode
	SceneGraph::SceneGraph() {}
	SceneGraph::~SceneGraph() {}


	glm::vec3 ViewMatrix_position;
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

	void SceneGraph::create(mgl::SceneNode* node, mgl::Camera* camera, glm::mat4 viewMatrix, glm::vec3 viewMatrix_position, glm::quat viewMatrix_rotation) {
		RootNode = node;
		Camera = camera;
		ViewMatrix = viewMatrix;
		ViewMatrix_position = viewMatrix_position;
		ViewMatrix_rotation = viewMatrix_rotation;
	}

	void SceneGraph::save(const std::string& filename) {

		std::ofstream outfile(filename);

		// Write root node
		outfile << glm::to_string(ViewMatrix) << std::endl;
		outfile << glm::to_string(ViewMatrix_position) << std::endl;
		outfile << glm::to_string(ViewMatrix_rotation) << std::endl;
		//CreateCamera

		// Recursively write all nodes
		saveNode(RootNode, outfile);

		outfile << "__END__" << std::endl;

		outfile.close();
	}

	void SceneGraph::saveNode(SceneNode* node, std::ofstream& outfile) {
		if (node->NodeName != "Root") {
			outfile << node->NodeName << std::endl;
		}

		if (node->Texture2D != nullptr) {
			//node->saveTexture("./assets/saves/" + node->NodeName + "Tex.png");
			outfile << "__TEXTURE__" << std::endl;
			outfile << "./assets/saves/" + node->NodeName + "Tex.png" << std::endl;
		}

		if (node->Mesh != nullptr) {
			outfile << "__MESH__" << std::endl;
			outfile << node->MeshLocation << std::endl;
		}

		if (node->Shader != nullptr) {
			outfile << "__SHADER__" << std::endl;
			outfile << node->ShaderVS << std::endl;
			outfile << node->ShaderFS << std::endl;
			GLfloat modelMatrix[16];
			glGetUniformfv(node->Shader->ProgramId, 0, modelMatrix);
			outfile << glm::to_string(glm::make_mat4(modelMatrix)) << std::endl;
		}

		if (node->NodeName != "Root") {
			outfile << "__NEXT__" << std::endl;
		}

		outfile << node->Nodes.size() << std::endl;

		for (SceneNode* Node : node->Nodes) {
			saveNode(Node, outfile);
		}
	}


	void SceneGraph::load(const std::string& filename) {

		std::ifstream infile(filename);

		std::string line;

		getline(infile, line);		//mat4
		ViewMatrix = glm::make_mat4(stringToGlm(line).data());
		getline(infile, line);		//vec3
		ViewMatrix_position = glm::make_vec3(stringToGlm(line).data());
		getline(infile, line);		//quat
		std::vector<float> r = stringToGlm(line);
		std::rotate(r.begin(),r.begin() + 1,r.end());
		ViewMatrix_rotation = glm::make_quat(r.data());


		// Create root
		RootNode = new SceneNode("Root");

		// Recursively load nodes 
		loadNode(RootNode, infile);

	}

	void SceneGraph::loadNode(SceneNode* parent, std::ifstream& infile) {
		std::string line;

		std::string name;
		Texture2D* newTexture = nullptr;
		std::string meshLocation;
		std::string shadervs;
		std::string shaderfs;
		glm::vec3 objPosition;


		getline(infile, line);
		int numberNodes = stoi(line);

		if (numberNodes == 0)
			return;

		getline(infile, name);
		getline(infile, line);

		if (line == "__TEXTURE__") {
			getline(infile, line);
			newTexture = nullptr;
			//Texture2D* newTexture = new mgl::Texture2D;
			//newTexture->load(line);
			getline(infile, line);
		}
		if (line == "__MESH__") {
			getline(infile, line);
			meshLocation = line;
			getline(infile, line);
		}
		if (line == "__SHADER__") {
			getline(infile, line);
			shadervs = line;
			getline(infile, line);
			shaderfs = line;
			getline(infile, line);
			objPosition = glm::make_mat4(stringToGlm(line).data())[3];
			getline(infile, line);
		}


		SceneNode* node = new SceneNode(name);
		if (!meshLocation.empty() && !shadervs.empty() && !shaderfs.empty()) {
			node->create(parent, newTexture, meshLocation, shadervs, shaderfs);
			node->move(objPosition);
			node->changeShaderColor(glm::vec3(0.7f, 0.7f, 0.7f));
		}

		while (numberNodes != 0) {
			loadNode(node, infile);
			numberNodes--;
		}

		parent->add(node);
	}

	std::vector<float> SceneGraph::stringToGlm(std::string line) {

		line = line.substr(line.find("(") + 1);

		line.erase(remove(line.begin(), line.end(), '('), line.end());
		line.erase(remove(line.begin(), line.end(), ')'), line.end());
		line.erase(remove(line.begin(), line.end(), '{'), line.end());
		line.erase(remove(line.begin(), line.end(), '}'), line.end());

		std::istringstream iss(line);

		std::string token;
		std::vector<float> values;

		while (iss >> token) {
			values.push_back(std::stof(token));
		}
		return values;
	}
}  // namespace mgl
