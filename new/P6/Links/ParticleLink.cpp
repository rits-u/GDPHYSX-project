#include "ParticleLink.h"
using namespace P6;

float ParticleLink::CurrentLength() {
	MyVector ret = particles[0]->Position - particles[1]->Position;
	return ret.getMagnitude();
}