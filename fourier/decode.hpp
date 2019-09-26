//
//  decode.hpp
//  fourier
//
//  Created by Marvin A. Ruder on 5/31/19.
//  Copyright © 2019 Marvin A. Ruder. All rights reserved.
//

#ifndef decode_hpp
#define decode_hpp

#include "main.hpp"

void decode_plot(double x[], double y[], int size, std::string title);

std::vector<int> getSpectrum(unsigned char *data, int x1, int y1, int x2, int y2);

uint64_t decode(const char filename[], int precision);

uint64_t decodeSpectrum(std::vector<int> datavec);

uint64_t decodeCrop(const char filename[], int precision);

std::string decodeString(const char filename[], int precision);

std::string decodeWord(const char filename[], int precision);

#endif /* decode_hpp */



//uint64_t decodeCrop(const char filename[], int precision) { // detects a circular tag in image, crops the image and decodes tag
//	cv::Mat orig_image = cv::imread(filename);
//	cv::Mat new_image;
//	cv::resize(orig_image, new_image, cv::Size(), 1, 1, cv::INTER_CUBIC);
//	cvtColor( new_image, new_image, cv::COLOR_BGR2GRAY );
//
//	cv::GaussianBlur(new_image, new_image, cv::Size(9, 9), 25, 25);
//	std::vector<cv::Vec3f> circles;
//	cv::HoughCircles(new_image, circles, cv::HOUGH_GRADIENT, 1, 10, 100, 50, 0, 0);
//
//	cv::Mat cropped_image;
//	cv::Mat findCenter_image;
//	if (circles.size() != 0) {
//		cv::Point center;
//		int radius = 0;
//		for (size_t current_circle = 0; current_circle < circles.size(); current_circle++) { // finding largest circle
//			if (cvRound(1 * circles[current_circle][2]) > radius) {
//				center = cv::Point(cvRound(1 * circles[current_circle][0]), cvRound(1 * circles[current_circle][1]));
//				radius = cvRound(1 * circles[current_circle][2]);
//				// cv::circle(orig_image, center, radius, cv::Scalar(0, 255, 0), 2);
//			}
//		}
//
//		// finding center of tag using the effect that it is the brightest part of the tag
//		cv::Rect findCenter_rect(cvRound(center.x - 0.1 * radius), cvRound(center.y - 0.1 * radius), cvRound(0.2 * radius), cvRound(0.2 * radius));
//		cv::Mat findCenter_image;
//		cv::resize(orig_image, findCenter_image, cv::Size(), 1, 1, cv::INTER_CUBIC);
//		cvtColor( findCenter_image, findCenter_image, cv::COLOR_BGR2GRAY );
//		findCenter_image = findCenter_image(findCenter_rect);
//		cv::GaussianBlur(findCenter_image, findCenter_image, cv::Size(9, 9), 3, 3);
//
//		double min, max;
//		cv::Point min_loc, max_loc;
//		cv::minMaxLoc(findCenter_image, &min, &max, &min_loc, &max_loc);
//		max_loc.x += center.x - 0.1 * radius;
//		max_loc.y += center.y - 0.1 * radius;
//		center = max_loc;
//
//		cv::Rect cropped_rect(cvRound(center.x - border * radius), cvRound(center.y - border * radius), cvRound(2 * border * radius), cvRound(2 * border * radius));
//		cropped_image = orig_image(cropped_rect);
//		cv::GaussianBlur(cropped_image, cropped_image, cv::Size(9, 9), 2, 2);
//
//		if (debug) { // showing cropped image of detected tag
////			std::cout << center.x << "\t" << center.y << "\t" << radius << std::endl;
//			char const* window = "Detected circle";
//			namedWindow( window, cv::WINDOW_AUTOSIZE );
//			imshow( window, cropped_image );
//			cv::waitKey(0);
//		}
//		imwrite("crop.png", cropped_image);
//		return decode("crop.png", precision);
//	} else {
//		std::cerr << "No circles detected." << std::endl;
//		return 0;
//	}
//}
