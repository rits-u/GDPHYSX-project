#include "PhysicsWorld.h"
using namespace P6;

//this method adds the parameter particle to the field list of particles
void PhysicsWorld::AddParticle(P6Particle* toAdd) {
	Particles.push_back(toAdd);	
	forceRegistry.Add(toAdd, &Gravity); 
}

//this function handles the overall update on every particle added in the list 
void PhysicsWorld::Update(float time) {
	UpdateParticleList();
	forceRegistry.UpdateForces(time);

	int num = 0;

	for (std::list<P6Particle*>::iterator p = Particles.begin();
		p != Particles.end();
		p++
		)
	{
		(*p)->Update(time);
	}

	if (contacts.size() > 0) {
		contactResolver.ResolveContacts(contacts, time);
	}
}

//this function checks if the particle has exceeded their lifespan, if so, call Destroy() function
void PhysicsWorld::CheckLifespan(float time) {
	for (std::list<P6Particle*>::iterator p = Particles.begin();
		p != Particles.end();
		p++)
	{
		if ((*p)->lifespan <= (int)time - (int)(*p)->spawnTime)
			(*p)->Destroy();
	}
}

void PhysicsWorld::AddContact(P6Particle* p1, P6Particle* p2, float restitution, MyVector contactNormal) {
	ParticleContact* toAdd = new ParticleContact();

	toAdd->particles[0] = p1;
	toAdd->particles[1] = p2;
	toAdd->restitution = restitution;
	toAdd->contactNormal = contactNormal;

	contacts.push_back(toAdd);
}

//this function removes destroyed particles from the list
void PhysicsWorld::UpdateParticleList() {
	Particles.remove_if(
		[](P6Particle* p) {
			return p->IsDestroyed();
		}
	);
}