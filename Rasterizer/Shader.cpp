#include "Shader.h"
#include<algorithm>


Meveral::Vector3f NormalFragmentShader(const fragmentShaderPayload &payload)
{
	Meveral::Vector3f  color = payload.normal.normalized() + Meveral::Vector3f(1.f, 1.f, 1.f) * 0.5f;

	return color * 255.f;
}

Meveral::Vector3f TextureFragmentShader(const fragmentShaderPayload &payload)
{
	Meveral::Vector3f color;
	Meveral::Vector3f texColor;
	if (payload.tex)
		texColor = payload.tex->getColor(payload.texCoords.x(), payload.texCoords.y());

	Meveral::Vector3f ka = Meveral::Vector3f(0.005, 0.005, 0.005);
	Meveral::Vector3f kd = texColor/ 255.f;
	Meveral::Vector3f ks = Meveral::Vector3f(0.7937, 0.7937, 0.7937);


	std::vector<Light*>lights{ &light1,&light2 };

	float p = 150;

	color += ka.cwiseProduct(ambLightIntensity);
	Meveral::Vector3f viewDir((eyePos - payload.viewSpacePoint).normalized());
	for (auto&l : lights)
	{
		Meveral::Vector3f Intensity(l->intensity);
		Meveral::Vector3f lightDir(l->point - payload.viewSpacePoint);
		Meveral::Vector3f h((viewDir + lightDir.normalized()).normalize());

		Intensity = Intensity / lightDir.dot(lightDir);
		color += kd.cwiseProduct(Intensity)*std::max(0.f, lightDir.normalized().dot(payload.normal));
		color += ks.cwiseProduct(Intensity)*std::pow(std::max(0.f, h.dot(payload.normal)), p);
	}
	return color * 255.f;
}

Meveral::Vector3f PhongFragmentShader(const fragmentShaderPayload &)
{
	return Meveral::Vector3f();
}

Meveral::Vector3f BulinPhongFragmentShader(const fragmentShaderPayload &payload)
{
	Meveral::Vector3f color(0, 0, 0);

	Meveral::Vector3f ka(0.005, 0.005, 0.005);
	Meveral::Vector3f kd = payload.color;
	Meveral::Vector3f ks(0.7937, 0.7937, 0.7937);
	float p = 150;

	std::vector<Light*>lights{ &light1,&light2 };

	//color = Ka*Ia+Kd*I*max(0,cos(l,n))/r2+Ks*I*pow(max(0,cos(n,h)),p)/r2
	color += ka.cwiseProduct(ambLightIntensity);
	Meveral::Vector3f viewDir((eyePos - payload.viewSpacePoint).normalized());
	for (auto&l : lights)
	{
		Meveral::Vector3f Intensity(l->intensity);
		Meveral::Vector3f lightDir(l->point - payload.viewSpacePoint);
		Meveral::Vector3f h((viewDir + lightDir.normalized()).normalize());

		Intensity = Intensity / lightDir.dot(lightDir);
		color += kd.cwiseProduct(Intensity)*std::max(0.f, lightDir.normalized().dot(payload.normal));
		color += ks.cwiseProduct(Intensity)*std::pow(std::max(0.f, h.dot(payload.normal)), p);
	}

	return color * 255.f;
}

Meveral::Vector3f DisplacementFragmentShader(const fragmentShaderPayload &)
{
	return Meveral::Vector3f();
}

Meveral::Vector3f BumpFragmentShader(const fragmentShaderPayload &)
{
	return Meveral::Vector3f();
}
