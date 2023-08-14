#pragma once
#pragma once
#define TRIH

#include "hitable.h"
#include <math.h>

class Triangle :public hitable {
public:
	Triangle() {}
	Triangle(float X0, float X1, float Y0, float Y1, float K, material* MAT) : x0(X0),x1(X1),y0(Y0),y1(Y1),k(K), mat(MAT) {};

	float x0, x1, y0, y1, k;
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record &rec)const;
	vec3 center;
	float size;
	material*mat;
};
bool Triangle::hit(const ray& r, float tmin, float tmax, hit_record &rec)const {
	float t = (k - r.origin().z()) / r.direction().z();
	if (t < tmin || t >tmax) {
		return false;
	}
	float x = r.origin().x() + t * r.direction().x();
	float y = r.origin().y() + t * r.direction().y();
	if (x < x0 || x >x1 || y < y0 || y >y1) {
		return false;
	}
	
	rec.t = t;
	rec.mat_ptr = mat;
	rec.p = r.point_at_parameter(t);
	rec.normal = vec3(0, 0, 1);
	return true;

	
}
class TriangleY : public hitable {
public:
	TriangleY(){}
	TriangleY(float Y0, float Y1, float Z0, float Z1, float K, material* MAT) : y0(Y0), y1(Y1), z0(Z0), z1(Z1), k(K), mat(MAT) {};

	float y0, y1, z0, z1, k;
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record &rec)const;
	vec3 center;
	float size;
	material*mat;
};
bool TriangleY::hit(const ray& r, float tmin, float tmax, hit_record &rec)const {
	float t = (k - r.origin().x()) / r.direction().x();
	if (t < tmin || t >tmax) {
		return false;
	}
	float y = r.origin().y() + t * r.direction().y();
	float z = r.origin().z() + t * r.direction().z();
	if (y < y0 || y >y1 || z < z0 || z >z1) {
		return false;
	}

	rec.t = t;
	rec.mat_ptr = mat;
	rec.p = r.point_at_parameter(t);
	rec.normal = vec3(1, 0, 0);
	return true;


}