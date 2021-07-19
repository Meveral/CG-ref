#include "Scene.h"

void Scene::buildBVH()
{
	this->bvh = new BVHAccel(objects,1,BVHAccel::SplitMethod::NAIVE);
}
