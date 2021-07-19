#include "ViewTransform.h"
#include"global.h"

Meveral::Matrix4f getModelMatrix(float rotationAngle, const Meveral::Vector3f&scale, const Meveral::Vector3f&displace)
{
	Meveral::Matrix4f modelM = Meveral::Matrix4f::Identity();
	Meveral::Matrix4f rotationM;
	Meveral::Matrix4f scaleM;
	Meveral::Matrix4f translateM;

	float rad = getRadian(rotationAngle);
	float c = cos(rad);
	float s = sin(rad);
	rotationM <<
		c, 0, s, 0,
		0, 1, 0, 0,
		-s, 0, c, 0,
		0, 0, 0, 1;
	scaleM <<
		scale[0], 0, 0, 0,
		0, scale[1], 0, 0,
		0, 0, scale[2], 0,
		0, 0, 0, 1;
	translateM <<
		1, 0, 0, displace[0],
		0, 1, 0, displace[1],
		0, 0, 1, displace[2],
		0, 0, 0, 1;

	modelM = translateM * scaleM * rotationM * modelM;
	return modelM;
}

Meveral::Matrix4f getViewMatrix(const Meveral::Vector3f & eyePos)
{
	Meveral::Matrix4f viewM = Meveral::Matrix4f::Identity();
	Meveral::Matrix4f translateM;


	translateM <<
		1, 0, 0, -eyePos[0],
		0, 1, 0, -eyePos[1],
		0, 0, 1, -eyePos[2],
		0, 0, 0, 1;

	return translateM * viewM;
}

Meveral::Matrix4f getProjectionMatrix(float eyeFov, float aspectRatio, float zNear, float zFar)
{
	Meveral::Matrix4f projectionM = Meveral::Matrix4f::Identity();
	Meveral::Matrix4f perspective2orthogonalM;
	Meveral::Matrix4f orthogonalM;

	float t = -abs(zNear) * tan(getRadian(eyeFov*0.5f));
	float r = t * aspectRatio;

	perspective2orthogonalM <<
		zNear, 0, 0, 0,
		0, zNear, 0, 0,
		0, 0, -(zNear + zFar), -zNear * zFar,
		0, 0, 1, 0;
	orthogonalM <<
		1.f / r, 0, 0, 0,
		0, 1.f / t, 0, 0,
		0, 0, 2.f / -(zNear - zFar), (zNear + zFar) / (zNear - zFar),
		0, 0, 0, 1;

	//	perspective2orthogonalM.print();
	//	std::cout << endl;
	//	orthogonalM.print();
	//	std::cout << endl;
	//	auto test1 =  perspective2orthogonalM*projectionM;//myf
	//	auto test2 = orthogonalM * test1;

	projectionM = orthogonalM * perspective2orthogonalM * projectionM;
	return projectionM;
}


