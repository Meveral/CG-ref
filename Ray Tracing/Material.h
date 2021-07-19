#pragma once
#include"My_Vector.h"

enum MaterialType { DIFFUSE_AND_GLOSSY, REFLECTION_AND_REFRACTION, REFLECTION };

class Material {
public:
	MaterialType type;
	Meveral::Vector3f color;
	Meveral::Vector3f emission;
	float ior;
	float kd, ks;
	float specularExponent;
public:
	Material(MaterialType t = DIFFUSE_AND_GLOSSY, const Meveral::Vector3f&c = { 0.5, 0.5, 0.5 }, const Meveral::Vector3f&e = { 0,0,0 })
		:type(t), color(c), emission(e), ior(), kd(), ks() { }
};

