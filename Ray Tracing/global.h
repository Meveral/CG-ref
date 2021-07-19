#pragma once

#define PI (float)3.1415926


inline float getRadian(float angle) { return angle * PI / 180.f; };
inline float getAngle(float radian) { return radian * 180.f / PI; };



