#pragma once

#include<algorithm>

#include"Ray.h"


class Bounds3 {
public:
	typedef Meveral::Vector3f Vec3f;
public:
	Vec3f minPoint;
	Vec3f maxPoint;
public:
	Bounds3() :
		minPoint(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()),
		maxPoint(-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity()) {}
	Bounds3(Vec3f minP, Vec3f maxP) :minPoint(minP), maxPoint(maxP) {}
public:
	static Vec3f getMin(const Vec3f&v1, const Vec3f&v2);
	static Vec3f getMax(const Vec3f&v1, const Vec3f&v2);
	static bool inside(const Meveral::Vector3f&point, const Bounds3&bound);
	static bool isIntersect(const Ray&ray, const Bounds3&bound);
public:
	Vec3f getDiagonal() const { return maxPoint - minPoint; }
	Vec3f getCentroid() const { return maxPoint * 0.5f + minPoint * 0.5f; }
	int maxExtent();

};
inline Bounds3::Vec3f Bounds3::getMin(const Vec3f & v1, const Vec3f & v2)
{
	return Vec3f(std::min(v1.x(), v2.x()), std::min(v1.y(), v2.y()), std::min(v1.z(), v2.z()));
}

inline Bounds3::Vec3f Bounds3::getMax(const Vec3f & v1, const Vec3f & v2)
{
	return Vec3f(std::max(v1.x(), v2.x()), std::max(v1.y(), v2.y()), std::max(v1.z(), v2.z()));
}

inline bool Bounds3::inside(const Meveral::Vector3f & point, const Bounds3 & bound)
{
	return	point.x() <= bound.maxPoint.x() && point.x() >= bound.minPoint.x() &&
			point.y() <= bound.maxPoint.y() && point.y() >= bound.minPoint.y() &&
			point.z() <= bound.maxPoint.z() && point.z() >= bound.minPoint.z();
}

inline bool Bounds3::isIntersect(const Ray & ray, const Bounds3 & bound)
{
	Meveral::Vector3f invDir = ray.direction.inverse();
	Meveral::Vector3f t1, t2;
	t1 = (bound.minPoint - ray.point).cwiseProduct(invDir);
	t2 = (bound.maxPoint - ray.point).cwiseProduct(invDir);
	float tIn  = std::max({ std::min(t1.x(), t2.x()), std::min(t1.y(), t2.y()), std::min(t1.z(), t2.z()) });
	float tOut = std::min({ std::max(t1.x(), t2.x()), std::max(t1.y(), t2.y()), std::max(t1.z(), t2.z()) });

	return tOut - tIn > std::numeric_limits<float>::min() && tIn > std::numeric_limits<float>::min();
}


Bounds3 Union(const Bounds3&, const Bounds3&);

Bounds3 Union(const Bounds3&, const Meveral::Vector3f&);



