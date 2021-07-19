#pragma once
#include"Scene.h"

class Render {
public:
	Render() = default;

public:
	Meveral::Vector3f castRay(const Meveral::Vector3f&, const Meveral::Vector3f&, const Scene&, int depth);
	void rendering(const Scene&, const Meveral::Vector3f& eyePos = { -1,5,10 });
public:
	int maxDepth{ 5 };
};


