#pragma once
#include"My_Vector.h"
#include"Light.h"
#include"Texture.h"

extern Light light1;
extern Light light2;
extern Meveral::Vector3f ambLightIntensity;
extern Meveral::Vector3f eyePos;

struct fragmentShaderPayload
{
	fragmentShaderPayload() { tex = nullptr; }
	fragmentShaderPayload(const Meveral::Vector3f&p, const Meveral::Vector3f&c, const Meveral::Vector3f&n, const Meveral::Vector2f&t,Texture*texp)
		:viewSpacePoint(p),color(c),normal(n.normalized()),texCoords(t),tex(texp) {}

	Meveral::Vector3f viewSpacePoint;
	Meveral::Vector3f color;
	Meveral::Vector3f normal;
	Meveral::Vector2f texCoords;
	Texture *tex;
};

class vertexShaderPayload
{

};
Meveral::Vector3f NormalFragmentShader(const fragmentShaderPayload&);

Meveral::Vector3f TextureFragmentShader(const fragmentShaderPayload&);

Meveral::Vector3f PhongFragmentShader(const fragmentShaderPayload&);

Meveral::Vector3f BulinPhongFragmentShader(const fragmentShaderPayload&);

Meveral::Vector3f DisplacementFragmentShader(const fragmentShaderPayload&);

Meveral::Vector3f BumpFragmentShader(const fragmentShaderPayload&);







