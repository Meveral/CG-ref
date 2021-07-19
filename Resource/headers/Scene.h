#pragma once
#include<vector>
#include<memory>
#include"Light.h"
#include"Object.h"
#include"BVH.h"

//extern Light;
//extern Object;

class Scene {

public:
	Scene() {};
	Scene(int x, int y) :width(x), height(y) {};
public:
//	void add(Object& pObj) { objects.push_back(std::unique_ptr<Object>(&pObj)); };
	void add(Object& obj) { objects.push_back(&obj); };
	void add(std::unique_ptr<Light>light) { lights.push_back(std::move(light)); };
	void clearObjects() { objects.clear(); }
	void buildBVH();

public:
//	std::vector<std::unique_ptr<Object>> objects{};
	std::vector<Object*> objects{};
	std::vector<std::unique_ptr<Light>> lights{};

private:
public:
	BVHAccel* bvh;
	int width{ 1280 };
	int height{960};
	float fov{90.f};
	Meveral::Vector3f backgroundColor{0.235294,0.67451,0.843137};
};









