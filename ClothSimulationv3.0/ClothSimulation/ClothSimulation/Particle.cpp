
#include "Particle.h"

void Particle::timeStep()
{
	//is the particle movable
	if (movable)
	{
		//get a temp pos
		Vec3 temp = pos;
		// using Time-Corrected Verlet Integration Method x1 = x + (x – x0) * dt / dt0 + a * dt^2
		//where x is pos, dt is damping, a is accel and dt^2 is timestep
		pos = pos + (pos - old_pos)*(1.0 - DAMPING) + acceleration * TIME_STEPSIZE2;
		//old pos is equal to temp as that was the old position
		old_pos = temp;
		//reset accel
		acceleration = Vec3(0, 0, 0);
	}
}
