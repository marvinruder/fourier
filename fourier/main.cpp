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
	
	unsigned char *data8, *data32;
	std::map<uint64_t,int> m, n;
	std::vector<double> x, y, z;
	data8 = stbi_load("fourier08bit.png", &r, &r, 0, 1);
	data32 = stbi_load("fourier32bit.png", &r, &r, 0, 1);

	// getting max and min values
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < r; j++) {
			if ((i-r/2) * (i-r/2) + (j-r/2) * (j-r/2) < r/(2 * border) * r/(2 * border)) {
				m[data8[i * r + j]]++;
				n[data32[i * r + j]]++;
			}
		}
	}

	for (int i = 0; i < 1 << 8; i++) {
		std::cout << i << "\t" << m[i] << "\t" << n[i] << std::endl;
		x.push_back(i);
		y.push_back(m[i]);
		z.push_back(n[i]);
	}
	
	gnuplot_ctrl * plot;
	plot = gnuplot_init();
	gnuplot_setstyle(plot, (char*)"linespoints");
	gnuplot_cmd(plot, (char*)"set linetype 1 lc rgb 'red'");
	gnuplot_cmd(plot, (char*)"set linetype 2 lc rgb 'green'");
	gnuplot_cmd(plot, (char*)"set xtics 32");
	gnuplot_cmd(plot, (char*)"set grid xtics");
	gnuplot_cmd(plot, (char*)"set xrange [0:255]");
	gnuplot_cmd(plot, (char*)"set xlabel 'Pixel value'");
	gnuplot_cmd(plot, (char*)"set xlabel 'Frequency'");
	gnuplot_plot_xy(plot, x, y, 256, (char*)"Histogram 8-bit");
	gnuplot_plot_xy(plot, x, z, 256, (char*)"Histogram 32-bit");
	//	std::getchar();
	gnuplot_close(plot);

//	testEncodeDecode(210);
//	testEncodeDecodeWord("Cheeeeeeeese");
//	testEncodeDecodeString("Cheeeese");
	
//	encode(20, "fourier05bit.png");
//	encode(100, "fourier07bit.png");
	
//	encode(210, "fourier08bit.png");
//	encode(3373, "fourier12bit.png");
//	encode(53970, "fourier16bit.png");
//	encode(863533, "fourier20bit.png");
//	encode(13816530, "fourier24bit.png");
//	encode(221064493, "fourier28bit.png");
//	encode(3537031890, "fourier32bit.png");
	
	return 0;
	
}
