#pragma once

#include "Vector.h"

class Camera
{
public:
	Camera();
	~Camera();

	Vec3 pos, view, up;
	float yaw = -90.f, pitch = 0.f;
	float FoVy = 60.f, aspect = 4.f / 3.f;
	float near = -0.1f, far = -1000.f;

private:

};