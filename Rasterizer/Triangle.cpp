#include "Triangle.h"

static float EPSILON = 0.00001;
Triangle::Triangle() :material(nullptr), boundingBox(), N()
{
	for (int i = 0; i < 3; ++i)
	{
		colors[i] = Meveral::Vector3f();
		normals[i] = Meveral::Vector3f();
		vertices[i] = Meveral::Vector3f();
		texCoords[i] = Meveral::Vector2f();
	}
}

Triangle::Triangle(const Meveral::Vector3f & v0, const Meveral::Vector3f & v1, const Meveral::Vector3f & v2, Material* m) :vertices{ v0,v1,v2 }, material(m)
{
	auto a = Bounds3::getMin(v0, Bounds3::getMin(v1, v2));
	auto b = Bounds3::getMax(v0, Bounds3::getMax(v1, v2));
	boundingBox = Bounds3(a, b);
	e1 = v1 - v0;
	e2 = v2 - v0;
	N = (e1.cross(e2)).normalized();
}

bool Triangle::isIntersect(const Ray & ray)const
{
	return Bounds3::inside(ray.point, boundingBox) || Bounds3::isIntersect(ray, boundingBox);
}

Intersection Triangle::getIntersection(const Ray &ray)//not yet
{
	Intersection ret;

	if (ray.direction.dot(N) > 0)
		return ret;
	double u, v, t_tmp = 0;
	auto pvec = ray.direction.cross(e2);
	double det = e1.dot(pvec);
	if (fabs(det) < EPSILON)
		return ret;

	double det_inv = 1. / det;
	Meveral::Vector3f tvec = ray.point - vertices[0];
	u = tvec.dot(pvec)*det_inv;
	if (u < 0 || u > 1)
		return ret;
	Meveral::Vector3f qvec = tvec.cross(e1);
	v = ray.direction.dot(qvec)*det_inv;
	if (v < 0 || u + v > 1)
		return ret;
	t_tmp = e2.dot(qvec)*det_inv;
	if (t_tmp > 0.f)
	{
		ret.coords = ray.point + ray.direction * t_tmp;
		ret.distance = t_tmp;
		ret.happened = true;
		ret.material = this->material;
		ret.normal = this->N;
		ret.object = this;
	}
	return ret;
}


//Bounds3 Triangle::getBound()const
//{
//	auto a = Bounds3::getMin(vertices[0],Bounds3::getMin(vertices[1],vertices[2]));
//	auto b = Bounds3::getMax(vertices[0],Bounds3::getMax(vertices[1],vertices[2]));
//	return Bounds3(a,b);
//}
//
void Triangle::setColor(int i, const Meveral::Vector3f & c)
{
	if (c.x() < 0.f || c.x() > 255.f ||
		c.y() < 0.f || c.y() > 255.f ||
		c.z() < 0.f || c.z() > 255.f)
		assert("color out of range");

	this->colors[i] = c / 255.f;
}

bool insideTriangle(int x, int y, const Meveral::Vector3f* t)
{
	Meveral::Vector3f v[3];
	for (int i = 0; i < 3; ++i)
	{
		v[i].x() = t[i].x();
		v[i].y() = t[i].y();
		v[i].z() = 1.f;
	}
	Meveral::Vector3f f0, f1, f2;
	f0 = v[1].cross(v[0]);
	f1 = v[2].cross(v[1]);
	f2 = v[0].cross(v[2]);
	Meveral::Vector3f p(x, y, 1.f);

	return (p.dot(f0)*f0.dot(v[2]) > 0) && (p.dot(f1)*f1.dot(v[0]) > 0) && (p.dot(f2)*f2.dot(v[1]) > 0);
}

/*
MeshTriangle::MeshTriangle(const std::string &object)
{
	objl::Loader loader;
	loader.LoadFile(object);
	assert(loader.LoadedMeshes.size() == 1 && "Load mesh size != 1");
	auto mesh = loader.LoadedMeshes[0];

	Meveral::Vector3f minVertex(std::numeric_limits<float>::infinity(),
		std::numeric_limits<float>::infinity(),
		std::numeric_limits<float>::infinity());

	Meveral::Vector3f maxVertex(-std::numeric_limits<float>::infinity(),
		-std::numeric_limits<float>::infinity(),
		-std::numeric_limits<float>::infinity());

	for (int i = 0, sz = mesh.Vertices.size(); i < sz; i += 3)
	{
		std::array<Meveral::Vector3f, 3>vertices{};
		for (int j = 0; j < 3; ++j)
		{
			Meveral::Vector3f vert(mesh.Vertices[i].Position.X,
				mesh.Vertices[i].Position.Y,
				mesh.Vertices[i].Position.Z);
			vert *= 60.f;//too small.
			vertices[j] = vert;

			minVertex.x() = std::min(minVertex.x(), vert.x());
			minVertex.y() = std::min(minVertex.y(), vert.y());
			minVertex.z() = std::min(minVertex.z(), vert.z());

			maxVertex.x() = std::max(maxVertex.x(), vert.x());
			maxVertex.y() = std::max(maxVertex.y(), vert.y());
			maxVertex.z() = std::max(maxVertex.z(), vert.z());
		}
		triangles.emplace_back(vertices[0], vertices[1], vertices[2]);
		boundingBox = Bounds3(minVertex,maxVertex);

		Material*m = new Material();
		m->kd = 0.6;
		m->ks = 0.f;
		m->specularExponent = 0;
		material = m;

		std::vector<Object*> pTrs;
		for (auto&it : triangles)
			pTrs.push_back(&it);
		bvh = new BVHAccel(pTrs);
	}
}
//*/

bool MeshTriangle::isIntersect(const Ray & ray)const
{
	return Bounds3::inside(ray.point, boundingBox) || Bounds3::isIntersect(ray, boundingBox);
}

Intersection MeshTriangle::getIntersection(const Ray &ray)
{
	Intersection ret;
	if (bvh) ret = bvh->getIntersection(bvh->root, ray);
	return ret;
}


