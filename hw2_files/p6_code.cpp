//insert your code in Part_6/p6_code.cpp
//edit the file extension and web template to match your programing language

#include <iostream>
#include <opencv2/opencv.hpp>
#include "label_components.h"
#include "find_comp_origin.h"

int main(int argc, char** argv) {

	cv::Mat img_gry, img_bw;
	img_gry = cv::imread("../p6_search.png", 0); // flag = 0 => CV_LOAD_IMAGE_GRAYSCALE

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
		"../../Symbol_Cutouts/plus.png", // needs to come before "slash.png"
		"../../Symbol_Cutouts/horizontal.png",
		"../../Symbol_Cutouts/O.png",
		"../../Symbol_Cutouts/slash.png",
		"../../Symbol_Cutouts/vertical.png",
		"../../Symbol_Cutouts/X.png"
	};
	cv::Vec3b colors[numSymbols] = {
		cv::Vec3b(255,0,0),
		cv::Vec3b(0,255,0),
		cv::Vec3b(0,0,255),
		cv::Vec3b(255,255,0),
		cv::Vec3b(255,0,255),
		cv::Vec3b(127,127,255),
		cv::Vec3b(200,127,127),
		cv::Vec3b(200,127,200)
	};
	cv::Mat equivs[numSymbols];

	for (uint8_t symInd = 0; symInd < numSymbols; ++symInd) {
		cv::Mat img_grySym, img_bwSym;
		img_grySym = cv::imread(symbolFiles[symInd], 0); // flag = 0 => CV_LOAD_IMAGE_GRAYSCALE
		if (img_grySym.empty()) {
			std::cout << "Input 2 is not a valid image file" << std::endl;
			return -1;
		}

		cv::threshold(img_grySym, img_bwSym, 0.5, 255, cv::THRESH_BINARY_INV);

		// Filter everything except Symbols
		cv::Mat SE_sym, img_sym_erode, img_sym;
		cv::Mat SE_disk = 255 * cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size2i(3,3));
		cv::erode(img_bwSym, SE_sym, SE_disk);
		cv::Point SE_sym_origin = find_comp_origin(SE_sym);
		cv::erode(img_bw, img_sym_erode, SE_sym, SE_sym_origin);
		cv::dilate(img_sym_erode, img_sym, SE_sym, SE_sym_origin);

		cv::Mat compMatSym = label_components(img_sym);
		// Get locations of Sym's
		double _min, max;
		cv::minMaxIdx(compMatSym, &_min, &max);
		ushort numSym = (ushort)max;

		cv::Mat locationsSym = cv::Mat::zeros(numSym, 2, CV_16UC1);
		for (int row = 0; row < compMatSym.rows; ++row) {
			for (int col = 0; col < compMatSym.cols; ++col) {
				if (ushort id = compMatSym.at<ushort>(row, col)) {
					locationsSym.at<ushort>(id-1, 0) = row;
					locationsSym.at<ushort>(id-1, 1) = col;
				}
			}
		}
		cv::Mat equivsSym = cv::Mat::zeros(numSym, 1, CV_16UC1);
		for (ushort symCompInd = 0; symCompInd < numSym; ++symCompInd) {
			equivsSym.at<ushort>(symCompInd, 0) = compMat.at<ushort>(
				locationsSym.at<ushort>(symCompInd, 0),
				locationsSym.at<ushort>(symCompInd, 1)
			);
		}

		equivs[symInd] = equivsSym;

		// Remove img_sym from img_bw
		cv::bitwise_not(img_bw - img_sym, img_bw);
		cv::bitwise_not(img_bw, img_bw);
	}

	int rows = compMat.rows;
	int cols = compMat.cols;

	cv::Mat coloredImg = cv::Mat(rows, cols, CV_8UC3);
	coloredImg.setTo(cv::Vec3b(255,255,255));
	for (int row = 0; row < rows; ++row) {
		for (int col = 0; col < cols; ++col) {
			if (int id = compMat.at<ushort>(row, col)) {
				cv::Vec3b color = cv::Vec3b(0,0,0);
				for (ushort symInd = 0; symInd < numSymbols; ++symInd) {
					cv::Mat equivsSym = equivs[symInd];
					for (ushort equivSymInd = 0; equivSymInd < equivsSym.rows; ++equivSymInd) {
						if (equivsSym.at<ushort>(equivSymInd, 0) == id) {
							color = colors[symInd];
							goto assignColor;
						}
					}
				}
				assignColor: coloredImg.at<cv::Vec3b>(row, col) = color;
			}
		}
	}

	cv::imwrite("p6_image.png", coloredImg);

	return 0;
}