//insert your code in Part_EC_1/ec1_code.cpp
//edit the file extension and web template to match your programing language

#include <iostream>
#include <opencv2/opencv.hpp>
#include "label_components.h"
#include "find_comp_origin.h"

int main(int argc, char** argv) {

	cv::Mat img_gry, img_bw;
	img_gry = cv::imread("../ec1_search.png", 0); // flag = 0 => CV_LOAD_IMAGE_GRAYSCALE

	if (img_gry.empty()) {
		std::cout << "Input 1 is not a valid image file" << std::endl;
		return -1;
	}
	cv::threshold(img_gry,  img_bw,  0.5, 255, cv::THRESH_BINARY_INV);

	cv::Mat compMat = label_components(img_bw);

	const uint8_t lineLen = 69;
	cv::Mat lineSE, img_horLines, img_vertLines, grid;
	// Locate y of rows
	lineSE = 255 * cv::Mat::ones(1, lineLen, CV_8UC1);
	cv::morphologyEx(img_bw, img_horLines, cv::MORPH_OPEN, lineSE);
	cv::Mat horCompMat = label_components(img_horLines);
	double _min, max;
	cv::Point minPoint, maxPoint;
	cv::minMaxLoc(horCompMat, &_min, &max, &minPoint, &maxPoint);
	ushort numRows = (ushort)max - 1;
	int horLineWidth = 0;
	float horLineOrigins[numRows+1];
	int start = 0;
	int lineInd = 0;
	int row = 0;
	for (row = 0; row < horCompMat.rows; ++row) {
		if (horCompMat.at<ushort>(row, maxPoint.x) > 0) {
			if (!start) {
				start = row;
			}
		}
		else if (start > 0) {
			horLineWidth = start - row;
			horLineOrigins[lineInd] = (start + row-1) / 2.0f; //substract 1 because this row is after the actual line's row
			++lineInd;
			start = 0;
		}
	}
	if (start > 0) {
		horLineOrigins[lineInd] = (start + row-1) / 2.0f;
		start = 0;
	}

	// Locate x of cols
	lineSE = 255 * cv::Mat::ones(lineLen, 1, CV_8UC1);
	cv::morphologyEx(img_bw, img_vertLines, cv::MORPH_OPEN, lineSE);
	cv::Mat vertCompMat = label_components(img_vertLines);
	cv::minMaxLoc(vertCompMat, &_min, &max, &minPoint, &maxPoint);
	ushort numCols = (ushort)max - 1;
	int vertLineWidth = 0;
	float vertLineOrigins[numCols+1];
	start = 0;
	lineInd = 0;
	int col = 0;
	for (col = 0; col < vertCompMat.cols; ++col) {
		if (vertCompMat.at<ushort>(maxPoint.y, col) > 0) {
			if (!start) {
				start = col;
			}
		}
		else if (start > 0) {
			vertLineWidth = start - col;
			vertLineOrigins[lineInd] = (start + col-1) / 2.0f;
			++lineInd;
			start = 0;
		}
	}
	if (start > 0) {
		vertLineOrigins[lineInd] = (start + col-1) / 2.0f;
		start = 0;
	}

	// Remove grid
	cv::bitwise_not(img_horLines | img_vertLines, grid);
	cv::bitwise_not(img_bw, img_bw);
	cv::bitwise_not(grid - img_bw, img_bw);
	cv::bitwise_not(img_bw, img_bw);

	// Find all filled squares
	ushort numFilledSquaresPerRow[numRows];
	for (row = 0; row < numRows; row++) {
		numFilledSquaresPerRow[row] = 0;
		cv::Range rowRange = cv::Range(horLineOrigins[row], horLineOrigins[row+1]);
		for (col = 0; col < numCols; col++) {
			cv::Range colRange = cv::Range(vertLineOrigins[col], vertLineOrigins[col+1]);
			cv::Mat img_bw_subsquare;
			img_bw(rowRange, colRange).copyTo(img_bw_subsquare);
			cv::minMaxLoc(img_bw_subsquare, &_min, &max);
			if (max > 0) {
				++numFilledSquaresPerRow[row];
			}
		}
	}

	for (row = 0; row < numRows; row++) {
		// Copy pattern
		ushort patLen = numFilledSquaresPerRow[row];
		cv::Range rowRange = cv::Range(horLineOrigins[row], horLineOrigins[row+1]);
		cv::Range colRange = cv::Range(vertLineOrigins[0], vertLineOrigins[patLen]);
		cv::Mat copiedPattern;
		img_bw(rowRange, colRange).copyTo(copiedPattern);
		for (int colStart = patLen; colStart < numCols; colStart += patLen) {
			if (colStart + patLen > numCols)
				{
				// Crop the pattern to fit in the grid
				colRange = cv::Range(vertLineOrigins[0], vertLineOrigins[numCols - colStart]);
				img_bw(rowRange, colRange).copyTo(copiedPattern);
			}
			// Paste pattern
			for (int rowCopy = 0; rowCopy < copiedPattern.rows; rowCopy++) {
				for (int colCopy = 0; colCopy < copiedPattern.cols; colCopy++) {
					img_bw.at<uint8_t>(
						horLineOrigins[row]+rowCopy,
						vertLineOrigins[colStart]+colCopy
					) = copiedPattern.at<uint8_t>(rowCopy, colCopy);
				}
			}
		}
	}

	// Add the grid back in
	cv::bitwise_not(grid, grid);
	img_bw = img_bw | grid;
	cv::bitwise_not(img_bw, img_bw);

	cv::imwrite("ec1_image.png", img_bw);

	return 0;
}