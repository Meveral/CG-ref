#include "Rasterizer.h"
#include<tuple>


#if 1
#include<fstream>
#endif 

extern std::mutex m;

void Rasterizer::clear(const Buffers&buff)
{
	if ((buff & Buffers::Color) == Buffers::Color)
	{
		std::fill(frameBuf.begin(), frameBuf.end(), Meveral::Vector3f());
	}
	if ((buff & Buffers::Depth) == Buffers::Depth)
	{
		std::fill(depthBuf.begin(), depthBuf.end(), std::numeric_limits<float>::infinity());
	}
}





std::tuple<float, float, float> computeBarycentric2D(float x, float y, const Meveral::Vector3f* v)
{
	float c1 = (x*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*y + v[1].x()*v[2].y() - v[2].x()*v[1].y()) / (v[0].x()*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*v[0].y() + v[1].x()*v[2].y() - v[2].x()*v[1].y());
	float c2 = (x*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*y + v[2].x()*v[0].y() - v[0].x()*v[2].y()) / (v[1].x()*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*v[1].y() + v[2].x()*v[0].y() - v[0].x()*v[2].y());
	float c3 = (x*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*y + v[0].x()*v[1].y() - v[1].x()*v[0].y()) / (v[2].x()*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*v[2].y() + v[0].x()*v[1].y() - v[1].x()*v[0].y());

	return { c1,c2,c3 };
}

inline Meveral::Vector2f interpolate(const Meveral::Vector2f* v, float a, float b, float g)
{
	return v[0] * a + v[1] * b + v[2] * g;
}

inline Meveral::Vector3f interpolate(const Meveral::Vector3f*v, float a, float b, float g)
{
	return v[0] * a + v[1] * b + v[2] * g;
}

void draw(Rasterizer&r,std::vector<Triangle*> triangleList)
{

	if (triangleList.empty())
		return;

	Meveral::Matrix4f MV = r.view * r.model;
	Meveral::Matrix4f MVP = r.projection * MV;
	Meveral::Matrix4f inverseTranspose = MV.invert().transpose();

	//	std::cout << "MV:\n"; 
	//	MV.print();
	//	std::cout << "MVP:\n"; 
	//	MVP.print();
	//	std::cout << "MV.invert.transpose:\n"; 
	//	inverseTranspose.print();

	//	std::fstream file("test.txt");//myf 28

	//	MVP.print();
//	auto sz = triangleList.size();

	for (auto&it : triangleList)
	{
		Triangle newTriangle = *it;
		auto homogeneous = [](auto&it) {for (int i = 0; i < 4; ++i)it[i] /= it[3]; };


		//get ViewspaceVertices
		std::array<Meveral::Vector4f, 3> viewspacePos{};
		for (int i = 0; i < 3; ++i)
		{
			Meveral::Vector4f tmp;
			tmp[0] = it->vertices[i].x();
			tmp[1] = it->vertices[i].y();
			tmp[2] = it->vertices[i].z();
			tmp[3] = 1;
			viewspacePos[i] = MV * tmp;
		}

		//setVertice
		for (int i = 0; i < 3; ++i)
		{//for every vetex
			Meveral::Vector4f tmp;
			tmp[0] = it->vertices[i].x();
			tmp[1] = it->vertices[i].y();
			tmp[2] = it->vertices[i].z();
			tmp[3] = 1;
			tmp = MVP * tmp;
			homogeneous(tmp);
			tmp.x() = 0.5*r._width*(tmp.x() + 1.f);
			tmp.y() = 0.5*r._width*(tmp.y() + 1.f);
			//			std::cout << "Vertex[" << i << "]:\t";
			//			tmp.print();
			//			std::cout << std::endl;

			newTriangle.setVertex(i, tmp.head<3>());
		}
		//			std::cout << std::endl;

				//setNormal
		for (int i = 0; i < 3; ++i)
		{
			Meveral::Vector4f tmp;
			tmp[0] = it->normals[i].x();
			tmp[1] = it->normals[i].y();
			tmp[2] = it->normals[i].z();
			tmp[3] = 0.f;
			newTriangle.setNormal(i, (inverseTranspose*tmp).head<3>());
		}

		//		newTriangle.normals[0].print(file);
		//		file << std::endl;
		//		newTriangle.normals[1].print(file);
		//		file << std::endl;
		//		newTriangle.normals[2].print(file);
		//		file << std::endl << std::endl;;


		//setColor
		newTriangle.setColor(0, Meveral::Vector3f(148, 121, 92));
		newTriangle.setColor(1, Meveral::Vector3f(148, 121, 92));
		newTriangle.setColor(2, Meveral::Vector3f(148, 121, 92));

//		newTriangle.setColor(0, Meveral::Vector3f(200, 0, 0));
//		newTriangle.setColor(1, Meveral::Vector3f(200, 0, 0));
//		newTriangle.setColor(2, Meveral::Vector3f(200, 0, 0));

		rasterizeTriangle(r,newTriangle, viewspacePos);
	}
	//	file.close();
}

void rasterizeTriangle(Rasterizer&r, const Triangle &t, std::array<Meveral::Vector4f, 3>&viewspacePos)
{

	const auto& v = t.vertices;
	int x_left = std::min({ v[0].x(),v[1].x(),v[2].x() });
	int x_right = std::max({ v[0].x(),v[1].x(),v[2].x() });
	int y_down = std::min({ v[0].y(),v[1].y(),v[2].y() });
	int y_top = std::max({ v[0].y(),v[1].y(),v[2].y() });

//	std::fstream file("test.txt", std::ios::app);

	for (int x = x_left; x <= x_right; ++x)
	{
		for (int y = y_down; y <= y_top; ++y)
		{
			if (insideTriangle(x, y, v))
			{
				auto[alpha, beta, gamma] = computeBarycentric2D(x, y, v);

//				file << alpha << " " << beta << " " << gamma << std::endl;

				float Z = 1.f / (alpha + beta + gamma);

				float zInterpolated = alpha * v[0].z() + beta * v[1].z() + gamma * v[2].z();
				zInterpolated *= Z;

				m.lock();
				if (std::abs(zInterpolated) < std::abs(r.depthBuf[y*r.width() + x]))
				{
					r.setDepth(x,y, zInterpolated);

					Meveral::Vector3f normalInterpolated = interpolate(t.normals, alpha, beta, gamma);
					normalInterpolated = normalInterpolated * Z;

					Meveral::Vector3f colorInterpolated = interpolate(t.colors, alpha, beta, gamma);
					colorInterpolated = colorInterpolated * Z;

					Meveral::Vector2f texCoordsInterpolated = interpolate(t.texCoords, alpha, beta, gamma);
					texCoordsInterpolated = texCoordsInterpolated * Z;

					Meveral::Vector3f viewspacePosInterpolated;
					viewspacePosInterpolated.x() = viewspacePos[0].x() * alpha + viewspacePos[1].x() * beta + viewspacePos[2].x()* gamma;
					viewspacePosInterpolated.y() = viewspacePos[0].y() * alpha + viewspacePos[1].y() * beta + viewspacePos[2].y()* gamma;
					viewspacePosInterpolated.z() = viewspacePos[0].z() * alpha + viewspacePos[1].z() * beta + viewspacePos[2].z()* gamma;


					fragmentShaderPayload payload(viewspacePosInterpolated, colorInterpolated, normalInterpolated, texCoordsInterpolated, r.texture ? &*r.texture : nullptr);
					Meveral::Vector3f color = r.fragmentShader(payload);
					r.setColor(x, r._height - y, color);
				}
				m.unlock();
			}
		}
	}


//	file.close();
}

