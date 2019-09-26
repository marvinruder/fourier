//
//  tests.cpp
//  fourier
//
//  Created by Marvin A. Ruder on 5/31/19.
//  Copyright © 2019 Marvin A. Ruder. All rights reserved.
//

#include "tests.hpp"

void testEncodeDecode(uint64_t x) { // tests encoding the given value in a tag and decoding of the tag
	char filename[32];
	sprintf(filename, "%lld.png", x);
	encode(x, filename);
	invertImage(filename);
	std::cout << x << "\t" << decode(filename, 8) << std::endl;
}

void testEncodeDecodeString(std::string s) { // tests encoding the given string in a tag and decoding of the tag
	char filename[16];
	sprintf(filename, "%s.png", s.c_str());
	encodeString(s, filename);
	invertImage(filename);
	std::cout << s << "\t" << decodeString(filename, 8) << std::endl;
}

void testEncodeDecodeWord(std::string s) { // tests encoding the given word in a tag and decoding of the tag
	char filename[24];
	sprintf(filename, "%s.png", s.c_str());
	encodeWord(s, filename);
	invertImage(filename);
	std::cout << s << "\t" << decodeWord(filename, 8) << std::endl;
}
