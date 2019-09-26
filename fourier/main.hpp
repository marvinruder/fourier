//
//  main.hpp
//  fourier
//
//  Created by Marvin A. Ruder on 5/31/19.
//  Copyright © 2019 Marvin A. Ruder. All rights reserved.
//

#ifndef main_h
#define main_h

#include <iostream>
#include <cstring>
#include <vector>
#include <cmath>
#include <cassert>
#include <map>
#include <numeric>
#include <unistd.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#include "gnuplot_i.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

extern int maxBits;
extern double n_oscillations;
extern int bits;
extern int r;
extern double threshold;
extern bool debug;
extern double border;

double oscillations();

int main(int argc, const char * argv[]);

#include "math_functions.cpp"
#include "encode.cpp"
#include "decode.cpp"
#include "tests.cpp"
#include "gnuplot_i.c"

#endif /* main_h */
