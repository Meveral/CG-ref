#include "BVH.h"

BVHAccel::BVHAccel(std::vector<Object*> objects, int m, SplitMethod s)
	:primitives(std::move(objects)), maxPrimsNum(std::min(m, 255)), splitMethod(s)
{
	if (primitives.empty())
		return;
	root = buildBVH(primitives.begin(), primitives.end());
}

BVHNode* BVHAccel::buildBVH(std::vector<Object*>::iterator begin, std::vector<Object*>::iterator end)
{
	BVHNode* node = new BVHNode();
	int sz = end - begin;
	assert(sz > 0 && "out of range");
	if (sz == 1)
	{
		node->bound = (*begin)->getBound();
		node->object = (*begin);
		//		node->left = nullptr;
		//		node->right= nullptr;
		return node;
	}
	else if (sz == 2)
	{
		node->left = buildBVH(begin, begin + 1);
		node->right = buildBVH(begin + 1, end);
		node->bound = Union(node->left->bound, node->right->bound);
		return node;
	}
	else
	{
		Bounds3 centroidBounds;
		auto it = Union(centroidBounds, (*begin)->getBound());
		for (auto it = begin; it < end; ++it)
			centroidBounds = Union(centroidBounds, (*it)->getBound());
		int dim = centroidBounds.maxExtent();
		if (dim == 0)//maxExtent is x;
			std::sort(begin, end, [](auto&lhs, auto&rhs) {return lhs->getBound().getCentroid().x() < rhs->getBound().getCentroid().x(); });
		else if (dim == 1)//maxExtent is y;
			std::sort(begin, end, [](auto&lhs, auto&rhs) {return lhs->getBound().getCentroid().y() < rhs->getBound().getCentroid().y(); });
		else//maxExtent is z;
			std::sort(begin, end, [](auto&lhs, auto&rhs) {return lhs->getBound().getCentroid().z() < rhs->getBound().getCentroid().z(); });

		auto mid = begin + sz / 2;

		node->left = buildBVH(begin, mid);
		node->right = buildBVH(mid, end);
		node->bound = Union(node->left->bound, node->right->bound);
	}

	return node;
}

void BVHNode::print(std::ostream & os)
{
	os << bound.minPoint << std::endl;
	os << bound.maxPoint << std::endl << std::endl;
	if (this->left)this->left->print(os);
	if (this->right)this->right->print(os);
}

void BVHNode::free()
{
	if (left)left->free();
	if (right)right->free();
	delete this;
}

Intersection BVHAccel::getIntersection(BVHNode*root, const Ray&ray)
{
	Intersection ret;
	if (!root)
		return ret;
	if (Bounds3::inside(ray.point, root->bound) || Bounds3::isIntersect(ray, root->bound))
	{
		if (!root->left && !root->right)
			return root->object->getIntersection(ray);
		else
		{
			auto leftIntersection = getIntersection(root->left, ray);
			auto rightIntersection = getIntersection(root->right, ray);
			return leftIntersection.distance < rightIntersection.distance ? leftIntersection : rightIntersection;
		}
	}
	return ret;
}

void BVHAccel::freeBVHAccel(BVHAccel *bvh)
{
	if (!bvh)
		return;
	if (bvh->root)bvh->root->free();
	delete bvh;
}



