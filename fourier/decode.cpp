//
//  decode.cpp
//  fourier
//
//  Created by Marvin A. Ruder on 5/31/19.
//  Copyright © 2019 Marvin A. Ruder. All rights reserved.
//

#include "decode.hpp"

void decode_plot(std::vector<double>& x, std::vector<double>& y, int size, std::string title) { // plots given spectrum and threshold using gnuplot call
	gnuplot_ctrl * plot;
	plot = gnuplot_init();
	gnuplot_setstyle(plot, (char*)"linespoints");
	gnuplot_cmd(plot, (char*)"set linetype 1 lc rgb 'red'");
	gnuplot_cmd(plot, (char*)"set linetype 2 lc rgb 'green'");
	gnuplot_cmd(plot, (char*)"set xtics 1");
	gnuplot_cmd(plot, (char*)"set grid xtics");
	gnuplot_cmd(plot, (char*)"set yrange [0:1]");
	gnuplot_plot_xy(plot, x, y, size, (char*)&title);
	gnuplot_setstyle(plot, (char*)"lines");
	char* cthreshold = (char*)malloc(256);
	sprintf(cthreshold, "%f", threshold);
	gnuplot_plot_equation(plot, cthreshold, (char*)"threshold");
	//	std::getchar();
	gnuplot_close(plot);
}

std::vector<int> getSpectrum(unsigned char *data, int x1, int y1, int x2, int y2) { // extracts value spectrum between two points in image, also removes border
	std::vector<int> spectrum;
	int dx = x2 - x1;
	int dy = y2 - y1;
	int length = ceil(sqrt(dx * dx + dy * dy));
	for (double i = 1e-9; i <= 1 - 1e-9; i += 1.0 / length) {
		spectrum.push_back(interpolate(data, x1 + dx * i, y1 + dy * i));
		// spectrum.push_back(data[(int)round(x1 + dx * i) * r + (int)round(y1 + dy * i)]);
	}
	std::vector<int> border_values;
	for (int i = 0; i < (border - 1) * r / 10; i++) { // one tenth of the border values are used to compute avg and stdev of border
		border_values.push_back(spectrum[i]);
		border_values.push_back(spectrum[spectrum.size() - i - 1]);
	}
	double sum = std::accumulate(border_values.begin(), border_values.end(), 0.0);
	double mean = sum / border_values.size();
	
	double sq_sum = std::inner_product(border_values.begin(), border_values.end(), border_values.begin(), 0.0);
	double stdev = std::sqrt(sq_sum / border_values.size() - mean * mean);
	
	double white = mean - 2.58 * stdev - 1; // using avg and stdev we define a brightness value, every value above will be interpreted as white border
	
	// removing any white border except for 3 pixels
	while (spectrum[3] >= white) {
		if (spectrum.size() == 4) {
			return spectrum;
		}
		spectrum.erase(spectrum.begin());
	}
	while (spectrum[spectrum.size() - 4] >= white) {
		spectrum.erase(spectrum.end() - 1);
	}
	
	return spectrum;
}

uint64_t decode(const char filename[], int precision) { // decodes tag from image, tag has to be in center of square image with a white border, the size of the border is given by variable border
	unsigned char *data;
	data = stbi_load(filename, &r, &r, 0, 1);
	bits = std::min(maxBits, 64) + 1;
	std::vector<uint64_t> result;
	precision /= 2;
	for (int i = 0; i < precision; i++) {
		result.push_back(decodeSpectrum(getSpectrum(data, (r - 1) * i / precision, 0, (r - 1) * (precision - i) / precision, r - 1)));
	}
	for (int i = 0; i < precision; i++) {
		result.push_back(decodeSpectrum(getSpectrum(data, r - 1, (r - 1) * i / precision, 0, (r - 1) * (precision - i) / precision)));
	}
	
	// we compare the results that we got from several spectrums of the tag
	int max = 0;
	uint64_t most_common = -1;
	std::map<uint64_t,int> m;
	for (std::vector<uint64_t>::iterator vi = result.begin(); vi != result.end(); vi++) {
		if (*vi != 0) {
			m[*vi]++;
		}
		if (m[*vi] > max) {
			max = m[*vi];
			most_common = *vi;
		}
	}
	if (true) {
		std::cout << 0.5 * max / precision << "\t";
	}
	return most_common;
}


