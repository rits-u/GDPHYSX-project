#pragma once

#include "P6/MyVector.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace utility {
	using namespace P6;

	class RenderLine
	{
		public:
			MyVector P1;
			MyVector P2;
			glm::mat4 ProjectionMatrix;

			MyVector Color;

			RenderLine(MyVector p1, MyVector p2, MyVector color) : P1(p1), P2(p2), Color(color) {}

			void Update(MyVector p1, MyVector p2, glm::mat4 projectionMatrix);
			void Draw();
	};

}

