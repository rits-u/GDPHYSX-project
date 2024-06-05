#include "P6Particle.h"
using namespace P6;


void P6Particle::UpdatePosition(float time) {
	this->Position = this->Position + (this->Velocity * time) + ((this->Acceleration * time * time) * (1.0f / 2.0f));
}


void P6Particle::UpdateVelocity(float time) {
	this->Velocity = this->Velocity + (this->Acceleration * time);
}

void P6Particle::Update(float time) {
	this->UpdatePosition(time);
	this->UpdateVelocity(time);
}

void P6Particle::Destroy() {
	this->isDestroyed = true;
}

bool P6Particle::IsDestroyed() {
	return isDestroyed;
}

bool P6Particle::IsAtOrigin() {

	if ((this->Position.x >= -10.0f && this->Position.x <= 10.0f) &&
		(this->Position.y >= -10.0f && this->Position.y <= 10.0f)) {
			
			return true;
	}
	return false;
}