#include "PhysicsWorld.h"
using namespace P6;

void PhysicsWorld::AddParticle(P6Particle* toAdd) {
	Particles.push_back(toAdd);	
}

void PhysicsWorld::Update(float time) {
	UpdateParticleList();

	for (std::list<P6Particle*>::iterator p = Particles.begin();

		p != Particles.end();

		p++
		)
	{
		(*p)->Update(time);
	}
}

void PhysicsWorld::CheckIfDone() {
	for (std::list<P6Particle*>::iterator p = Particles.begin();

		p != Particles.end();

		p++
		)
	{
		(*p)->IsAtOrigin();
	}
}

void PhysicsWorld::UpdateParticleList() {
	Particles.remove_if(
		[](P6Particle* p) {
			return p->IsDestroyed();
		}
		//lambda expression
	);
}