#pragma once
#include<vector>
#include<algorithm>

#include"My_Vector.h"
#include"Object.h"



class BVHNode;

class BVHAccel {
public:
	enum SplitMethod { NAIVE, SAH };
	const int maxPrimsNum;
	const SplitMethod splitMethod;
	std::vector<Object*> primitives;
public:
	BVHAccel() = default;
	BVHAccel(std::vector<Object*>, int = 1, SplitMethod = NAIVE);

	~BVHAccel() {};
public:
	BVHNode* buildBVH(std::vector<Object*>::iterator, std::vector<Object*>::iterator);
public:
	static Intersection getIntersection(BVHNode*, const Ray&);
	static void freeBVHAccel(BVHAccel*);
public:
	BVHNode* root;
};

class BVHNode {
public:
	BVHNode() :left(nullptr), right(nullptr), object(nullptr), bound() {}
public:
	void print(std::ostream& os = std::cout);
	void free();
	
public:
	BVHNode*left;
	BVHNode*right;
	Object* object;
	Bounds3 bound;
};







