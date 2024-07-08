#pragma once
#include "P6Particle.h"

namespace P6 {
	class ParticleContact
	{
		public:
			float depth; //interpenetration depth

			P6Particle* particles[2];
			float restitution;
			MyVector contactNormal;

			void Resolve(float time);

		protected:
			void ResolveInterpenetration(float time);
			float GetSeparatingSpeed();
			void ResolveVelocity(float time);
	};

}

