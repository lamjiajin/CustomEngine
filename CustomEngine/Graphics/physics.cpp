#include "main.h"


bool Physics::AABBvsAABB(const AABB & a, const AABB & b)
{
	// Exit with no intersection if found separated along an axis
	if (a.max.x < b.min.x or a.min.x > b.max.x) return false;
	if (a.max.y < b.min.y or a.min.y > b.max.y) return false;

	// No separating axis found, therefor there is at least one overlapping axis
	return true;
}

bool Physics::CirclevsCircle(Manifold * m)
{
	// Setup a couple pointers to each object
	PhysicsComponent *A = m->A;
	PhysicsComponent *B = m->B;

	// Vector from A to B
	Vec2 n = B->owner->position - A->owner->position;

	float r = A->circle_collision_data.radius + B->circle_collision_data.radius;

	if (n.Dot(n) > r*r)
		return false;

	// Circles have collided, now compute manifold
	float d = n.Length(); // perform actual sqrt

						  // If distance between circles is not zero
	if (d != 0)
	{
		// Distance is difference between radius and distance
		m->penetration = r - d;

		// Utilize our d since we performed sqrt on it already within Length( )
		// Points from A to B, and is a unit vector
		m->normal = n / d;
		return true;
	}

	// Circles are on same position
	else
	{
		// Choose random (but consistent) values
		m->penetration = A->circle_collision_data.radius;
		m->normal = vec2(1, 0);
		return true;
	}
}

#define clamp(v, _min, _max) (max(min(v, _max), _min))
bool Physics::AABBvsCircle(Manifold * m)
{
	// Setup a couple pointers to each object
	PhysicsComponent *A = m->A;
	PhysicsComponent *B = m->B;

	// Vector from A to B
	Vec2 n = B->owner->position - A->owner->position;

	// Closest point on A to center of B
	Vec2 closest = n;

	// Calculate half extents along each axis
	float x_extent = (A->aabb_collision_data.max.x - A->aabb_collision_data.min.x) / 2;
	float y_extent = (A->aabb_collision_data.max.y - A->aabb_collision_data.min.y) / 2;

	// Clamp point to edges of the AABB
	closest.x = clamp(closest.x, -x_extent, x_extent);
	closest.y = clamp(closest.y, -y_extent, y_extent);

	bool inside = false;

	// Circle is inside the AABB, so we need to clamp the circle's center
	// to the closest edge
	if (n == closest)
	{
		inside = true;

		// Find closest axis
		if (abs(n.x) > abs(n.y))
		{
			// Clamp to closest extent
			if (closest.x > 0)
				closest.x = x_extent;
			else
				closest.x = -x_extent;
		}

		// y axis is shorter
		else
		{
			// Clamp to closest extent
			if (closest.y > 0)
				closest.y = y_extent;
			else
				closest.y = -y_extent;
		}
	}

	Vec2 normal = n - closest;
	float d = normal.SqLength();// .LengthSquared();
	float r = B->circle_collision_data.radius;

	// Early out of the radius is shorter than distance to closest point and
	// Circle not inside the AABB
	if (d > r * r && !inside)
		return false;

	// Avoided sqrt until we needed
	d = sqrt(d);

	// Collision normal needs to be flipped to point outside if circle was
	// inside the AABB
	if (inside)
	{
		m->normal = -normal / d;
		m->penetration = r - d;
	}
	else
	{
		m->normal = normal / d;
		m->penetration = r - d;
	}

	return true;
}

