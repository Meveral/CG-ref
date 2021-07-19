#include"Render.h"
#include"global.h"
#include<opencv2/opencv.hpp>

#include<iostream>

static int fileNameCount{ 0 };

Meveral::Vector3f Render::castRay(const Meveral::Vector3f &eyePos, const Meveral::Vector3f &eyeDir, const Scene &scene, int depth)
{
	const float EPSILON = 0.00001;
	Meveral::Vector3f hitColor = scene.backgroundColor;
	if (depth > maxDepth)
		return hitColor;

	Ray eyeRay(eyePos, eyeDir);
	auto* node = scene.bvh->root;
	Intersection hit = BVHAccel::getIntersection(node, eyeRay);
	if (hit.happened)
	{
		//		std::cout << "hit happend" << std::endl;
				//shader
		auto N = hit.normal.normalized();
		auto Kd = hit.material->kd;
		auto Ks = hit.material->ks;
		auto hitPoint = hit.coords;
		hitPoint = eyeRay.direction.dot(N) > 0 ? hitPoint - N * EPSILON : hitPoint + N * EPSILON;

		Meveral::Vector3f lightAmt;
		Meveral::Vector3f colorSpecular;
		for (auto&light : scene.lights)
		{
			Meveral::Vector3f lightDir = light->point - hitPoint;
			float lightDistance2 = lightDir.dot(lightDir);
			lightDir.normalize();
			auto reflectDir = Ray::reflect(-lightDir, N);

			float LdotN = std::max(0.f, lightDir.dot(N));
			Ray shadowRay(hitPoint, lightDir);
			auto shadowHit = BVHAccel::getIntersection(scene.bvh->root, shadowRay);
			bool inShadow = shadowHit.happened && shadowHit.distance*shadowHit.distance < lightDistance2;

			lightAmt += inShadow ? Meveral::Vector3f() : light->intensity * LdotN;

			colorSpecular += light->intensity * std::pow(std::max(0.f, -reflectDir.dot(eyeDir)), hit.material->specularExponent);
			//			if(colorSpecular.x()>0.f||colorSpecular.y()>0.f||colorSpecular.z()>0.f)
			//				std::cout << "Specular color: " << colorSpecular << std::endl;
			//			if (!inShadow)
			//				std::cout << "Ambient color: " << lightAmt << std::endl;
		}
		hitColor = lightAmt.cwiseProduct(hit.material->color)*Kd + colorSpecular * Ks;
	}
	return hitColor * 255.f;
}

void Render::rendering(const Scene &scene, const Meveral::Vector3f & eyePos)
{
	std::vector<Meveral::Vector3f>frameBuffer(scene.width*scene.height);
	float scale = tan(getRadian(scene.fov*0.5f));
	float aspectRatio = scene.width / (float)scene.height;

	int count{ 0 };
	for (int i = 0; i < scene.width; ++i)
	{
		for (int j = 0; j < scene.height; ++j)
		{
			float x = (2 * (i + 0.5) / (float)scene.width - 1) * aspectRatio * scale;
			float y = (2 * (j + 0.5) / (float)scene.height - 1) * scale;
			auto tmpX{ x }, tmpY(y);
			x = tmpY;
			y = -tmpX;

			//			float x = (1 - 2 * (i + 0.5) / (float)scene.width) * aspectRatio * scale;
//			float y = (1 - 2 * (j + 0.5) / (float)scene.height) * scale;

			Meveral::Vector3f dir(x, y, -1);
			dir.normalize();
			frameBuffer[count++] = castRay(eyePos, dir, scene, 0);
			//			frameBuffer[i*scene.height + j] = castRay(eyePos, dir, scene, 0);
		}
	}

	cv::Mat image(scene.width, scene.height, CV_32FC3, frameBuffer.data());
	image.convertTo(image, CV_8UC3);
	cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
	cv::imshow("test", image);
	cv::waitKey(10);
	char fileName[100];
	sprintf_s(fileName, "./output/output%d.jpg", fileNameCount++);
	cv::imwrite(fileName, image);

	return;
}
