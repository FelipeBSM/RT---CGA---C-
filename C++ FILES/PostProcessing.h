#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "vec3.h"
/*GET IMAGE DATA AND CHANGE ->*/
class PostProcessing {
public:
	int Largura, Altura, maxColor;
	color **Pixel;	
	color **PixelOther;
	int rendering;
	PostProcessing() {}
	void ChooseEffect() {
		label:
		std::cout << "User, Choose you effect option: [1]-GrayScale; [2]-Invert;" 
			<< "[3]-Different Binarization;[4]-One Color;[5]-Strange;[6]-?;[7]-Crazy;[8]-Old TV;[9]-Pepper and Salt;" <<
				"[10]-?????;"<< std::endl;
		std::cout << "Type [none] to select the standart one." << std::endl;
		std::cin >> effects;
		if (effects == "4") {
			std::cout << "What channel do you want to make: [R]-red;[G]-green;[B]-blue" << std::endl;
			std::cin >> channel;
		}
		if (effects == "9") {
			std::cout << "Do you want to make it with GreyScale, or not? [Yes][No]." << std::endl;
			std::cin >> effectsTwo;
			std::cout << "Ok, so now tell me, how much noise do you want? - Type Numbers ->Suggestion [5] - limit 80!" << std::endl;
			std::cin >> amount;
			if (amount > 80) {
				goto label;
			}
		}
		if (effects == "none") {
			std::cout << "Sure? - type [Yes] or [No]" << std::endl;
			std::cin >> effects;
			if (effects == "No") 
			{
				goto label;
			}
		}
		std::cout << "Ok! Writing Image..." << std::endl;
	}
	void ChangeImageData(int r, int g, int b,int x, int y, int width, int adjustment, vec3 &color) {
		
		if (effects == "1") {
			MakeGreyScaleEffect(r,g,b);
		}
		else if (effects == "2") {
			MakeInvert(r, g, b);
		}
		else if (effects == "3") {
			MakeBinEffect(160.0, r, g, b);
		}
		else if (effects == "4") {
	 		MakeOneColor(r, g, b);
		}
		else if (effects == "5") {
			MakeStrange(r,g,b);
		}
		else if (effects == "6") {
			MakeBlur(r, g, b, 0.1, width, x, y);
		}
		else if (effects == "7") {
			MakeIdontKnow(r, g, b, adjustment);
		}
		else if (effects == "8") {
			MakeExponencialIdontKnow(r, g, b, adjustment);
		}
		else if (effects == "9") {
			if (effectsTwo == "Yes") 
			{
				MakeNoise(r, g, b, amount);
			}
			else if (effectsTwo == "No") {
				MakeNoiseNormal(r, g, b, amount);
			}
			
		}
		else if (effects == "10") {
			MakeNormalized(r, g, b);
		}
		else if (effects == "11") {
			MakeGaussianMin(color, x, y);
		}
		else if (effects == "Yes") {
			red = r;
			green = g;
			blue = b;
		}
		
	}
	void MakeGreyScaleEffect(int _red,int _green,int _blue) {
		float media = (_red + _green+ _blue) / 3.0;

		_red = media; //R
		_green = media; //G
		_blue = media; //B

		red = _red;
		green = _green;
		blue = _blue;
	}
	void MakeBinEffect(float _limit, int _red, int _green, int _blue) {
		float limit;
		limit = _limit;
		float media = (_red * 0.2125 + _green*0.7154 + _blue*0.0721);

		if (media >= limit) {
			
			_red = media; //R
			_green = media; //G
			_blue = media; //B	
		}
		else {
			float grey = (_red + _green + _blue) / 3.0;
			_red = grey; //R
			_green = grey; //G
			_blue = grey; //B
		}
		

		red = _red;
		green = _green;
		blue = _blue;
	}
	void MakeInvert(int _r, int _g, int _b) {
		// Inverter Cores
		_r = _r ^ 255;
		_b = _b ^ 255;
		_g = _g ^ 255;

		red = _r;
		green = _g;
		blue = _b;
	}

