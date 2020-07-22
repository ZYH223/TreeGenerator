#pragma once

#include <float.h>
#include "constant.h"
#include "vectors.h"

class Camera {
public:
	virtual float getTMin() const = 0;

	virtual void glInit(int w, int h) = 0;
	virtual void glPlaceCamera(void) = 0;
	virtual void dollyCamera(float dist) = 0;
	virtual void truckCamera(float dx, float dy) = 0;
	virtual void rotateCamera(float rx, float ry) = 0;
	virtual void scaleCamera(float ratio) = 0;
};

class OrthographicCamera : public Camera {
private:
	float tmin;
	Vec3f center;
	float size;
	Vec3f direction, horizontal, up, screenUp;
public:
	OrthographicCamera(Vec3f c, Vec3f d, Vec3f u, float s);
	float getTMin() const { return tmin; }
	void glInit(int w, int h);
	void glPlaceCamera(void);
	void dollyCamera(float dist);
	void truckCamera(float dx, float dy);
	void rotateCamera(float rx, float ry);
	void scaleCamera(float ratio);
};

class PerspectiveCamera : public Camera {
private:
	float tmin;
	Vec3f center;
	float fov, zNear, zFar;
	Vec3f direction, horizontal, up, screenUp;
public:
	PerspectiveCamera(Vec3f c, Vec3f d, Vec3f u, float a);
	float getTMin() const { return tmin; }
	void glInit(int w, int h);
	void glPlaceCamera(void);
	void dollyCamera(float dist);
	void truckCamera(float dx, float dy);
	void rotateCamera(float rx, float ry);
	void scaleCamera(float ratio);
};
