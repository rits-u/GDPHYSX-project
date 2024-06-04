#pragma once
#include <list>
#include "P6Particle.h"

namespace P6 {
	class PhysicsWorld
	{
	public:
		std::list<P6Particle*> Particles;

		//function to add particles to the list
		void AddParticle(P6Particle* toAdd);

		//Universal update function to call the updates of ALL
		void Update(float time);

	private:
		//updates the particle list
		void UpdateParticleList();
	};
}