	void MakeOneColor(int _r, int _g, int _b) {
		
		float media = (_r + _g + _b) / 3.0;

		if (channel == "B") {
			_b = 255;
			_r = media; //R
			_g = media;

		}
		else if (channel == "R") {

			_b = media;
			_r = 255; //R
			_g = media;
		}
		else if (channel == "G") {

			_b = media;
			_r = media; //R
			_g = 255;
		}

		red = _r;
		green = _g;
		blue = _b;
	}
	void MakeStrange(int _red, int _green, int _blue) {
		int red2, green2, blue2;
		for (int i = 0; i < 9; i++) {
			red2 = ofsstes[i] *0.1 ;
			green2 = ofsstes[i] *0.1;
			blue2 = ofsstes[i] * 0.1;

			red2 += _red * kernel[i];
			green2 += _green * kernel[i];
			blue2 += _blue * -kernel[i];
		}
		red = red2*4;
		green = green2*4;
		blue = blue2*4;
		
	}
	void MakeBlur(int r, int g, int b, float k, int width, int nx, int ny) {
		float c[3];
		
		c[0] = c[0] + k * r*(3 * (nx + ny * width + 0) - c[0]);
		r = c[0]/10000000;

		c[1] = c[1] + k * g*(3 * (nx + ny * width + 1) - c[1]);
		g = c[1]/10000000;

		c[2] = c[2] + k * b*(3 * (nx + ny * width + 2) - c[2]);
		b = c[2]/1000000;


		red = r;
		green = g;
		blue = b;
	
		
	}
	void MakeIdontKnow(int r,int g, int b, float adjust) 
	{
		r = log(r) * adjust;
		g = log(g) * adjust;
		b = log(b) * adjust;

		red = r;
		green = g;
		blue = b;
	}
	void MakeExponencialIdontKnow(int r, int g, int b, float adjust) {
		
		if (adjust > 0.2) {
			adjust-=0.1;
		}
		if (adjust < 1) {
			adjust += 0.1;
		}
		
		
		r = exp(r/adjust);
		g = exp(g/adjust);
		b = exp(b/adjust);

		red = r/1000;
		green = g/1000;
		blue = b/1000;
	}
	void MakeNoise(int r, int g, int b, int noiseParameter) {
		int random = rand() % 100;
		float media = (r + g+ b) / 3.0;

		r= media; //R
		g= media; //G
		b= media; //B

		if (random < noiseParameter) {
			r = 255;
			g = 255;
			b = 255;
			int random2 = rand() % 2;
			if (random2 == 0) {
				r = 0;
				g = 0;
				b = 0;
			}
		}

		red = r;
		green = g;
		blue = b;
	}
	void MakeNoiseNormal(int r, int g, int b, int noiseParameter) {
		int random = rand() % 100;
		red = r;
		green = g;
		blue = b;
		if (random < noiseParameter) {
			r = 255;
			g = 255;
			b = 255;
			int random2 = rand() % 2;
			if (random2 == 0) {
				r = 0;
				g = 0;
				b = 0;
			}
		}

		red = r;
		green = g;
		blue = b;
	}
	void MakeNormalized(float r, float g, float b) {
		int normalized = r + g + b;

		r = pow((r / normalized),16) * 255;
		g = pow((g / normalized),16) * 255;
		b = pow((b / normalized),16) * 255;
		
		red = pow(r,2);
		green = pow(g,2);
		blue = pow(b,2);
	}
	void MakeGaussianMin(vec3 &color, int nx,int ny){
	
		int r, g, b;
		for (int i = 0; i <= 2; i++) {
			for (int j = 0; j <= 2; j++) {
				blurUse[i][j] = blur[i][j];
			}
		}

		for (int n = 0; n < ny; n++) {
			for (int c = 0; c < nx; c++) {
				for (int ch = 0; ch < 3; ch++) {
					color[ch] = 0.0f;
					for (int ky = 0; ky <= 1; ky++) {
						color[ch*ky] *= blurUse[ky + 1][ky + 1];
					}
				}
			}
		}
		r = color[0];
		g = color[1];
		b = color[2];

		red = r;
		green = g;
		blue = b;
	
	}
	int getResultR() { return red; }
	int getResultG() { return green; }
	int getResultB() { return blue; }

