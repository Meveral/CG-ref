#pragma once

#include<string>
#include<array>

#include"BVH.h"

class BVHAccel;

class Triangle :public Object {
public:
	Triangle();
	Triangle(const Meveral::Vector3f&p1, const Meveral::Vector3f&p2, const Meveral::Vector3f&p3,Material*m=nullptr);
	~Triangle() {};
public:
	bool isIntersect(const Ray&)const override;
	Intersection getIntersection(const Ray&)override;
	Bounds3 getBound()const override { return boundingBox; };

public:
	void setVertex(int i, const Meveral::Vector3f&v) { vertices[i] = v; };
	void setNormal(int i, const Meveral::Vector3f&n) { normals[i] = n; };
	void setColor(int i, const Meveral::Vector3f&c);
	void setTex(int i, const Meveral::Vector2f&uv) { texCoords[i] = uv; };

public:
	Meveral::Vector3f vertices[3];
	Meveral::Vector3f normals[3];
	Meveral::Vector3f colors[3];
	Meveral::Vector2f texCoords[3];

	Meveral::Vector3f N;//flat Normal
	Meveral::Vector3f e1, e2;//edges
	Material* material;
	Bounds3 boundingBox;
};

class MeshTriangle :public Object {
public:
	BVHAccel* bvh;
	Material* material;
	Bounds3 boundingBox;
	std::vector<Triangle>triangles;
public:
	MeshTriangle(): triangles(),material(nullptr),bvh(nullptr),boundingBox(){};
	MeshTriangle(const std::string&) {};
public:
	bool isIntersect(const Ray&ray)const override;
	Intersection getIntersection(const Ray&)override;
	Bounds3 getBound()const override { return boundingBox;}
	

public:

};

bool insideTriangle(int x, int y, const Meveral::Vector3f* t);
