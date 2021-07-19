#pragma once
#include"My_Vector.h"


class Light {
public:
	Light() {};
	Light(const Meveral::Vector3f&p,const Meveral::Vector3f&i ):point(p),intensity(i) {};

public:
	Meveral::Vector3f point;
	Meveral::Vector3f intensity;
};




