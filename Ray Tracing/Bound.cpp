#include "Bound.h"

Bounds3 Union(const Bounds3 & b1, const Bounds3 & b2)
{
	Bounds3 ret;
	ret.minPoint = Bounds3::getMin(b1.minPoint, b2.minPoint);
	ret.maxPoint = Bounds3::getMax(b1.maxPoint, b2.maxPoint);
	return ret;
}

Bounds3 Union(const Bounds3 & b, const Meveral::Vector3f & v)
{
	Bounds3 ret;
	ret.minPoint = Bounds3::getMin(b.minPoint, v);
	ret.maxPoint = Bounds3::getMax(b.maxPoint, v);
	return ret;
}


int Bounds3::maxExtent()
{
	auto d = this->getDiagonal();
	if (d.x() > d.y() && d.x() > d.z())
		return 0;
	else if (d.y() > d.z())
		return 1;
	else
		return 2;
}
