#pragma once
#include<functional>
#include<optional>
#include<array>

#include"My_Vector.h"
#include"Object.h"
#include"Triangle.h"
#include"Texture.h"
#include"Shader.h"


enum class Buffers {
	Color = 1,
	Depth = 2
};
inline Buffers operator&(const Buffers&lhs, const Buffers&rhs) { return Buffers((int)lhs & (int)rhs); };
inline Buffers operator|(const Buffers&lhs, const Buffers&rhs) { return Buffers((int)lhs | (int)rhs); };

class Rasterizer {
public:
	friend void draw(Rasterizer&r, std::vector<Triangle*>triangleList);
	friend void rasterizeTriangle(Rasterizer&r, const Triangle&,std::array<Meveral::Vector4f,3>&);
public:
	Rasterizer() = delete;
	Rasterizer(int w, int h) :_width(w), _height(h) { frameBuf.resize(w*h); depthBuf.resize(w*h), texture = std::nullopt; };
	~Rasterizer() {};
public:
	void setModel(const Meveral::Matrix4f&m) { model = m; }
	void setView(const Meveral::Matrix4f&v) { view = v; }
	void setProjection(const Meveral::Matrix4f&p) { projection = p; }

	void setFragmentShader(std::function<Meveral::Vector3f(fragmentShaderPayload&)>shader) { fragmentShader = shader; };
	void setVertexShader(std::function<Meveral::Vector3f(vertexShaderPayload&)>shader) { vertexShader = shader; };

	void setTexture(const Texture&t) { texture = t; };
public:
	void clear(const Buffers&buff);
	std::vector<Meveral::Vector3f>& frameBuffer() { return frameBuf; };
	std::vector<float>& depthBuffer() { return depthBuf; };
	void setColor(int x, int y, const Meveral::Vector3f color) { frameBuf[x + y * _width] = color; }
	void setDepth (int x, int y, float dep) { depthBuf[x + y * _width] = dep; }

	int width() { return this->_width; };
	int height() { return this->_height; };
private:
private:
	Meveral::Matrix4f model;
	Meveral::Matrix4f view;
	Meveral::Matrix4f projection;

	std::function<Meveral::Vector3f(fragmentShaderPayload&)> fragmentShader;
	std::function<Meveral::Vector3f(vertexShaderPayload&)> vertexShader;

	std::optional<Texture> texture;

	std::vector<Meveral::Vector3f> frameBuf;
	std::vector<float> depthBuf;

	int _width;
	int _height;
};

std::tuple<float, float, float> computeBarycentric2D(float, float ,const Meveral::Vector3f*);

Meveral::Vector2f interpolate(const Meveral::Vector2f*,float,float,float);

Meveral::Vector3f interpolate(const Meveral::Vector3f*,float,float,float);


void draw(Rasterizer&r, std::vector<Triangle*>triangleList);

void rasterizeTriangle(Rasterizer&r, const Triangle&,std::array<Meveral::Vector4f,3>&);


