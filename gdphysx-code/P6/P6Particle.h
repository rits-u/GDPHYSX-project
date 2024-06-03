#pragma once

#include "MyVector.h"

namespace P6 {

	class P6Particle
	{
		public:
			float mass = 0;
			MyVector Position;
			MyVector Velocity;
			MyVector Acceleration;

		protected:
			void UpdatePosition(float time);
			void UpdateVelocity(float time);

		public:
			void Update(float time);
	};
}

