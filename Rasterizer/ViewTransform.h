#pragma once
#include"My_Vector.h"


Meveral::Matrix4f getModelMatrix(float rotationAngle, const Meveral::Vector3f&scale = {2.5, 2.5, 2.5}, const Meveral::Vector3f&displace = {0, 0, 0});
Meveral::Matrix4f getViewMatrix(const Meveral::Vector3f&eyePos);//do more: add direction
Meveral::Matrix4f getProjectionMatrix(float eyeFov, float aspectRatio, float zNear, float zFar);//zFar<zNear<0
