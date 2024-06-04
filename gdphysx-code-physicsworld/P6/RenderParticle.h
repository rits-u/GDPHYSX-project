#pragma once

#include "P6Particle.h"
#include "../Class/Model3D.h"

namespace P6 {
	class RenderParticle
	{
		public:
			P6Particle* PhysicsParticle;
			Model3D* RenderObject;

			RenderParticle(P6::P6Particle* p, Model3D* obj) : PhysicsParticle(p), RenderObject(obj) {}
			
			void Draw(std::vector<GLuint> mesh_indices, GLuint shaderProg, GLuint* VAO);


	};
}
