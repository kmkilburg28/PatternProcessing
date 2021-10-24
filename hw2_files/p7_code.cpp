//insert your code in Part_7/p7_code.cpp
//edit the file extension and web template to match your programing language

#include <iostream>
#include <opencv2/opencv.hpp>
#include "label_components.h"
#include "find_comp_origin.h"

int main(int argc, char** argv) {

	cv::Mat img_gry, img_bw;
	img_gry = cv::imread("../p7_search.png", 0); // flag = 0 => CV_LOAD_IMAGE_GRAYSCALE

	if (img_gry.empty()) {
		std::cout << "Input 1 is not a valid image file" << std::endl;
		return -1;
	}
	cv::threshold(img_gry,  img_bw,  0.5, 255, cv::THRESH_BINARY_INV);

	cv::Mat compMat = label_components(img_bw);

	const uint8_t numSymbols = 1;
	const char* symbolFiles[numSymbols] = {
		"../../Symbol_Cutouts/X.png"
	};
	const char* replaceFiles[numSymbols] = {
		"../../Symbol_Cutouts/Y.png"
	};
	cv::Mat replacedMats[numSymbols];

	for (uint8_t symInd = 0; symInd < numSymbols; ++symInd) {
		cv::Mat img_grySym, img_gryRepl, img_bwSym, img_bwRepl;
		img_grySym = cv::imread(symbolFiles[symInd], 0); // flag = 0 => CV_LOAD_IMAGE_GRAYSCALE
		if (img_grySym.empty()) {
			std::cout << "Symbol " << symInd << " is not a valid image file" << std::endl;
			return -1;
		}
		img_gryRepl = cv::imread(replaceFiles[symInd], 0); // flag = 0 => CV_LOAD_IMAGE_GRAYSCALE
		if (img_grySym.empty()) {
			std::cout << "Replacement " << symInd << " is not a valid image file" << std::endl;
			return -1;
		}

		cv::threshold(img_grySym, img_bwSym, 0.5, 255, cv::THRESH_BINARY_INV);
		cv::threshold(img_gryRepl, img_bwRepl, 0.5, 255, cv::THRESH_BINARY_INV);

		// Filter everything except Symbols
		cv::Mat SE_sym, img_sym_erode, img_sym, img_repl;
		cv::Mat SE_disk = 255 * cv::Mat::ones(3, 3, CV_8UC1);
		cv::erode(img_bw, img_sym_erode, img_bwSym);
		cv::dilate(img_sym_erode, img_sym, img_bwSym);
		cv::dilate(img_sym, img_sym, SE_disk);
		cv::dilate(img_sym_erode, img_repl, img_bwRepl);

		replacedMats[symInd] = img_repl;

		// Remove img_sym from img_bw
		cv::bitwise_not(img_bw - img_sym, img_bw);
		cv::bitwise_not(img_bw, img_bw);
	}

	int rows = compMat.rows;
	int cols = compMat.cols;

	for (int symInd = 0; symInd < numSymbols; ++symInd) {
		img_bw = img_bw | replacedMats[symInd];
	}
	cv::bitwise_not(img_bw, img_bw);

	cv::imwrite("p7_image.png", img_bw);

	return 0;
}