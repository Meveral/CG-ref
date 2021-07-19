#include<iostream>
#include<memory>
#include<string>
#include<iostream>
#include<iterator>

#include"Scene.h"
#include"global.h"
#include"Render.h"
#include"Material.h"
#include"Triangle.h"
#include"OBJ_Loader.h"
#include"Viewtransform.h"

#include<fstream>

using std::cin;
using std::cout;
using std::endl;



void printTree(BVHNode*root, std::iostream&os)
{
	if (!root)
		return;

	root->bound.minPoint.print(os);
	os << endl;
	root->bound.maxPoint.print(os);
	os << endl << endl;

	printTree(root->left, os);
	printTree(root->right, os);
}


void initialMeshTriangle(MeshTriangle&meshTriangle, const std::string&object);

void rebuildMeshTriangle(MeshTriangle&mesh, const Meveral::Matrix4f&Model);


int main(int argc, char* argv[])
{
	Scene scene(1280, 960);
	Render r;

	std::string objPath("../Resource/object/");
	std::string objName("bunny.obj");

	std::fstream file("test.txt", std::ios::out | std::ios::trunc);
	if (!file.is_open())
		std::cout << "can`t open this file." << std::endl;

	MeshTriangle bunny;
	initialMeshTriangle(bunny, objPath + objName);

	float rotationAngel = 5.f;
	Meveral::Matrix4f Model = getModelMatrix(rotationAngel, {1,1,1});

	Meveral::Matrix4f tmpModel = getModelMatrix(0.f, { 1,1,1 }, {2.f,0,-1.f});
	rebuildMeshTriangle(bunny, tmpModel);

	scene.add(std::make_unique<Light>(Meveral::Vector3f(-20, 70, 20), Meveral::Vector3f(1, 1, 1)));
	scene.add(std::make_unique<Light>(Meveral::Vector3f(20, 70, 20), Meveral::Vector3f(1, 1, 1)));
	Meveral::Vector3f eyePos{ -1,5,12 };
	while (1)
	{
		scene.add(bunny);
		scene.buildBVH();

		r.rendering(scene,eyePos);

		BVHAccel::freeBVHAccel(bunny.bvh);
		BVHAccel::freeBVHAccel(scene.bvh);
		scene.clearObjects();

		rebuildMeshTriangle(bunny, Model);
	}
	return 0;
}




void initialMeshTriangle(MeshTriangle&meshTriangle, const std::string&object)
{
	objl::Loader loader;
	loader.LoadFile(object);
	assert(loader.LoadedMeshes.size() == 1 && "Load mesh size != 1");
	auto mesh = loader.LoadedMeshes[0];

	Meveral::Vector3f minVertex(
		std::numeric_limits<float>::infinity(),
		std::numeric_limits<float>::infinity(),
		std::numeric_limits<float>::infinity());

	Meveral::Vector3f maxVertex(
		-std::numeric_limits<float>::infinity(),
		-std::numeric_limits<float>::infinity(),
		-std::numeric_limits<float>::infinity());

	for (int i = 0, sz = mesh.Vertices.size(); i < sz; i += 3)
	{
		std::array<Meveral::Vector3f, 3>vertices{};
		for (int j = 0; j < 3; ++j)
		{
			Meveral::Vector3f vert(mesh.Vertices[i + j].Position.X,
				mesh.Vertices[i + j].Position.Y,
				mesh.Vertices[i + j].Position.Z);
			vert *= 60.f;//too small.
			vertices[j] = vert;

			minVertex.x() = std::min(minVertex.x(), vert.x());
			minVertex.y() = std::min(minVertex.y(), vert.y());
			minVertex.z() = std::min(minVertex.z(), vert.z());

			maxVertex.x() = std::max(maxVertex.x(), vert.x());
			maxVertex.y() = std::max(maxVertex.y(), vert.y());
			maxVertex.z() = std::max(maxVertex.z(), vert.z());
		}
		Material*m = new Material(MaterialType::DIFFUSE_AND_GLOSSY, Meveral::Vector3f(0.5, 0.5, 0.5), Meveral::Vector3f());
		m->kd = 0.6;
		m->ks = 0.f;//todo
		m->specularExponent = 0;//todo

//		for (int i = 0; i < 3; ++i)//Model transformation
//		{
//			Meveral::Vector4f v;
//			v[0] = vertices[i][0];
//			v[1] = vertices[i][1];
//			v[2] = vertices[i][2];
//			v[3] = 1;
//			v = Model * v;
//			for (int j = 0; j < 4; ++j)
//				v[i] /= v[3];
//			vertices[i][0] = v[0];
//			vertices[i][1] = v[1];
//			vertices[i][2] = v[2];
//		}
//
		meshTriangle.triangles.emplace_back(vertices[0], vertices[1], vertices[2], m);
	}
	meshTriangle.boundingBox = Bounds3(minVertex, maxVertex);

	std::vector<Object*> pTrs;
	for (auto&it : meshTriangle.triangles)
		pTrs.push_back(&it);
	meshTriangle.bvh = new BVHAccel(pTrs);
}

void rebuildMeshTriangle(MeshTriangle & mesh, const Meveral::Matrix4f & Model)
{
	Meveral::Vector3f minVertex(
		std::numeric_limits<float>::infinity(),
		std::numeric_limits<float>::infinity(),
		std::numeric_limits<float>::infinity());

	Meveral::Vector3f maxVertex(
		-std::numeric_limits<float>::infinity(),
		-std::numeric_limits<float>::infinity(),
		-std::numeric_limits<float>::infinity());

	for (int i = 0, sz = mesh.triangles.size(); i < sz; ++i)
	{
		std::array<Meveral::Vector3f, 3> newVertices{};

		//		auto do_it = [&](auto&it) {
		//			Meveral::Vector4f t{ it[0],it[1],it[2],1 };
		//			t = Model * t;
		//			for (int i = 0; i < 3; ++i)
		//				t[i] /= t[3];
		//			return Meveral::Vector3f(t[0],t[1],t[2]);
		//		};
		//		std::transform(std::begin(mesh.triangles[i].vertices), std::end(mesh.triangles[i].vertices), newVertices,do_it);
		//
		for (int j = 0; j < 3; ++j)
		{
			Meveral::Vector4f t({ mesh.triangles[i].vertices[j][0],mesh.triangles[i].vertices[j][1],mesh.triangles[i].vertices[j][2],1.f });
			t = Model * t;
			t[0] /= t[3];
			t[1] /= t[3];
			t[2] /= t[3];
			newVertices[j] = { t[0],t[1],t[2] };
		}

		for (int j = 0; j < 3; ++j)
		{
			minVertex.x() = std::min(minVertex.x(), newVertices[j].x());
			minVertex.y() = std::min(minVertex.y(), newVertices[j].y());
			minVertex.z() = std::min(minVertex.z(), newVertices[j].z());

			maxVertex.x() = std::max(maxVertex.x(), newVertices[j].x());
			maxVertex.y() = std::max(maxVertex.y(), newVertices[j].y());
			maxVertex.z() = std::max(maxVertex.z(), newVertices[j].z());
		}
		mesh.triangles[i] = Triangle(newVertices[0], newVertices[1], newVertices[2], mesh.triangles[i].material);
	}
	mesh.boundingBox = Bounds3(minVertex, maxVertex);
	std::vector<Object*> ptrs{};
	for (auto&it : mesh.triangles)
		ptrs.push_back(&it);
	mesh.bvh = new BVHAccel(ptrs);
}



