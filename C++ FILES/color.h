#pragma once
#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>
#include <cmath>
inline double clamp(double x, double min, double max) {
	if (x < min) return min;
	if (x > max) return max;
	return x;
}
void write_color(std::ostream& out, color pixel_color, int samples_per_pixel) {

	auto r = pixel_color.x();
	auto g = pixel_color.y();
	auto b = pixel_color.z();

	// Divide the color total by the number of samples and gamma-correct for gamma=2.0.
	auto scale = 1.0 / samples_per_pixel;
	r = sqrt(scale * r);
	g = sqrt(scale * g);
	b = sqrt(scale * b);

	// Write the translated [0,255] value of each color component.
	out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';

}
//Filtros durante processamento
//semi_cell_shaded
void write_color_cell_shaded(std::ostream& out, color pixel_color, int samples_per_pixel) {

	auto r = pixel_color.x();
	auto g = pixel_color.y();
	auto b = pixel_color.z();

	// Divide the color total by the number of samples and gamma-correct for gamma=2.0.

	auto scale = 1.0 / samples_per_pixel;
	r = sqrt(scale * r);
	g = sqrt(scale * g);
	b = sqrt(scale * b);

	double intensidade = (r + g + b) / 3;
	if (intensidade > 0.7 && intensidade <= 0.9) {
		double modificador = 1.8 - intensidade;
		r *= modificador;
		g *= modificador;
		b *= modificador;
	}

	if (intensidade > 0.5 && intensidade <= 0.7) {
		double modificador = 1.6 - intensidade;
		r *= modificador;
		g *= modificador;
		b *= modificador;
	}
	else if (intensidade <= 0.5) {
		double modificador = 1.4 - intensidade;
		r *= modificador;
		g *= modificador;
		b *= modificador;
	}

	// Write the translated [0,255] value of each color component.
	out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';

}

//inverter cor
void write_color_inverted(std::ostream& out, color pixel_color, int samples_per_pixel) {

	auto r = pixel_color.x();
	auto g = pixel_color.y();
	auto b = pixel_color.z();

	// Divide the color total by the number of samples and gamma-correct for gamma=2.0.
	auto scale = 1.0 / samples_per_pixel;
	r = sqrt(scale * r);
	g = sqrt(scale * g);
	b = sqrt(scale * b);

	out << static_cast<int>(256 - 256 * clamp(r, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 - 256 * clamp(g, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 - 256 * clamp(b, 0.0, 0.999)) << '\n';

}

//Mono
void write_color_mono(std::ostream& out, color pixel_color, int samples_per_pixel, color mono) {

	auto r = pixel_color.x();
	auto g = pixel_color.y();
	auto b = pixel_color.z();

	// Divide the color total by the number of samples and gamma-correct for gamma=2.0.
	auto scale = 1.0 / samples_per_pixel;
	r = sqrt(scale * r);
	g = sqrt(scale * g);
	b = sqrt(scale * b);

	// Write the translated [0,255] value of each color component.
	out << static_cast<int>(256 * clamp(r, 0.0, 0.999 * mono[0])) << ' '
		<< static_cast<int>(256 * clamp(g, 0.0, 0.999 * mono[1])) << ' '
		<< static_cast<int>(256 * clamp(b, 0.0, 0.999 * mono[2])) << '\n';

}

//Brilho
void write_color_brightness(std::ostream& out, color pixel_color, int samples_per_pixel, double brightness) {

	auto r = pixel_color.x();
	auto g = pixel_color.y();
	auto b = pixel_color.z();

	// Divide the color total by the number of samples and gamma-correct for gamma=2.0.
	auto scale = 1.0 / samples_per_pixel;
	r = sqrt(scale * r) + brightness;
	g = sqrt(scale * g) + brightness;
	b = sqrt(scale * b) + brightness;

	// Write the translated [0,255] value of each color component.
	out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

//Binario
void write_color_binary(std::ostream& out, color pixel_color, int samples_per_pixel, double binary) {

	auto r = pixel_color.x();
	auto g = pixel_color.y();
	auto b = pixel_color.z();

	// Divide the color total by the number of samples and gamma-correct for gamma=2.0.
	auto scale = 1.0 / samples_per_pixel;
	r = sqrt(scale * r);
	g = sqrt(scale * g);
	b = sqrt(scale * b);

	double media = (r + g + b) / 3;
	if (media >= binary) {
		r = 1.0;
		g = 1.0;
		b = 1.0;
	}
	else {
		r = 0.0;
		g = 0.0;
		b = 0.0;
	}

	// Write the translated [0,255] value of each color component.
	out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}
#endif