uint64_t decodeSpectrum(std::vector<int> datavec) { // decodes a spectrum that has been extracted from a tag
	if (datavec.size() <= 4) {
		return 0;
	}
	int decode_div = 64; // resolution of frequency spectrum
	std::vector<double> xvec(bits * decode_div + 1, 0);
	std::vector<double> yvec(bits * decode_div + 1, 0);
	for (double n = 0.0; n <= bits; n += 1.0 / (double)decode_div) { // decoding value spectrum, creating frequency spectrum
		xvec[(int)round(n * decode_div)] = n;
		yvec[(int)round(n * decode_div)] = x(datavec, n);
	}
	double min = 65536.0;
	double max = -65536.0;
	for(int n = decode_div; n <= decode_div * bits; n++) {
		if (yvec[n] > max) {
			max = yvec[n];
		}
		if (yvec[n] < min) {
			min = yvec[n];
		}
	}
	
	// checking whether we have a value spectrum or just a circle without any data
	//	bool iszero = true;
	//	for (int i = 0; i < datavec.size(); i++) {
	//		if (datavec[i] > 192) {
	//			iszero = false;
	//			break;
	//		}
	//	}
	
	// normalizing the frequency spectrum
	for (int n = 0; n <= decode_div * bits; n++) {
		yvec[n] = strap(min, 0, max, 1, yvec[n]);
	}
	
	interpolate_spectrum(xvec, yvec); // interpolating the frequency spectrum so that the peaks match whole numbers
	
	// extracting bit value from frequency spectrum
	uint64_t y = 0;
	bool valid = false;
	for (int n = decode_div * bits; n >= decode_div; n -= decode_div) {
		y *= 2;
		if (yvec[n] > threshold) {
			y++;
		} else {
			if (y != 0) {
				valid = true; // this eliminates false values of the type (2^n)-1
			}
		}
	}
	
	if (!valid && y >= 255) {
		y = 0;
	}
	
	if (debug) {
		decode_plot(xvec, yvec, decode_div * (std::max(getBits(y), 9)), "spectrum"); // plotting frequency spectrum
	}
	return y;
	
}


