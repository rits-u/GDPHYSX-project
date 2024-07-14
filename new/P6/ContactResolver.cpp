#include "ContactResolver.h"
using namespace P6;

void ContactResolver::ResolveContacts(std::vector<ParticleContact*> contacts, float time) {
	current_iterations = 0;

	while (current_iterations < max_iterations) {
		unsigned current_index = 0;
		float curr_min = contacts[0]->GetSeparatingSpeed();
		float max_depth = contacts[0]->depth;
		
		//for(int i = 1; 1 < )

		//finish this
	}
}