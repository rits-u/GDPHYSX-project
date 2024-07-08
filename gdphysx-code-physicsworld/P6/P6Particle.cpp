#include "P6Particle.h"
using namespace P6;


void P6Particle::UpdatePosition(float time) {
	this->Position = this->Position + (this->Velocity * time) + ((this->Acceleration * time * time) * (1.0f / 2.0f));
}


void P6Particle::UpdateVelocity(float time) {
	//A                 =        F         /    m
	this->Acceleration += accumulatedForce * (1/mass);

	//Vf           =        V1      +         A
	this->Velocity = this->Velocity + (this->Acceleration * time);

	//Vfd          =         Vf     *       d ^ t
	this->Velocity = this->Velocity * powf(damping, time);
}

void P6Particle::Update(float time) {
	this->UpdatePosition(time);
	this->UpdateVelocity(time);

	this->ResetForce();
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

void P6Particle::AddForce(MyVector force) {
	accumulatedForce += force;
}

void P6Particle::ResetForce() {
	this->accumulatedForce = MyVector(0, 0, 0);
	this->Acceleration = MyVector(0, 0, 0);
}