uint64_t decodeCrop(const char filename[], int precision) { // detects a circular tag in image, crops the image and decodes tag
	
	// reading image and converting to greyscale
	cv::Mat img =  cv::imread(filename);
	cv::Mat imgGrayScale;
	cvtColor( img, imgGrayScale, cv::COLOR_BGR2GRAY );
	
	// finding contours
	std::vector<std::vector<cv::Point>> contours;
	cv::Mat canny_output;
	int thresh = 3200;
	Canny( imgGrayScale, canny_output, thresh, thresh*2, 5 );
	findContours( canny_output, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );
	
	// extracting quadrangles
	std::vector<std::vector<cv::Point>> approxCurves;
	for( int i = 0; i< contours.size(); i++ ) {
		std::vector<cv::Point> approxCurve;
		approxPolyDP(contours[i], approxCurve, 10, true);
		if (approxCurve.size() == 4) {
			approxCurves.push_back(approxCurve);
//			cv::line(img,approxCurve[0],approxCurve[1],(0,255,255),3);
//			cv::line(img,approxCurve[1],approxCurve[2],(0,255,255),3);
//			cv::line(img,approxCurve[2],approxCurve[3],(0,255,255),3);
//			cv::line(img,approxCurve[3],approxCurve[0],(0,255,255),3);
		}
	}
		
	// extracting the right quadrangle
	if (approxCurves.size() == 0) {
		std::cerr << "No tags detected." << std::endl;
		return 0;
	}
	
	int maxIndex = 0;
	double maxCirc = 0.0;
	for (int i = 0; i < approxCurves.size(); i++) {
		// check that we have not a triangle
		bool invalid = false;
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < j; k++) {
				double dx = approxCurves[i][j].x - approxCurves[i][k].x;
				double dy = approxCurves[i][j].y - approxCurves[i][k].y;
				if (sqrt(dx * dx + dy * dy) < 20) invalid = true;
			}
		}
		if (invalid) continue;
		
		double circ = 0;
		for (int j = 0; j < 4; j++) {
			double dx = approxCurves[i][j % 4].x - approxCurves[i][(j + 1) % 4].x;
			double dy = approxCurves[i][j % 4].y - approxCurves[i][(j + 1) % 4].y;
			circ += sqrt(dx * dx + dy * dy);
		}
		if (circ > maxCirc) {
			maxIndex = i;
			maxCirc = circ;
		}
	}
	
	// perspective transformation
	cv::Mat cropped;
	cv::Point2f inputQuad[4];
	cv::Point2f outputQuad[4];
	cv::Mat lambda(3, 3, CV_8UC1);
	lambda = cv::Mat::zeros(3, 3, imgGrayScale.type());
	
	for (int i = 0; i < 4; i++) {
		inputQuad[i] = approxCurves[maxIndex][i];
	}
	
	outputQuad[0] = cv::Point2f(0, 0);
	outputQuad[1] = cv::Point2f(imgGrayScale.cols-1, 0);
	outputQuad[2] = cv::Point2f(imgGrayScale.cols-1, imgGrayScale.rows-1);
	outputQuad[3] = cv::Point2f(0, imgGrayScale.rows-1);
	
	lambda = getPerspectiveTransform( inputQuad, outputQuad );
	
	warpPerspective(imgGrayScale,cropped,lambda,cropped.size());
	
	// stretching cropped image to square
	
	cv::resize(cropped, cropped, cv::Size(), 8 * maxCirc / (4.0 * cropped.cols), 8 * maxCirc / (4.0 * cropped.rows));
	
	cv::bitwise_not(cropped, cropped); // inverting image
	
	// finding center of tag using the effect that it is the brightest part of the tag
	cv::Point center(cropped.cols / 2.0, cropped.rows / 2.0);
	double radius = cropped.cols / (2.0 * border);
	
	if (maxCirc > 4 * 200) { // at smaller images the center point is not always the brightest part of the tag
		cv::Rect findCenter_rect(cvRound(center.x - 0.1 * radius), cvRound(center.y - 0.1 * radius), cvRound(0.2 * radius), cvRound(0.2 * radius));
		cv::Mat findCenter_image = cropped;
		findCenter_image = findCenter_image(findCenter_rect);
		cv::GaussianBlur(findCenter_image, findCenter_image, cv::Size(9, 9), 3, 3);
		
		double min, max;
		cv::Point min_loc, max_loc;
		cv::minMaxLoc(findCenter_image, &min, &max, &min_loc, &max_loc);
		max_loc.x += center.x - 0.1 * radius;
		max_loc.y += center.y - 0.1 * radius;
		center = max_loc;
	}
	
	int minDist = std::min(std::min(center.x, center.y), std::min(cropped.cols - center.x, cropped.rows - center.y));
	
	minDist /= 1 + (border - 1) / 4; // shrinking the image a little so that black edges disappear
	
	cv::Rect cropped_rect(center.x - minDist, center.y - minDist, 2 * minDist, 2 * minDist);
	cropped = cropped(cropped_rect);
	cv::GaussianBlur(cropped, cropped, cv::Size(9, 9), 2, 2);

	
	if (debug) { // showing cropped image of detected tag
		namedWindow("Crop", cv::WINDOW_AUTOSIZE);
		imshow("Crop", cropped);
		cv::waitKey(0);
	}
	
	imwrite("crop.png", cropped);
	return decode("crop.png", precision);
}


std::string decodeString(const char filename[], int precision) { // used to decode small (max. 8 char) string in tag
	uint64_t x = decode(filename, precision);
	std::string s;
	while (x > 0) {
		s.push_back(x % 256);
		x /= 256;
	}
	return s;
}

std::string decodeWord(const char filename[], int precision) { // used to encode small (max. 12 char) case-insensitive word in tag
	uint64_t x = decode(filename, precision);
	std::string s;
	while (x > 0) {
		s.push_back((x % 32) + 64);
		x /= 32;
	}
	return s;
}
