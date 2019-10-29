

#include "Cloth.h"

void Constraint::satisfyConstraint()
{
	// vector from particle1 to particle2
	Vec3 p1ToP2 = p2->getPos() - p1->getPos();

	// current distance between p1 and p2
	float current_distance = p1ToP2.length();

	// The offset vector that could moves p1 into a distance of rest_distance to p2
	Vec3 correctionVector = p1ToP2 * (1 - rest_distance / current_distance);

	// Lets make it half that length, so that we can move BOTH p1 and p2.
	Vec3 correctionVectorHalf = correctionVector * 0.5;

	// correctionVectorHalf is pointing from p1 to p2, so the length should move p1 half the length needed to satisfy the constraint.
	p1->offsetPos(correctionVectorHalf);

	// we must move p2 the negative direction of correctionVectorHalf since it points from p2 to p1, and not p1 to p2.	
	p2->offsetPos(-correctionVectorHalf); 
}