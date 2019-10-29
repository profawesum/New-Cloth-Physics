
#include "Cloth.h"

//calculates the normal of each triangle using it's particles
Vec3 Cloth::calcTriangleNormal(Particle *p1, Particle *p2, Particle *p3)
{
	//get the positions of the particles
	Vec3 pos1 = p1->getPos();
	Vec3 pos2 = p2->getPos();
	Vec3 pos3 = p3->getPos();

	//set the 2 vectors
	Vec3 v1 = pos2 - pos1;
	Vec3 v2 = pos3 - pos1;

	//get the cross product of v1 and v2
	return v1.cross(v2);
}

//calculates the windforce based on the triangle particles, in a set direction
void Cloth::addWind(Particle *p1, Particle *p2, Particle *p3, const Vec3 direction)
{
	//get the normal
	Vec3 normal = calcTriangleNormal(p1, p2, p3);
	//normalize it
	Vec3 d = normal.normalized();
	//calculate the force in a set direction
	Vec3 force = normal * (d.dot(direction));
	//actually add force
	p1->addForce(force);
	p2->addForce(force);
	p3->addForce(force);
}

//draw the triangles, called by the main renderCloth function
void Cloth::drawTriangle(Particle *p1, Particle *p2, Particle *p3, const Vec3 color)
{
	glColor3fv((GLfloat*)&color);

	glNormal3fv((GLfloat *) &(p1->getNormal().normalized()));
	glVertex3fv((GLfloat *) &(p1->getPos()));

	glNormal3fv((GLfloat *) &(p2->getNormal().normalized()));
	glVertex3fv((GLfloat *) &(p2->getPos()));

	glNormal3fv((GLfloat *) &(p3->getNormal().normalized()));
	glVertex3fv((GLfloat *) &(p3->getPos()));
}

//create the cloth
Cloth::Cloth(float width, float height, int num_particles_width, int num_particles_height) : num_particles_width(num_particles_width), num_particles_height(num_particles_height)
{
	//set the particle vector size to be equal to the max number of particles in the system
	particles.resize(num_particles_width*num_particles_height); 

	// creating particles in a grid of particles from (0,0,0) to (width,-height,0)
	for (int x = 0; x < num_particles_width; x++)
	{
		for (int y = 0; y < num_particles_height; y++)
		{
			Vec3 pos = Vec3(width * (x / (float)num_particles_width),
				-height * (y / (float)num_particles_height),
				0);
			particles[y*num_particles_width + x] = Particle(pos); // insert particle in column x at y'th row
		}
	}

	// Connecting immediate neighbor particles with constraints (distance 1 and sqrt(2) in the grid)
	for (int x = 0; x < num_particles_width; x++)
	{
		for (int y = 0; y < num_particles_height; y++)
		{
			if (x < num_particles_width - 1) makeConstraint(getParticle(x, y), getParticle(x + 1, y));
			if (y < num_particles_height - 1) makeConstraint(getParticle(x, y), getParticle(x, y + 1));
			if (x < num_particles_width - 1 && y < num_particles_height - 1) makeConstraint(getParticle(x, y), getParticle(x + 1, y + 1));
			if (x < num_particles_width - 1 && y < num_particles_height - 1) makeConstraint(getParticle(x + 1, y), getParticle(x, y + 1));
		}
	}


	// Connecting secondary neighbors with constraints (distance 2 and sqrt(4) in the grid)
	for (int x = 0; x < num_particles_width; x++)
	{
		for (int y = 0; y < num_particles_height; y++)
		{
			if (x < num_particles_width - 2) makeConstraint(getParticle(x, y), getParticle(x + 2, y));
			if (y < num_particles_height - 2) makeConstraint(getParticle(x, y), getParticle(x, y + 2));
			if (x < num_particles_width - 2 && y < num_particles_height - 2) makeConstraint(getParticle(x, y), getParticle(x + 2, y + 2));
			if (x < num_particles_width - 2 && y < num_particles_height - 2) makeConstraint(getParticle(x + 2, y), getParticle(x, y + 2));
		}
	}


	// making the upper left most three and right most three particles unmovable
	//pinning those particles
	for (int i = 0; i < 3; i++)
	{
		getParticle(0 + i, 0)->offsetPos(Vec3(0.5, 0.0, 0.0));
		getParticle(0 + i, 0)->makeUnmovable();

		getParticle(0 + i, 0)->offsetPos(Vec3(-0.5, 0.0, 0.0));
		getParticle(num_particles_width - 1 - i, 0)->makeUnmovable();
	}
}

