#include<functional>
#include<algorithm>
#include<numeric>

#include<opencv2/opencv.hpp>
#include<MeveralUse_HeadersUsual.h>

#include"global.h"
#include"My_Vector.h"
#include"Object.h"
#include"OBJ_Loader.h"
#include"Triangle.h"
#include"Rasterizer.h"
#include"Shader.h"
#include"Light.h"
#include"ViewTransform.h"
//#define myf_debug

using std::string;
using std::vector;
using std::cin;
using std::cout;
using std::endl;
using namespace Meveral;

Light light1(Meveral::Vector3f(20, 20, 20), Meveral::Vector3f(500, 500, 500));
Light light2(Meveral::Vector3f(-20, 20, 0), Meveral::Vector3f(500, 500, 500));
Meveral::Vector3f ambLightIntensity(10, 10, 10);
Meveral::Vector3f eyePos(0, 0, 10);


//useage: [a.exe] [outfileName] [ShaderName]
int main(int argc, char*argv[])
{
#ifdef myf_debug

	Vector3f a(1, 2, 3);
	auto it = a.dot(a);


	Matrix3f m;
	m <<
		1, 2, 3,
		4, 5, 3,
		7, 8, 1;
	cout << "\nM:" << endl;
	m.print();

	cout << "\ncofactor(0,0):" << endl;

	cout << "\nM transpose:" << endl;
	m.transpose().print();
	cout << "\nM invert:" << endl;
	m.invert().print();
	cout << "\nM adjoint:" << endl;
	m.adjugate().print();





	return 0;
#endif


#ifndef myf_debug

	float angle = 140.f;//rotation angle with -axis;
	float eyeFov{ 45.f };
	float aspectRatio{ 1.f };
	float zNear{ 0.1 };
	float zFar{ 50.f };

	bool commandLine = argc > 1;
	string texPath{ "../Resource/texture/" };
	string texName{ "spot_texture.png" };
	string outfileName{ "output.png" };
	string objPath{ "../Resource/object/" };
	string objName = "spot_triangulated_good.obj";
	//	string objName = "bunny.obj";

	objl::Loader loader;
	std::vector<Triangle*>TriangleList{};
	//	std::function<Vector3f(fragmentShaderPayload&)>activeShader = BulinPhongFragmentShader;
	std::function<Vector3f(fragmentShaderPayload&)>activeShader = TextureFragmentShader;

	//Load .obj 
	if (!loader.LoadFile(objPath + objName))
	{
		std::cout << "Can`t load .obj file \"" << objPath + objName << "\"!" << std::endl;
		return -1;
	}
	for (auto&mesh : loader.LoadedMeshes)
	{
		for (int i = 0, sz = mesh.Vertices.size(); i < sz; i += 3)
		{
			Triangle* t = new Triangle();
			for (int j = 0; j < 3; ++j)
			{
				t->setVertex(j, Vector3f(mesh.Vertices[i + j].Position.X, mesh.Vertices[i + j].Position.Y, mesh.Vertices[i + j].Position.Z));
				t->setNormal(j, Vector3f(mesh.Vertices[i + j].Normal.X, mesh.Vertices[i + j].Normal.Y, mesh.Vertices[i + j].Normal.Z));
				t->setTex(j, Vector2f(mesh.Vertices[i + j].TextureCoordinate.X, mesh.Vertices[i + j].TextureCoordinate.Y));
			}
			TriangleList.push_back(t);
		}
	}

	if (commandLine&&argc > 1)
		outfileName = argv[1];

	Rasterizer r(700, 700);
	r.setTexture(Texture(texPath + texName));

	if (commandLine&&argc > 2)
	{
		string shader = argv[2];
		std::transform(shader.begin(), shader.end(), shader.begin(), tolower);
		if (shader == "phong")
		{
			activeShader = PhongFragmentShader;
			std::cout << "Using Phong Shader" << std::endl;
		}
		else if (shader == "bulinphong")
		{
			activeShader = BulinPhongFragmentShader;
			std::cout << "Using Bulin-Phong Shader" << std::endl;
		}
		else if (shader == "normal")
		{
			activeShader = NormalFragmentShader;
			std::cout << "Using Normal Shader" << std::endl;
		}
		else if (shader == "bump")
		{
			activeShader = BumpFragmentShader;
			std::cout << "Using Bump Texture Shader" << std::endl;
		}
		else if (shader == "displacement")
		{
			activeShader = DisplacementFragmentShader;
			std::cout << "Using Displacement Texture Shader" << std::endl;
		}
		else if (shader == "texture")
		{
			texName = "spot_texture.png";
			r.setTexture(Texture(texPath + texName));
			activeShader = TextureFragmentShader;
			std::cout << "Using Texxture Shader" << std::endl;
		}
		else
			std::cout << "Using default Bulin-Phong Shader " << std::endl;
	}
	else { std::cout << "Using Bulin-Phong Shader" << std::endl; };
	r.setFragmentShader(activeShader);

	if (commandLine)
	{
		r.clear(Buffers::Color | Buffers::Depth);
		r.setModel(getModelMatrix(angle));
		r.setView(getViewMatrix(eyePos));
		r.setProjection(getProjectionMatrix(eyeFov, aspectRatio, zNear, zFar));


		r.draw(TriangleList);

		cv::Mat image(r.width(), r.height(), CV_32FC3, r.frameBuffer().data());
		image.convertTo(image, CV_8UC3);
		cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
		cv::imshow("test", image);
		cv::waitKey(0);
		cv::imwrite(outfileName, image);

		return 0;
	}

	int key{ 0 };
	while (key != 27)
	{
		r.clear(Buffers::Color | Buffers::Depth);
		r.setModel(getModelMatrix(angle));
		r.setView(getViewMatrix(eyePos));
		r.setProjection(getProjectionMatrix(eyeFov, aspectRatio, zNear, zFar));

		r.draw(TriangleList);

		cv::Mat image(r.width(), r.height(), CV_32FC3, r.frameBuffer().data());
		image.convertTo(image, CV_8UC3);
		cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
		cv::imshow("test", image);
		key = cv::waitKey(10);

		angle -= 1.f;
	}


#endif
	return 0;
}



