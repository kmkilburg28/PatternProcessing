//insert your code in Part_9/p9_code.cpp
//edit the file extension and web template to match your programing language

#include <iostream>
#include <opencv2/opencv.hpp>
#include "label_components.h"
#include "find_comp_origin.h"

int main(int argc, char** argv) {

	cv::Mat img_gry, img_bw;
	img_gry = cv::imread("../p9_search.png", 0); // flag = 0 => CV_LOAD_IMAGE_GRAYSCALE

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

	cv::Mat coloredImg = cv::Mat(img_bw.rows, img_bw.cols, CV_8UC3);
	coloredImg.setTo(cv::Vec3b(255,255,255));
	for (row = 0; row < numRows; row++) {
		// Copy pattern
		ushort patLen = numFilledSquaresPerRow[row] / 2;
		cv::Vec3b colors[2] = {
			cv::Vec3b(255,0,0),
			cv::Vec3b(0,0,255)
		};
		for (int patternIter = 0; patternIter < 2; ++patternIter) {
			int colStart = patternIter * patLen;
			for (int rowCopy = 0; rowCopy < horLineOrigins[row+1] - horLineOrigins[row]; rowCopy++) {
				for (int colCopy = 0; colCopy < vertLineOrigins[patLen] - vertLineOrigins[0]; colCopy++) {
					if (img_bw.at<uint8_t>(
						horLineOrigins[row]+rowCopy,
						vertLineOrigins[colStart]+colCopy) > 0)
					{
						coloredImg.at<cv::Vec3b>(
							horLineOrigins[row]+rowCopy,
							vertLineOrigins[colStart]+colCopy
						) = colors[patternIter];
					}
				}
			}
		}
	}

	// Add the grid back in
	for (int row = 0; row < grid.rows; ++row) {
		for (int col = 0; col < grid.cols; ++col) {
			if (grid.at<uint8_t>(row, col) == 0)
			{
				coloredImg.at<cv::Vec3b>(row, col) = cv::Vec3b(0,0,0);
			}
		}
	}

	cv::imwrite("p9_image.png", coloredImg);

	return 0;
}