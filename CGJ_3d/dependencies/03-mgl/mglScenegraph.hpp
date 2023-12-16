////////////////////////////////////////////////////////////////////////////////
//
// Scene Management Class
//
// Copyright (c)2022-23 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MGL_SCENEGRAPH_HPP
#define MGL_SCENEGRAPH_HPP

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>


#include <iostream>
#include <vector>

namespace mgl {

	class IDrawable;

	////////////////////////////////////////////////////////////////////// IDrawable

	class IDrawable {
	public:
		virtual void draw(void) = 0;
	};


	/**/

	class Mesh;
	class Texture;
	class ShaderProgram;
	class Camera;

	class SceneNode {
	protected:

	public:
		SceneNode();
		~SceneNode();
		void draw();
		static SceneNode& getInstance();
		void create(SceneNode* parent, Texture* texture, Mesh* mesh, ShaderProgram* shader);
		void add(SceneNode* node);
		void move(glm::vec3 movement);
		SceneNode* lookForObject();

		SceneNode* Parent = nullptr;
		std::vector<mgl::SceneNode*> Nodes;
		Texture* Texture = nullptr;
		Mesh* Mesh = nullptr;
		ShaderProgram* Shader = nullptr;
	};



	class SceneGraph {
	protected:

	public:
		SceneGraph();
		~SceneGraph();

		void create(mgl::SceneNode* node, mgl::Camera* camera, glm::mat4 ViewMatrix, glm::vec3 ViewMatrix_position, glm::quat ViewMatrix_rotation);

		SceneNode* RootNode = nullptr;
		Camera* Camera = nullptr;
		glm::mat4 ViewMatrix;
		glm::vec3 ViewMatrix_position;	
		glm::quat ViewMatrix_rotation;
	};

}  // namespace mgl

#endif /* MGL_SCENEGRAPH_HPP */
