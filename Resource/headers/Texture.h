#pragma once
#include<opencv2/opencv.hpp>
#include<string>
#include"My_Vector.h"

class Texture {
public:
	Texture(const std::string&name)
	{
		imageData = cv::imread(name);
		cv::cvtColor(imageData, imageData, cv::COLOR_RGB2BGR);
		width = imageData.cols;
		height= imageData.rows;
	}
public:
	Meveral::Vector3f getColor(float u, float v)
	{
		auto u_img = u * width;
		auto v_img = (1.f-v) * height;
		auto color = imageData.at<cv::Vec3b>(v_img, u_img);
		return Meveral::Vector3f(color[0],color[1],color[2]);
	}
	


private:
	cv::Mat imageData;
	int width;
	int height;
};








