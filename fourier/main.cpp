//
//  main.cpp
//  fourier
//
//  Created by Marvin A. Ruder on 5/19/19.
//  Copyright © 2019 Marvin A. Ruder. All rights reserved.
//

#include "main.hpp"

int maxBits = 32; // maximum number of bits to be encoded in a tag
double n_oscillations = 4; // number of oscillations across the diameter of the tag

double oscillations() { // internal value to realize number of oscillations
	return n_oscillations * 2 * M_PI;
}

int bits = maxBits; // number of bits to be encoded in a tag (redefined later)
double border = 1.125; // size of white border around tag
int r = oscillations() * maxBits * border; // radius of tag (redefined later)
double threshold = 0.54; // threshold value between low and high bit
bool debug = false; // enables graph and image output

int main(int argc, const char * argv[]) {
		
//	testEncodeDecode(210);
//	testEncodeDecodeWord("Cheeeeeeeese");
//	testEncodeDecodeString("Cheeeese");
	
	std::cout << decodeCrop("210_print.png") << std::endl;
	std::cout << decodeCrop("210_large.png") << std::endl;
	std::cout << decodeCrop("210_test.png") << std::endl;

	return 0;
	
}

