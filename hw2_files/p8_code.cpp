//insert your code in Part_8/p8_code.cpp
//edit the file extension and web template to match your programing language

#include <iostream>
#include <opencv2/opencv.hpp>
#include "label_components.h"
#include "find_comp_origin.h"

int main(int argc, char** argv) {

	cv::Mat img_gry, img_bw;
	img_gry = cv::imread("../p8_search.png", 0); // flag = 0 => CV_LOAD_IMAGE_GRAYSCALE

	if (img_gry.empty()) {
		std::cout << "Input 1 is not a valid image file" << std::endl;
		return -1;
	}
	cv::threshold(img_gry,  img_bw,  0.5, 255, cv::THRESH_BINARY_INV);

	cv::Mat compMat = label_components(img_bw);

	const uint8_t lineLen = 69;
	cv::Mat lineSE, img_horLines, img_vertLines, grid;
	lineSE = 255 * cv::Mat::ones(1, lineLen, CV_8UC1);
	cv::morphologyEx(img_bw, img_horLines, cv::MORPH_OPEN, lineSE);
	lineSE = 255 * cv::Mat::ones(lineLen, 1, CV_8UC1);
	cv::morphologyEx(img_bw, img_vertLines, cv::MORPH_OPEN, lineSE);
	cv::bitwise_not(img_horLines | img_vertLines, grid);
	cv::bitwise_not(img_bw, img_bw);
	cv::bitwise_not(grid - img_bw, img_bw);
	cv::bitwise_not(img_bw, img_bw);

	const uint8_t numSymbols = 8;
	const char* symbolFiles[numSymbols] = {
		"../../Symbol_Cutouts/backslash.png",
		"../../Symbol_Cutouts/dot.png",
		"../../Symbol_Cutouts/plus.png",
		"../../Symbol_Cutouts/horizontal.png",
		"../../Symbol_Cutouts/O.png",
		"../../Symbol_Cutouts/slash.png",
		"../../Symbol_Cutouts/vertical.png",
		"../../Symbol_Cutouts/X.png"
	};
	const char* replaceFiles[numSymbols] = {
		"../../Symbol_Cutouts/dot.png",
		"../../Symbol_Cutouts/plus.png",
		"../../Symbol_Cutouts/horizontal.png",
		"../../Symbol_Cutouts/O.png",
		"../../Symbol_Cutouts/slash.png",
		"../../Symbol_Cutouts/vertical.png",
		"../../Symbol_Cutouts/X.png",
		"../../Symbol_Cutouts/backslash.png"
	};
	cv::Mat replacedMats[numSymbols];

	for (uint8_t symInd = 0; symInd < numSymbols; ++symInd) {
		cv::Mat img_grySym, img_gryRepl, img_bwSym, img_bwRepl;
		img_grySym = cv::imread(symbolFiles[symInd], 0); // flag = 0 => CV_LOAD_IMAGE_GRAYSCALE
		if (img_grySym.empty()) {
			std::cout << "Symbol " << std::to_string(symInd) << " is not a valid image file" << std::endl;
			return -1;
		}
		img_gryRepl = cv::imread(replaceFiles[symInd], 0); // flag = 0 => CV_LOAD_IMAGE_GRAYSCALE
		if (img_grySym.empty()) {
			std::cout << "Replacement " << std::to_string(symInd) << " is not a valid image file" << std::endl;
			return -1;
		}

		cv::threshold(img_grySym, img_bwSym, 0.5, 255, cv::THRESH_BINARY_INV);
		cv::threshold(img_gryRepl, img_bwRepl, 0.5, 255, cv::THRESH_BINARY_INV);

		// Filter everything except Symbols
		cv::Mat SE_sym, img_sym_erode, img_sym, img_repl;
		cv::Mat SE_disk = 255 * cv::Mat::ones(7, 7, CV_8UC1);
		cv::Point SE_sym_origin = find_comp_origin(img_bwSym);
		cv::erode(img_bw, img_sym_erode, img_bwSym, SE_sym_origin);
		cv::Mat tmp;
		cv::dilate(img_sym_erode, tmp, img_bwSym, SE_sym_origin);
		cv::dilate(tmp, img_sym, SE_disk);
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

	cv::bitwise_not(grid, grid);
	img_bw = img_bw | grid;
	cv::bitwise_not(img_bw, img_bw);

	cv::imwrite("p8_image.png", img_bw);

	return 0;
}