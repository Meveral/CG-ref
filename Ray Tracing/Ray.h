#pragma once
#include"My_Vector.h"

class Ray {
public:
	Ray() = default;
	Ray(const Meveral::Vector3f&p,const Meveral::Vector3f&d):point(p),direction(d){}
public:
	static Meveral::Vector3f reflect(const Meveral::Vector3f&ray, const Meveral::Vector3f&normal);
public:
	Meveral::Vector3f point;
	Meveral::Vector3f direction;
};

inline Meveral::Vector3f Ray::reflect(const Meveral::Vector3f & I, const Meveral::Vector3f & N)
{
	return I - N * I.dot(N) * 2;
}



