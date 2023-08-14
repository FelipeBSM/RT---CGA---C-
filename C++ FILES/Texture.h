#pragma once
#define TEX
#include <iostream>
#include <fstream>
#include <string>
struct RGB {
	int R, G, B;
};
class Texture{

public:

	std::ifstream inTexture;
	std::string filename;
	std::string type = ".ppm";
	std::string result;
	void SelectFile() {
		std::cout << "Choose you texture:" << std::endl;
		std::cin >> filename;

		result = filename + type;
	}
	void ReadImg() {

		inTexture.open(result);
		if (!inTexture.is_open) {
			std::cout << "ERROR!. . . Cant open "<< result << "please check your file location and name."<< std::endl;
			std::cerr << EXIT_FAILURE << std::endl;
		}
		else 
		{
			while (!inTexture.eof()) {
				int data;
				inTexture >> data;
				std::cout << data << std::endl;
			}
		}
		
	}
	void 

};
