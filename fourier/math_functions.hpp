//
//  math_functions.hpp
//  fourier
//
//  Created by Marvin A. Ruder on 5/31/19.
//  Copyright © 2019 Marvin A. Ruder. All rights reserved.
//

#ifndef math_functions_hpp
#define math_functions_hpp

#include "main.hpp"

int getBits(uint64_t x);

double strap(double minIn, double minOut, double maxIn, double maxOut, double value);

double X(double omega, uint64_t x);

double x(std::vector<int>& omega, double n);

int interpolate(unsigned char *data, double x, double y);

void interpolate_spectrum(std::vector<double>& xvec, std::vector<double>& yvec);

void invertData(unsigned char *data);

void invertImage(const char filename[]);

#endif /* math_functions_hpp */
