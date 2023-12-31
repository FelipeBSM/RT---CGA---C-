// RayTracing.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include "pch.h"
#include <iostream>

#include <fstream>

#include "Triangle.h"
#include <random>
#include "sphere.h"
#include "hitable_list.h"
#include "material.h"
#include "camera.h"
#include "PostProcessing.h"
//random function to generate float values from 0 to 1
float randF() {
	static std::random_device rd;
	static std::mt19937  gen(rd());
	static std::uniform_real_distribution<float> d{ 0.0f,1.0 };

	return d(gen);
}
vec3 random_in_unit_sphere() {
	vec3 p;
	do {
		//random rays per sphere
		p = 2.0*vec3(randF(), randF(), randF()) - vec3(1, 1, 1);

	} while (p.squared_lenght() > 1.0);
	return p;
}
class material { //vritual class to implement each material separated
public:
	virtual bool isLight() const = 0;

	virtual color emitted() const {
		return color(0, 0, 0);
	}

	virtual color getAlbedo() const {
		return color(0, 0, 0);
	}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered)const = 0;
};
class lambert:public material {
public:
	lambert(const vec3& a): albedo(a){}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered)const {
		vec3 target= rec.p + rec.normal + random_in_unit_sphere();
		
		scattered = ray(rec.p, target - rec.p);
		attenuation = albedo;
		return true;		
	}
	virtual bool isLight() const {
		return false;
	}
	color albedo;
};
vec3 reflect(const vec3&v, const vec3& n) {
	return v - 2 * dot(v, n)*n; //reflection calculation -> same as the class formula
}
bool refract(const vec3&v, const vec3& n,float ni_over_nt, vec3& refracted) {
	vec3 uv = unit_vector(v);
	float dt = dot(uv, n);
	float discriminant = 1 - ni_over_nt * ni_over_nt*(1 - dt * dt);
	if (discriminant > 0) {
		refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
		return true;
	}
	else {
		return false;
	}

}
//vec3 refract(const vec3&v, const vec3& n, float c1, float c2) {
//	//n = normal
//	//v = vector
//	vec3 rout;
//	vec3 rout2;
//	float t = (c1*c1) - (c2*c2);
//	rout = t*((dot(v, n)*dot(v, n)-1) * 1 / c1 * (-n) + c2 / c1 * ((dot(v, n))*(n - v)));
//	
//	return rout;
//}
class metal :public material {
public:
	metal(const vec3& a, float f) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; }
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered)const {
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal*randF());
		scattered = ray(rec.p,  reflected +fuzz*random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.direction(),rec.normal)>0);
	}
	virtual bool isLight() const {
		return false;
	}

	color albedo;
	float fuzz; // mirroring material - multiplie per pixel -> spehre interferation -> ray gets out of the sphere with a disturb value -> not perfec 
					//reflection
};
class glass :public material {
public:
	glass(float ri) : ref_idx(ri) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered)const {
		vec3 outward_normal;
		vec3 reflected = reflect(r_in.direction(), rec.normal);
		float ni_over_nt;
		attenuation = vec3(1.0, 1.0, 1.0);
		vec3 refracted;
		if (dot(r_in.direction(),rec.normal)>0) 
		{
			outward_normal = -rec.normal;
			ni_over_nt = ref_idx;
		}
		else{
			outward_normal = rec.normal;
			ni_over_nt = 1.0/ref_idx;
		}
		if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
			scattered = ray(rec.p, refracted);
		}
		else {
			scattered = ray(rec.p, reflected);
			return false;
		}
		return true;
	}
	virtual bool isLight() const {
		return false;
	}
	float ref_idx;// mirroring material - multiplie per pixel -> spehre interferation -> ray gets out of the sphere with a disturb value -> not perfec 
					//reflection
};

class Light : public material {
public:
	Light(const color& l) : emitting(l) {}

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
	) const {
		return false;
	}

	virtual color emitted() const {
		return emitting;
	}
	virtual bool isLight() const {
		return true;
	}
	virtual color getAlbedo() const {
		return vec3(0,0,0);
	}
public:
	color emitting;
};

color colorBeforeLight(const ray& r, hitable *world, int depth) { //cor somente para fazer
	hit_record rec;
	if (world->hit(r, 0.001, FLT_MAX, rec)) {
		if (rec.mat_ptr->isLight()) {
			return rec.mat_ptr->getAlbedo();
		}
		else {
			ray scattered;
			color attenuation;
			if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
				return attenuation * colorBeforeLight(scattered, world, depth + 1);
			}
			else {
				return vec3(0, 0, 0);
			}
		}
		
	}
	else {
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}
color colorF(const ray& r, hitable *world, int depth) {
	hit_record rec;
	if (world->hit(r, 0.001, FLT_MAX, rec)) {
		if (rec.mat_ptr->isLight()) { // verifica se o material do obj é luz, se sim muda a forma de retorno
			ray scattered;
			color attenuation;
			color emitted = rec.mat_ptr->emitted();

			if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
				return emitted;



			return emitted + attenuation * colorF(scattered, world, depth - 1);
		}
		else { // se não retorno padrao do GA
			ray scattered;
			color attenuation;
			if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
				return attenuation * colorF(scattered, world, depth + 1);
			}
			else {
				return vec3(0, 0, 0);
			}
		}
	
		//past
		/*vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		return 0.5*color(ray(rec.p, target - rec.p), world);*/
		//return 0.5*vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
	}
	else {
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return (1.0 - t)*vec3(0.5, 0.2, 0.2) + t * vec3(0.2, 0.5, 0.2);
	}
	
}

