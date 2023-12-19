////////////////////////////////////////////////////////////////////////////////
//
// Scene Management Class
//
// Copyright (c)2022-23 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MGL_SCENEGRAPH_HPP
#define MGL_SCENEGRAPH_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "stb_image.h"
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
	class Texture2D;
	class ShaderProgram;
	class Camera;

	class SceneNode {
	protected:

	public:
		SceneNode();
		SceneNode(std::string NodeName);
		~SceneNode();
		void draw();
		SceneNode* getInstance();
		void create(SceneNode* parent, Texture2D* texture, std::string mesh, std::string shadervs, std::string shaderfs);
		void add(SceneNode* node);
		void move(glm::vec3 movement);
		SceneNode* getNode(std::string name);
		void saveTexture(const std::string& filename);
		mgl::Mesh* createMeshes(std::string mesh_obj);
		mgl::ShaderProgram* createShaderProgram(std::string vs, std::string fs);
		void changeShaderColor(glm::vec4 color);
		SceneNode* getSceneAlpha(float alpha);

		std::string NodeName;
		SceneNode* Parent = nullptr;
		std::vector<mgl::SceneNode*> Nodes;
		Texture2D* Texture2D = nullptr;

		Mesh* Mesh = nullptr;				//MESH
		std::string MeshLocation;
		const GLuint UBO_BP = 0;

		ShaderProgram* Shader = nullptr;	//SHADER
		std::string ShaderVS;
		std::string ShaderFS;
		GLint ModelMatrixId, ColorId;
	};



	class SceneGraph {
	protected:

	public:
		SceneGraph();
		~SceneGraph();

		void create(mgl::SceneNode* node, mgl::Camera* camera, glm::mat4 ViewMatrix, glm::vec3 ViewMatrix_position, glm::quat ViewMatrix_rotation);

		void save(const std::string& filename);
		void saveNode(SceneNode* node, std::ofstream& outfile);
		void load(const std::string& filename);
		void loadNode(SceneNode* parent, std::ifstream& infile);

		std::vector<float> stringToGlm(std::string line);

		SceneNode* RootNode = nullptr;
		Camera* Camera = nullptr;
		glm::mat4 ViewMatrix;
		glm::vec3 ViewMatrix_position;
		glm::quat ViewMatrix_rotation;
	};

}  // namespace mgl

#endif /* MGL_SCENEGRAPH_HPP */
