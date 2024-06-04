#include "RenderParticle.h"
using namespace P6;

void RenderParticle::Draw(std::vector<GLuint> mesh_indices, GLuint shaderProg, GLuint* VAO) {
	if (!PhysicsParticle->IsDestroyed()) {
		RenderObject->setPosition(PhysicsParticle->Position);
		RenderObject->drawModel(mesh_indices, shaderProg, VAO);
	}
}