	void ReadImageFile(std::string _file) {
		std::ifstream inFile;
		outputFile = _file +"V3";
		inFile.open(_file + ".ppm");

		if (!inFile.is_open()) {

			std::cout << "Your image file: " << _file << " cant be open..." << std::endl;

		}
		else {
			
			std::cout << "In image: " << _file << " is open!" << std::endl;
			std::string firstData;
			
			inFile >> firstData;
			if (strcmp(firstData.c_str(), "P3") != 0) throw("Cant acsees file!");

			inFile >> Largura >> Altura>> maxColor;
			std::cout << Largura << "/" << Altura << std::endl;
			Pixel = new color *[Largura];
			for (int X = 0; X < Largura; X++)
			{
				Pixel[X] = new color[Altura];
			}
			//-----------------------------------------------------------------------//

			//Captura as cores dos pixels
			for (int X = 0; X < Largura; X++)
			{
				for (int Y = 0; Y < Altura; Y++)
				{
					inFile >> Pixel[X][Y][0] >> Pixel[X][Y][1] >> Pixel[X][Y][2];
					std::cout << Pixel[X][Y][0] << " " << Pixel[X][Y][1] << " " << Pixel[X][Y][2] << std::endl;
					
				}
			}
			std::cout << "Done reading..." << std::endl;
			
		}
	}
	void ReadImageFileOther(std::string _file) {
		std::ifstream inFile;
		inFile.open(_file + ".ppm");

		if (!inFile.is_open()) {

			std::cout << "Your image file: " << _file << " cant be open..." << std::endl;

		}
		else {

			std::cout << "In image: " << _file << " is open!" << std::endl;
			std::string firstData;

			inFile >> firstData;
			if (strcmp(firstData.c_str(), "P3") != 0) throw("Cant acsees file!");

			inFile >> Largura >> Altura >> maxColor;
			std::cout << Largura << "/" << Altura << std::endl;
			PixelOther = new color *[Largura];
			for (int X = 0; X < Largura; X++)
			{
				PixelOther[X] = new color[Altura];
			}
			//-----------------------------------------------------------------------//

			//Captura as cores dos pixels
			for (int X = 0; X < Largura; X++)
			{
				for (int Y = 0; Y < Altura; Y++)
				{
					inFile >> PixelOther[X][Y][0] >> PixelOther[X][Y][1] >> PixelOther[X][Y][2];
					std::cout << PixelOther[X][Y][0] << " " << PixelOther[X][Y][1] << " " << PixelOther[X][Y][2] << std::endl;

				}
			}
			std::cout << "Done reading File 2..." << std::endl;

		}
	}
	void CreateOutputFile(std::string exit,int l, int a,int imageProcess) {
		std::string fileString = exit + "Outline";
		imgOut.open(fileString+".ppm");
		imgOut << "P3" << std::endl;
		imgOut << l << " " << a << std::endl;
		imgOut << "255" << std::endl;
		if (!imgOut.is_open()) {
			std::cout << "This is just one more error! Actualy i am glad that it come this far!" << std::endl;
		}
		else {
			for (int i = 0; i < l; i++) {
				for (int j = 0; j < a; j++) {
					color cor(0, 0, 0);
					cor[0] = Pixel[i][j][0];
					cor[1] = Pixel[i][j][1];
					cor[2] = Pixel[i][j][2];
					if (imageProcess == 1) {
						cor = MakeSobelPlease2(i, j);
					}
					else if (imageProcess == 2) {
						cor = AddImages(i,j);
					}
					else if (imageProcess == 3) {
						cor = MayWorkCellShading(cor,i,j);
					}
					
					std::cout << cor[0] << "/" << cor[1] << "/" << cor[2] << std::endl;
					imgOut << cor[0] << ' ' << cor[1] << ' ' << cor[2] << '\n';
				}
				imgOut << '\n';
			}
			std::cout << "File "<< fileString << " was created!"<< std::endl;
			imgOut.close();
		}
	
	}
	void CreateOutputFileOther(std::string exit,int l, int a, int imageProcess) {
		std::string fileOther = exit + "FinalImage";
		imgOutOther.open(fileOther + ".ppm");
		imgOutOther << "P3" << std::endl;
		imgOutOther << l << " " << a << std::endl;
		imgOutOther << "255" << std::endl;
		if (!imgOutOther.is_open()) {
			std::cout << "This is just one more error! Actualy i am glad that it come this far!" << std::endl;
		}
		else {
			for (int i = 0; i < l; i++) {
				for (int j = 0; j < a; j++) {
					color cor(0, 0, 0);
					cor[0] = Pixel[i][j][0];
					cor[1] = Pixel[i][j][1];
					cor[2] = Pixel[i][j][2];
					if (imageProcess == 1) {
						cor = MakeSobelPlease2(i, j);
					}
					else if (imageProcess == 2) {
						cor = AddImages(i, j);
					}
					else if (imageProcess == 3) {
						cor = MayWorkCellShading(cor, i, j);
					}

					std::cout << cor[0] << "/" << cor[1] << "/" << cor[2] << std::endl;
					imgOutOther << cor[0] << ' ' << cor[1] << ' ' << cor[2] << '\n';
				}
				imgOutOther << '\n';
			}
			std::cout << "File 2 named " << fileOther << " was created!" << std::endl;
			imgOutOther.close();
		}
	}
	inline double clamp(double x, double min, double max) {
		if (x < min) return min;
		if (x > max) return max;
		return x;
	}
	inline vec3 MakeSobelPlease2( int x, int y) {
		int A[3][3];

		color color2;
		for (int X = -1; X <= +1; X++)
		{
			for (int Y = -1; Y <= +1; Y++)
			{
				int Xl = (X + x + Largura) % Largura;
				int Ya = (Y + y + Altura) % Altura;

				A[X + 1][Y + 1] = Pixel[Xl][Ya][0];
			}

		}
		int Gx[3][3] = {
		{-1, +0, +1},
		{-2, +0, +2},
		{-1, +0, +1} };

		int Gy[3][3] = {
		{+1, +2, +1},
		{+0, +0, +0},
		{-1, -2, -1} };

		int GxA = 0;
		int GyA = 0;

		for (int X = 0; X < 3; X++)
		{
			for (int Y = 0; Y < 3; Y++)
			{
				GxA += Gx[X][Y] * A[X][Y];
				GyA += Gy[X][Y] * A[X][Y];
			}
		}

		int val = sqrt(pow(GxA, 2) + pow(GyA, 2));
		
		if (val > 255)val = 255;
		if (val < 0) val = 0;

		for (int rgb = 0; rgb < 3; rgb++) {
			color2[rgb] = val;
		}
	
		return color2;


	}
	inline vec3 AddImages(int x, int y)
	{
		std::cout << Pixel[x][y];
		color c;
		c[0] = PixelOther[x][y][0] - Pixel[x][y][0];
		c[1] = PixelOther[x][y][1] - Pixel[x][y][1];
		c[2] = PixelOther[x][y][2] - Pixel[x][y][2];

		return c;
	}
	inline vec3 MayWorkCellShading(color c, int x, int y) {
		double r = c[0];
		double g = c[1];
		double b = c[2];

		r = r / 255;
		g = g / 255;
		b = b / 255;

		double btightness = (r + g + b) / 3;
		if (btightness > 0.7 && btightness <= 0.9) {
			double modificador = 1.8 - btightness;
			r *= modificador;
			g *= modificador;
			b *= modificador;
		}

		if (btightness > 0.5 && btightness <= 0.7) {
			double modificador = 1.6 - btightness;
			r *= modificador;
			g *= modificador;
			b *= modificador;
		}
		else if (btightness <= 0.5) {
			double modificador = 1.4 - btightness;
			r *= modificador;
			g *= modificador;
			b *= modificador;
		}

		c[0] = static_cast<int>(256 * clamp(r, 0.0, 0.999));
		c[1] = static_cast<int>(256 * clamp(g, 0.0, 0.999));
		c[2] = static_cast<int>(256 * clamp(b, 0.0, 0.999));

		return c;
	}
	inline void MakeAdd(std::string initialFile) {
		ReadImageFile(initialFile);
		CreateOutputFile(initialFile,700, 700, 1);
		ReadImageFile(initialFile + "Outline");
		ReadImageFileOther(initialFile);
		CreateOutputFileOther(initialFile, 700, 700, 2);
	}
	inline void MakeAddWithCell(std::string initialFile) {
		ReadImageFile(initialFile);
		CreateOutputFile(initialFile, 200, 200, 1);
		ReadImageFile(initialFile);
		CreateOutputFileOther(initialFile, 200, 200, 3);
		ReadImageFileOther(initialFile + "FinalImage");
		ReadImageFile(initialFile + "Outline");
		CreateOutputFileOther(initialFile+"2", 200, 200, 2);
	}
	inline void MakeAddWithNoL(std::string initialFile) {
		ReadImageFile(initialFile+"NoL");
		CreateOutputFile(initialFile, 200, 200, 1);
		ReadImageFile(initialFile);
		CreateOutputFileOther(initialFile, 200, 200, 3);
		ReadImageFileOther(initialFile + "FinalImage");
		ReadImageFile(initialFile + "Outline");
		CreateOutputFileOther(initialFile + "2", 200, 200, 2);
	}
	inline void MakeAddWithOtherFilters(std::string initialFile) {
		ReadImageFile(initialFile);
		CreateOutputFile(initialFile, 200, 200, 1);
		ReadImageFile(initialFile + "Outline");
		ReadImageFileOther(initialFile);
		CreateOutputFileOther(initialFile, 200, 200, 2);
		std::string choise;
		std::cout << "Do you want to ADD another filter? Type Yes or No" << std::endl;
		std::cin >> choise;
		if (choise == "Yes") {
			std::cout << "What filter would you like to merge? " << std::endl;
			std::cout << "1 - Greyscale." << std::endl;
			std::cout << "2 - Peper&Salt." << std::endl;
			std::cout << "3 - Invert." << std::endl;
		}
		else {
			std::cout << "Ok, thats it is a no then!" << std::endl;
		}
	}
private:
	
	std::ofstream imgOut,imgOutOther;
	std::string outputFile;
	int currentRed,currentGreen,currentBlue;
	int width;
	int height;
	int amount;
	std::string effects;
	std::string effectsTwo;
	std::string channel;
	int red, green, blue;
	float offset = 1/300;
	const float ofsstes[9] = {-offset,offset,-offset,offset,-offset,offset,-offset,offset,-offset};
	const float kernel[9] = {
		4.0 / 16, 12.0 / 16, 4.0 / 16,
		2.0 / 16, 2.0 / 16, 2.0 / 16,
		4.0 / 16, 12.0 / 16, 4.0 / 16
	};
	float blur[3][3] = { 1 / 9.0,1 / 9.0,1 / 9.0,
					  1 / 9.0,1 / 9.0,1 / 9.0,
					  1 / 9.0,1 / 9.0,1 / 9.0 };
	float blurUse[3][3];
};
