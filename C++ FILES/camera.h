#pragma once
#define CAMERAH
#define _USE_MATH_DEFINES
#include "ray.h"
#include <math.h>

class camera {
public:
	camera(vec3 lookfrom, vec3 lookAt, vec3 vUp,float fov, float ratio) 
	{
	
		vec3 u, v, w;
		float theta = fov * M_PI / 180;
		float halftH = tan(theta / 2);
		float width = ratio * halftH;
		origin = lookfrom;
		w = unit_vector(lookfrom - lookAt);
		u = unit_vector(cross(vUp, w));
		v = cross(w, u);

		lower_left_corner = vec3(-width,-halftH,-1.0);
		lower_left_corner = origin - width * u - halftH * v - w;

		horizontal = 2*width*u;
		vertical = 2*halftH*v;
		
	}
	ray get_ray(float u, float v) { return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin); }

	vec3 origin;
	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;

};