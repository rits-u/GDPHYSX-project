#pragma once
#include <list>
#include "P6Particle.h"
#include "ForceRegistry.h"
#include "GravityForceGenerator.h"

namespace P6 {
	class PhysicsWorld
	{
	public:
		ForceRegistry forceRegistry;

		std::list<P6Particle*> Particles;

		//function to add particles to the list
		void AddParticle(P6Particle* toAdd);

		//Universal update function to call the updates of ALL
		void Update(float time);

		void CheckIfDone();

	private:
		//updates the particle list
		void UpdateParticleList();

		GravityForceGenerator Gravity = GravityForceGenerator(MyVector(0, -9.8, 0));
	};
}
