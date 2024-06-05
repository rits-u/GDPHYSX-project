#pragma once

#include <vector>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../tiny_obj_loader.h"

#include "../P6/MyVector.h"
using namespace P6;

class Model3D
{
	private:
		MyVector position;
		glm::vec3 scale;
		glm::vec3 axis;
		glm::vec3 theta;
		glm::vec4 color;


	public:
		Model3D();
		Model3D(MyVector position, glm::vec3 scale, glm::vec4 color);

	public:
		std::vector<GLuint> loadModel(std::string objSrc, tinyobj::attrib_t* attributes);
		void bindBuffers(tinyobj::attrib_t attributes, std::vector<GLuint> mesh_indices, GLuint* VAO, GLuint* VBO);
		void drawModel(std::vector<GLuint> mesh_indices, GLuint shaderProg, GLuint* VAO);
		void bindCamera(GLuint shaderProg, glm::mat4 projection, glm::mat4 viewMatrix);
		void bindCamera(GLuint shaderProg, glm::mat4 projection);

	public:
		void setPosition(MyVector position);
		void setScale(glm::vec3 scale);
		void setColor(glm::vec4 color);

};

