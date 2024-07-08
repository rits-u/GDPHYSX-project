#include "ParticleContact.h"
using namespace P6;

void ParticleContact::ResolveInterpenetration(float time) {
	if (depth <= 0)
		return;

	float totalMass = (float)1 / particles[0]->mass;
	if (particles[1])
		totalMass += (float)1 / particles[1]->mass;
}

float ParticleContact::GetSeparatingSpeed() {
	MyVector velocity = particles[0]->Velocity;
	if (particles[1]) {
		velocity -= particles[1]->Velocity;
		
		//N
		MyVector position = particles[0]->Position - particles[1]->Position;
		contactNormal = position.getDirection();
	}

	return velocity.scalarProduct(contactNormal);
}

void ParticleContact::ResolveVelocity(float time) {
	//sS
	float separatingSpeed = GetSeparatingSpeed();

	if (separatingSpeed > 0) {
		return;
	}

	float newSS = -restitution * separatingSpeed;
	float deltaSpeed = newSS - separatingSpeed;  //magnitude of delta velocity

	//get the total inverse mass of the colliding particles
	float totalMass = (float)1 / particles[0]->mass;
	if (particles[1])
		totalMass += (float)1 / particles[1]->mass;

	if (totalMass <= 0)
		return;

	float impulse_mag = deltaSpeed / totalMass;
	MyVector Impulse = contactNormal * impulse_mag;

	MyVector V_a = Impulse * ((float)1 / particles[0]->mass);
	MyVector velocity = particles[0]->Velocity + V_a;
	particles[0]->Velocity = velocity;

	if (particles[1]) {
		MyVector V_b = Impulse * ((float)-1 / particles[1]->mass);
		particles[1]->Velocity = particles[1]->Velocity + V_b;
	}
}

void ParticleContact::Resolve(float time) {
	ResolveVelocity(time);
}