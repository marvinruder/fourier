//
//  math_functions.cpp
//  fourier
//
//  Created by Marvin A. Ruder on 5/31/19.
//  Copyright © 2019 Marvin A. Ruder. All rights reserved.
//

#include "math_functions.hpp"

int getBits(uint64_t x) { // returns number of bits required to store the given value
	int bits = 0;
	while (x >= 1) {
		x = x >> 1;
		bits++;
	}
	return bits;
}

double strap(double minIn, double minOut, double maxIn, double maxOut, double value) { // used for normalization of the given value using linear interpolation
	double m = (maxOut - minOut) / (maxIn - minIn);
	double b = minOut - (m * minIn);
	return m * value + b;
}

double X(double omega, uint64_t x) { // encoding function
	if (x == 0) {
		return 0.0;
	}
	double X = 0.0;
	for (int n = -1.0 * bits; n <= bits; n++) {
		if (n == 0) continue;
		uint64_t N = std::abs(n) - 1;
		X += ((x & ( 1LL << N )) >> N) * std::cos(omega * n);
	}
	return 64.0 / (double)bits * X + 127.0;
}

double x(std::vector<int>& omega, double n) { // decoding function
	double x = 0;
	double dsize = omega.size();
	for (int i = 0; i < omega.size(); i++) {
		x += (1.0 / dsize) * omega[i] * std::cos(n * (oscillations() * i/dsize));
	}
	return x;
}

int interpolate(unsigned char *data, double x, double y) { // some kind of anti-aliasing, computes sub-pixel value from 2D data array
	double subx = x - floor(x);
	double suby = y - floor(y);
	return (int)data[(int)(floor(x) * r + floor(y))] * (1.0 - subx) * (1.0 - suby)
	+ (int)data[(int)((floor(x) + 1) * r + floor(y))] * subx * (1.0 - suby)
	+ (int)data[(int)(floor(x) * r + (floor(y) + 1))] * (1.0 - subx) * suby
	+ (int)data[(int)((floor(x) + 1) * r + (floor(y) + 1))] * subx * suby;
}

void interpolate_spectrum(std::vector<double>& xvec, std::vector<double>& yvec) { // interpolates the frequency spectrum so that the last peak matches a whole number
	
	// getting the largest value of the last peak
	int i = (int)yvec.size() - 1;
	while (yvec[i] < threshold) {
		i--;
	}
	while (yvec[i] < yvec[i - 1]) {
		i--;
	}
	double peak = xvec[i];
	int value = round(peak);
	double factor = peak / value;
		
	if (factor == 1) return;
	
	// interpolating
	std::vector<double> yvec_new;
	for (int n = 0; n < yvec.size(); n++) {
		double index = factor * n;
		double subindex = index - floor(index);
		if (index >= yvec.size()) {
			yvec_new.push_back(threshold / 2);
			continue;
		}
		yvec_new.push_back(yvec[(int)floor(index)] * (1.0 - subindex) + yvec[(int)floor(index) + 1] * subindex);
	}
	yvec = yvec_new;
}


void invertData(unsigned char *data) { // inverts 2D data array of image
	for (int i = 0; i < r * r; i++) {
		data[i] = 255 - data[i];
	}
}

void invertImage(const char filename[]) {
	unsigned char *data;
	data = stbi_load(filename, &r, &r, 0, 1);
	invertData(data);
	stbi_write_png(filename, r, r, 1, data, r);
}
