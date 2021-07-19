#pragma once
#include"Bound.h"
#include"Material.h"

class Intersection;
class Object {
public:
	Object() = default;
	virtual ~Object() {}

public:
	virtual bool isIntersect(const Ray&ray)const = 0;
	virtual Intersection getIntersection(const Ray&ray)= 0;
	virtual Bounds3 getBound()const = 0;
};


class Intersection {
public:
	Intersection() :
		happened(false),
		object(nullptr),
		material(nullptr),
		distance(std::numeric_limits<double>::infinity()),
		coords(),
		normal() {};
public:
	bool happened;
	Object* object;
	Material* material;
	double distance;
	Meveral::Vector3f coords;
	Meveral::Vector3f normal;
};



