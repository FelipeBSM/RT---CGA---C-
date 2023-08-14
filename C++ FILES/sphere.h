#pragma once
#define SPHEREH

#include "hitable.h"
#include <math.h>
#include <ctime>

class sphere:public hitable {
public:
	sphere(){}
	sphere(vec3 cen, float r, material* MAT) : center(cen), radius(r), mat(MAT) {};

	virtual bool hit(const ray& r, float tmin, float tmax, hit_record &rec)const;
	vec3 center;
	float radius;
	material*mat;
};
float randFl() {
	static std::random_device rd;
	static std::mt19937  gen(rd());
	static std::uniform_real_distribution<float> d{ 0.0f,1.0 };

	return d(gen);
}
int randi() {
	srand(time(0));
	int randV = (rand() % 255) + 1;
	return randV;

}
bool sphere::hit(const ray& r, float tmin, float tmax, hit_record &rec)const {
	vec3 oc = r.origin() - center;
	float a= dot(r.direction(),r.direction());
	float b=dot(oc,r.direction());
	float c=dot(oc,oc)-radius*radius;
	float discriminant = b * b - a * c;
	if (discriminant > 0) {
		float temp = (-b - sqrt(b*b - a * c)) / a;
		if (temp < tmax && temp > tmin) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = ((rec.p - center)) / radius;
			rec.mat_ptr = mat;
			return true;
		}
		temp = (-b + sqrt(b*b - a * c)) / a;
		if (temp < tmax && temp > tmin) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = ((rec.p - center) / radius);
			rec.mat_ptr = mat;
			return true;
		}
	}
	return false;
}