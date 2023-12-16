////////////////////////////////////////////////////////////////////////////////
//
// Shader Program Class
//
// Copyright (c)2022-23 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#include "./mglScenegraph.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <mgl.hpp>

#include <cassert>
#include <fstream>



namespace mgl {

	////////////////////////////////////////////////////////////////// SceneNode    
	SceneNode::SceneNode() {}
	SceneNode::~SceneNode() {}

	SceneNode& SceneNode::getInstance(void) {
		static SceneNode instance;
		return instance;
	}

	void SceneNode::create(mgl::SceneNode* parent, mgl::Texture* texture, mgl::Mesh* mesh, mgl::ShaderProgram* shader) {
		Parent = parent;
		Texture = texture;
		Mesh = mesh;
		Shader = shader;
	}

	void SceneNode::add(mgl::SceneNode* node) {
		Node.push_back(node);
	}

	void SceneNode::draw() {
		if (Shader != nullptr && Mesh != nullptr) {
			Shader->bind();
			/* Read the framebuffer into our storage */

			Mesh->draw();

			Shader->unbind();
		}
		for (SceneNode* node : Node) {
			node->draw();
		}
	}

	void SceneNode::move(glm::vec3 movement) {
		if (Shader != nullptr && Mesh != nullptr) {

			Shader->bind();
			
			// Variable to store retrieved matrix
			GLfloat modelMatrix[16];

			// Retrieve uniform value
			glGetUniformfv(Shader->ProgramId, 0, modelMatrix);

			glm::mat4 new_position = glm::make_mat4(modelMatrix) *glm::translate(glm::mat4(1.0f),movement);

			glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(new_position));

			Shader->unbind();
		}
		for (SceneNode* node : Node) {
			node->move(movement);
		}
	}

	SceneNode* SceneNode::lookForObject() {
		for (SceneNode* node : Node) {
			if (!(node->Node.empty())) {
				//node->Shader->isUniform();    //Check if correct
				node->lookForObject();
			}
			else
				return nullptr;
		}
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
}  // namespace mgl