int main()
{
	int width, height;
	PostProcessing post;
	std::string filename;

	std::cout << "Name your file: ";
	std::cin >> filename;
	std::cout << "---------------" << std::endl;
	std::cout << "Please give me the file width that you want: ";
	std::cin >> width;
	std::cout << "---------------" << std::endl;
	std::cout << "Okay, now tell me the height: ";
	std::cin >> height;

	std::ofstream img(filename + ".ppm"); //creating .ppm image
	std::ofstream imgNoLight(filename + "NoL" + ".ppm");
	std::cout << "File: " << filename << " was created!" << "File size: "<< width << " / " << height << std::endl;

	int nx = width;
	int ny = height;
	int ns = 100;
	float adjustment = 1;

	int ir, ig, ib, ibN,irN,igN;

	//ppm settings
	img << "P3" << std::endl;
	img << nx << " " << ny << std::endl;
	img << "255" << std::endl;

	imgNoLight << "P3" << std::endl;
	imgNoLight << nx << " " << ny << std::endl;
	imgNoLight << "255" << std::endl;


	int rendering;
	std::cout << "Choose your rendering engine: 1 - GRAU A; 2 - GRAU B." << std::endl;
	std::cin >> rendering;
	std::string filter;
	std::cout << "Ok, Grau B is Non-Photorealistic-Rendering, you Ray Tracer will look like a cartoon." << std::endl;
	std::cout << "Would you like to apply any filters? Yes or No." << std::endl;
	std::cin >> filter;

	if (rendering == 1) {
		post.ChooseEffect();
	}
	if (rendering == 2) {
		if (filter == "Yes") {
			
			post.ChooseEffect();
		}
		else {
			std::cout << "Ok! Writing Image..." << std::endl;
		}
	}
	

	//list of spheres -> hitable objects
	//PreSet01
 
#pragma region OBJCREATION

	hitable *list[5];
	list[0] = new sphere(vec3(0, 0, -1), 0.5,new lambert(color(0.9,0.2,0.2)));
	list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambert(color(0.5,0.5,0.5)));
	list[2] = new sphere(vec3(1, 0, -1), 0.5, new lambert(color(0.1, 0.4, 0.9)));
	list[3] = new sphere(vec3(-1,0, -1), 0.5, new lambert(color(1.0, 1.0, 0.1)));
	list[4] = new sphere(vec3(0, 1.5, -1), 0.3, new Light(color(4.0,4.0, 4.0)));
	/*list[5] = new sphere(vec3(-1, 1.5, -1), 0.3, new Light(color(4.0, 4.0, 4.0)));
	list[6] = new sphere(vec3(1, 1.5, -1), 0.3, new Light(color(4.0, 4.0, 4.0)));*/
	//triangle
	//list[7] = new Triangle(0,0.5,0,0.5,-2, new lambert(color(0.2, 0.2, 0.4)));

	//list[7] = new Triangle(-1,2,0,3,-2, new lambert(color(0.1, 0.2, 0.4)));

	hitable *world = new hitable_list(list, 5);
#pragma endregion
	
	color col(0, 0, 0);
	color colNoL(0, 0, 0);
	camera cam(vec3(-2,1,1),vec3(0,0,-1),vec3(0,0.3,0),50,float(nx)/float(ny));
	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			
			for (int s = 0; s < ns; s++)
			{
				float u = float(i + randF()) / float(nx);
				float v = float(j + randF()) / float(ny);
				ray r = cam.get_ray(u, v);
				vec3 p = r.point_at_parameter(2.0);
				col += colorF(r, world,0);
				colNoL += colorBeforeLight(r, world, 0);
				
			}
			//normalize the RGB values to put them in the .ppm file
			col = col / 100;
			colNoL = colNoL / 100;
			
		
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			colNoL = vec3(sqrt(colNoL[0]), sqrt(colNoL[1]), sqrt(colNoL[2]));
			
			ir = int(255.99*col[0]);
			ig = int(255.99*col[1]);
			ib = int(255.99*col[2]);

			irN = int(255.99*colNoL[0]);
			igN = int(255.99*colNoL[1]);
			ibN = int(255.99*colNoL[2]);
			adjustment += 0.001;

			//Ray Tracing do Grau A
			if (rendering == 1) {
				post.ChangeImageData(ir, ig, ib, nx, ny, nx, adjustment, col);
			}
			//Grau A
			if (rendering == 1) {
				std::cout << post.getResultR() << " " << post.getResultR() << " " << post.getResultR() << std::endl;
				img << post.getResultR() << " " << post.getResultG() << " " << post.getResultB() << std::endl;
			}
			else if (rendering == 2) {
				
				if (filter == "Yes") {
					post.ChangeImageData(ir, ig, ib, nx, ny, nx, adjustment, col);
					img << post.getResultR() << " " << post.getResultG() << " " << post.getResultB() << std::endl;
					imgNoLight << irN << " " << igN << " " << ibN << std::endl;
				}
				else if (filter == "No"){
					
					img << ir << " " << ig << " " << ib << std::endl; // Para o Grau B -> Sem transformação antes de criar imagem
					imgNoLight << irN << " " << igN << " " << ibN << std::endl;
				}
				else {
				
					img << ir << " " << ig << " " << ib << std::endl;
					imgNoLight << irN << " " << igN << " " << ibN << std::endl;
				}
				
				
			}
		}
	}
	std::cout << "DONE!" << std::endl;

	if (rendering == 2) {
		
		post.MakeAddWithNoL(filename);
		
	}

	return 0;

}


