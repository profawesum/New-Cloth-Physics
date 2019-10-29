#pragma once

#include "Vec3.h"

#define DAMPING 0.01f
#define TIME_STEPSIZE2 0.5f*0.5f
#define CONSTRAINT_ITERATIONS 15

//particle class that manages the particles for the cloth
class Particle
{
private:
	//is the particle movable or not, used to pin the cloth
	bool movable;

	//the mass of the particle
	float mass;
	//position of the particle
	Vec3 pos;
	//the old position of the particle
	Vec3 old_pos;
	//the acceleration of the particle
	Vec3 acceleration;
	//non normalized vector
	Vec3 accumulated_normal;

public:

	Particle(Vec3 pos) : pos(pos), old_pos(pos), acceleration(Vec3(0, 0, 0)), mass(1), movable(true), accumulated_normal(Vec3(0, 0, 0)) {}
	Particle() {}

	//adds force to the particle with f= ma equation (a = f/m)
	void addForce(Vec3 f) { acceleration += f / mass; }


	//time step of particle class
	void timeStep();

	Vec3& getPos() { return pos; }

	//resets the acceleration of a particle
	void resetAcceleration() { acceleration = Vec3(0, 0, 0); }
	
	//sets an offset
	void offsetPos(const Vec3 v) { if (movable) pos += v; }
	//makes a particle immovable
	void makeUnmovable() { movable = false; }
	//makes a particle movable
	void makeMovable() { movable = true; }
	//adds to the particles normal
	void addToNormal(Vec3 normal) { accumulated_normal += normal.normalized(); }
	// the normal is not unit length
	Vec3& getNormal() { return accumulated_normal; }
	//resets the normal
	void resetNormal() { accumulated_normal = Vec3(0, 0, 0); }

};