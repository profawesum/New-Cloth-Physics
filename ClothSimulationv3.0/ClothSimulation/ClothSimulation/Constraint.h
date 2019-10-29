#pragma once

#include "Particle.h"
#include "Vec3.h"

//constraint class that manages the constraints for the cloth
class Constraint
{
private:
	// the length between particle p1 and p2 in rest configuration
	float rest_distance; 

public:
	//the two particles that are connected through this constraint
	Particle *p1, *p2;

	Constraint(Particle *p1, Particle *p2) : p1(p1), p2(p2) { 
		Vec3 vec = p1->getPos() - p2->getPos(); 
		rest_distance = vec.length(); 
	}

	//called by cloth time step to make sure the particles are constrained
	void satisfyConstraint();
};