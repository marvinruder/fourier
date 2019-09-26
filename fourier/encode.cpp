//
//  encode.cpp
//  fourier
//
//  Created by Marvin A. Ruder on 5/31/19.
//  Copyright © 2019 Marvin A. Ruder. All rights reserved.
//

#include "encode.hpp"

void setBits(uint64_t x) { // sets number of bits and radius of tag according to data to be encoded
	bits = getBits(x);
	r = oscillations() * maxBits * border;
}

void normalizeData(unsigned char *data) { // normalizes 2D data array of image to use all available values between 0 and 255, thereby maximizing contrast
	int min = 255;
	int max = 0;
	
	// getting max and min values
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < r; j++) {
			if ((i-r/2) * (i-r/2) + (j-r/2) * (j-r/2) < r/(2 * border) * r/(2 * border)) {
				if (data[i * r + j] > max) {
					max = data[i * r + j];
				}
				if (data[i * r + j] < min) {
					min = data[i * r + j];
				}
			}
		}
	}
	
	// setting values accordingly
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < r; j++) {
			if ((i-r/2) * (i-r/2) + (j-r/2) * (j-r/2) < r/(2 * border) * r/(2 * border)) {
				data[i * r + j] = std::max((int)strap(min, 0.0, max, 255.0, data[i * r + j]), 0);
			}
		}
	}
}

void encode(uint64_t x, const char filename[]) { // encodes the given value x and writes the tag as png file with given filename
	setBits(x);
	double dr = r;
	unsigned char data[r * r];
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < r; j++) {
			if ((i-r/2) * (i-r/2) + (j-r/2) * (j-r/2) < r/(2 * border) * r/(2 * border)) {
				data[i * r + j] = (int)(X(oscillations()/dr * border * std::sqrt((i-r/2) * (i-r/2) + (j-r/2) * (j-r/2)),x));
			} else {
				data[i * r + j] = 255;
			}
		}
	}
	normalizeData(data);
	invertData(data);
	stbi_write_png(filename, r, r, 1, data, r);
}

void encodeString(std::string s, const char filename[]) { // used to encode small (max. 8 char) string in tag
	if (s.size() > 8) {
		throw 1; // string too long
	}
	uint64_t x = 0;
	for (int i = 0; i < s.size(); i++) {
		x += ((uint64_t)s[i] << (8 * i));
	}
	encode(x, filename);
}

void encodeWord(std::string s, const char filename[]) { // used to encode small (max. 12 char) case-insensitive word in tag
	if (s.size() > 12) {
		throw 1; // word too ling
	}
	uint64_t x = 0;
	for (int i = 0; i < s.size(); i++) {
		if ((int)(s[i]) < 64 || (int)(s[i]) >= 128) {
			throw 2; // wrong character
		} else if ((int)(s[i]) >= 96) {
			s[i] -= 32; // converting lower case letter to upper case letter
		}
		x += ((uint64_t)(s[i] - 64) << (5 * i));
	}
	encode(x, filename);
}