//renders the cloth
void Cloth::renderCloth()
{
	// reset the normals, through the iterator
	std::vector<Particle>::iterator particle;
	for (particle = particles.begin(); particle != particles.end(); particle++)
	{
		(*particle).resetNormal();
	}

	//renders the cloth
	glBegin(GL_TRIANGLES);
	//for width
	for (int x = 0; x < num_particles_width - 1; x++)
	{
		//for height
		for (int y = 0; y < num_particles_height - 1; y++)
		{
			//set the color
			Vec3 color(1, 0, 0);
			//draw triangles based on particles
			drawTriangle(getParticle(x + 1, y), getParticle(x, y), getParticle(x, y + 1), color);
			drawTriangle(getParticle(x + 1, y + 1), getParticle(x + 1, y), getParticle(x, y + 1), color);
		}
	}
	//finish drawing triangles
	glEnd();
}

void Cloth::timeStep()
{
	//iterate over the constraints to make sure that they are satisfied
	std::vector<Constraint>::iterator constraint;
	for (int i = 0; i < CONSTRAINT_ITERATIONS; i++)
	{
		for (constraint = constraints.begin(); constraint != constraints.end(); constraint++)
		{
			// satisfy constraint
			(*constraint).satisfyConstraint();
		}
	}

	//iterate all the particles and call their timestep function
	std::vector<Particle>::iterator particle;
	for (particle = particles.begin(); particle != particles.end(); particle++)
	{
		// calculate the position of each particle at the next time step.
		(*particle).timeStep();
	}
}

//add force to the cloth/particles
void Cloth::addForce(const Vec3 direction)
{
	//iterate over the particles and add force to each particle
	std::vector<Particle>::iterator particle;
	for (particle = particles.begin(); particle != particles.end(); particle++)
	{
		(*particle).addForce(direction);
	}

}

//adds wind in a set direction
void Cloth::windForce(const Vec3 direction)
{
	//for each width
	for (int x = 0; x < num_particles_width - 1; x++)
	{
		//for each height
		for (int y = 0; y < num_particles_height - 1; y++)
		{
			//add wind to each particle triangle
			addWind(getParticle(x + 1, y), getParticle(x, y), getParticle(x, y + 1), direction);
			addWind(getParticle(x + 1, y + 1), getParticle(x + 1, y), getParticle(x, y + 1), direction);
		}
	}
}

//checks collision
void Cloth::ballCollision(const Vec3 center, const float radius)
{
	//iterate through the particles
	std::vector<Particle>::iterator particle;
	for (particle = particles.begin(); particle != particles.end(); particle++)
	{
		//v = particle position - the center of the object
		Vec3 v = (*particle).getPos() - center;
		//get the length of the vector
		float l = v.length();
		// if the particle is inside the ball
		if (v.length() < radius)
		{
			//make it so the particle is ontop of the object
			(*particle).offsetPos(v.normalized()*(radius - l));
		}
	}
}

//makes the cloth drop from the pins
void Cloth::dropCloth() {
	//iterate through the particles
	std::vector<Particle>::iterator particle;
	for (particle = particles.begin(); particle != particles.end(); particle++)
	{
		//make every particle movable
		particle->makeMovable();
	}
}