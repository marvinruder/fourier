//
//  encode.hpp
//  fourier
//
//  Created by Marvin A. Ruder on 5/31/19.
//  Copyright © 2019 Marvin A. Ruder. All rights reserved.
//

#ifndef encode_hpp
#define encode_hpp

#include "main.hpp"

void setBits(uint64_t x);

void normalizeData(unsigned char *data);

void encode(uint64_t x, const char filename[]);

void encodeString(std::string s, const char filename[]);

void encodeWord(std::string s, const char filename[]);


#endif /* encode_hpp */
