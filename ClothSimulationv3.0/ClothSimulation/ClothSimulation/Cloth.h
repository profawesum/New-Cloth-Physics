#pragma once

//lib includes
#include <glut.h> 
#include <math.h>
#include <vector>
#include <iostream>

//local includes
#include "Vec3.h"
#include "Particle.h"
#include "Constraint.h"

#define DAMPING 0.01f
#define TIME_STEPSIZE2 0.5f*0.5f
#define CONSTRAINT_ITERATIONS 15

//cloth class that manages the main part of the cloth
class Cloth
{
private:

	//Total number of particles = height * width
	//how many particles in the horizontal direction
	int num_particles_width;
	//how many particles in the vertical direction
	int num_particles_height;

	//vector of particles that make up the cloth
	std::vector<Particle> particles;
	//all of the constraints that the particles have for the cloth
	std::vector<Constraint> constraints;

	Particle* getParticle(int x, int y) { return &particles[y*num_particles_width + x]; }

	void makeConstraint(Particle *p1, Particle *p2) { constraints.push_back(Constraint(p1, p2)); }

	//calculates the normals of the triangle based on the three particles passed through
	Vec3 calcTriangleNormal(Particle *p1, Particle *p2, Particle *p3);

	//calculate the wind force for a triangle
	void addWind(Particle *p1, Particle *p2, Particle *p3, const Vec3 direction);

	//draws a triangle with a set color
	void drawTriangle(Particle *p1, Particle *p2, Particle *p3, const Vec3 color);

public:

	//creates the particle and constraint system for the cloth.
	Cloth(float width, float height, int num_particles_width, int num_particles_height);

	//renders the cloth and calls some other render functions as well
	void renderCloth();

	//calls timeStep for particles and makes sure the constraints are satisfied
	void timeStep();

	//adds force to the particles, mainly gravity
	void addForce(const Vec3 direction);

	//adds wind in a set direction
	void windForce(const Vec3 direction);

	//this checks collision with the cloth and another object
	void ballCollision(const Vec3 center, const float radius);

	// this makes it so all of the cloth particles are movable so it will drop to the floor
	void dropCloth();


};