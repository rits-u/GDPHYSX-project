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
			float damping = 0.9f;

		protected:
			MyVector accumulatedForce = MyVector(0, 0, 0);

			bool isDestroyed = false;

		protected:
			void UpdatePosition(float time);
			void UpdateVelocity(float time);
			

		public:
			void Update(float time);
			void Destroy();
			bool IsDestroyed();
			bool IsAtOrigin();
			void AddForce(MyVector force); //adds force to particle
			void ResetForce();
	};
}

