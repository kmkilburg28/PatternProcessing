//insert your code in Part_EC_2/ec2_code.cpp
//edit the file extension and web template to match your programing language

#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>
#include "label_components.h"
#include "find_comp_origin.h"

cv::Mat rotateImg(cv::Mat image, double angle) {
	int width = image.cols;
	int height = image.rows;
	int scale = 1;
	cv::Mat rot = cv::getRotationMatrix2D(cv::Point2f(0,0), angle, scale) / scale; // scale later
	double sinv = rot.at<double>(0,1);
	double cosv = rot.at<double>(0,0);
	rot.at<double>(1,2) = width*sinv; // adjust row offset
	cv::Size dstSize(width*cosv + height*sinv, width*sinv + height*cosv);
	cv::Mat dst;
	cv::warpAffine(image, dst, rot, dstSize);
	cv::resize(dst, dst, cv::Size(), scale, scale); // scale now

	// cv::Mat rot = cv2.getRotationMatrix2D(cv::(image.cols/2,image.rows/2),90,1)
	// cv::Mat dst = cv2.warpAffine(image,rot,cv::(image.cols,image.rows))
	return dst;
}

cv::Mat findTopCorners(cv::Mat image) {
	cv::Point2i firstPoint; // highest Point
	for (int row = 0; row < image.rows; ++row) {
		for (int col = 0; col < image.cols; ++col) {
			if (image.at<uint8_t>(row, col) > 0) {
				firstPoint = cv::Point2i(col, row);
				if (firstPoint.x > image.cols / 2) {
					// Ensure we get the rightmost pixel
					for (int colInside = image.cols-1; colInside >= firstPoint.x; ++colInside) {
						if (image.at<uint8_t>(row, colInside) > 0) {
							firstPoint = cv::Point2i(colInside, firstPoint.y);
							goto getSecondPoint;
						}
					}
				}
				goto getSecondPoint;
			}
		}
	}
	getSecondPoint: cv::Point2i secondPoint;
	if (firstPoint.x < image.cols / 2) { // found top-left point, find top-right
		for (int col = image.cols-1; col >= firstPoint.x; --col) {
			for (int row = firstPoint.y; row < image.rows; ++row) {
				if (image.at<uint8_t>(row, col) > 0) {
					secondPoint = cv::Point2i(col, row);
					goto sortPoints;
				}
			}
		}
	}
	else { // found top-right point, find top-left
		for (int col = 0; col <= firstPoint.x; ++col) {
			for (int row = firstPoint.y; row < image.rows; ++row) {
				if (image.at<uint8_t>(row, col) > 0) {
					secondPoint = cv::Point2i(col, row);
					goto sortPoints;
				}
			}
		}
	}

	sortPoints: if (firstPoint.x > secondPoint.x) {
		// Swap so that firstPoint is leftmost
		cv::Point2i tmp = firstPoint;
		firstPoint = secondPoint;
		secondPoint = tmp;
	}
	cv::Mat points = cv::Mat(2, 2, CV_32SC1);
	points.at<int>(0,0) = firstPoint.x;
	points.at<int>(0,1) = firstPoint.y;
	points.at<int>(1,0) = secondPoint.x;
	points.at<int>(1,1) = secondPoint.y;
	return points;
}

double identifyTiltAngle(cv::Mat image) {
	cv::Mat points = findTopCorners(image);
	cv::Point2i firstPoint = cv::Point2i(points.at<int>(0,0), points.at<int>(0,1));
	cv::Point2i secondPoint = cv::Point2i(points.at<int>(1,0), points.at<int>(1,1));

	// tan(angle) = opp/adj
	// angle = tan^-1(opp/adj)
	double angle = std::atan2(secondPoint.y - firstPoint.y, secondPoint.x - firstPoint.x);

	return angle;
}

cv::Mat levelImage(cv::Mat image) {
	cv::Mat points = findTopCorners(image);
	cv::Point2i firstPoint = cv::Point2i(points.at<int>(0,0), points.at<int>(0,1));
	cv::Point2i secondPoint = cv::Point2i(points.at<int>(1,0), points.at<int>(1,1));
	cv::Mat imgCopy = cv::Mat::zeros(image.rows, image.cols, CV_8UC1);

	int midY = (firstPoint.y + secondPoint.y) / 2;
	for (int col = 0; col < image.cols; ++col) {
		int shiftY = 0;
		uint8_t topFound = 0;
		for (int row = 0; row < image.rows; ++row) {
			if (!topFound && image.at<uint8_t>(row, col) > 0) {
				topFound = 1;
				shiftY = midY - row;
			}
			if (0 <= (row + shiftY) && (row + shiftY) < imgCopy.rows) {
				imgCopy.at<uint8_t>(row+shiftY, col) = image.at<uint8_t>(row, col);
			}
		}
	}

	return imgCopy;
}

int main(int argc, char** argv) {
	const int numSearches = 2;
	const char* searchFiles[numSearches] = {
		"../ec2_NoisyPattern1.png",
		"../ec2_NoisyPattern2.png"
	};
	const char* imageFiles[numSearches] = {
		"ec2_NoisyPattern1_image.png",
		"ec2_NoisyPattern2_image.png"
	};
	for (int searchNum = 0; searchNum < numSearches; ++searchNum) {
		cv::Mat img_gry, img_bw;
		img_gry = cv::imread(searchFiles[searchNum], 0); // flag = 0 => CV_LOAD_IMAGE_GRAYSCALE

		if (img_gry.empty()) {
			std::cout << "Input 1 is not a valid image file" << std::endl;
			return -1;
		}
		cv::threshold(img_gry,  img_bw,  180, 255, cv::THRESH_BINARY_INV);

		double angle = identifyTiltAngle(img_bw) * 180 / M_PI;
		img_gry = rotateImg(img_gry, angle); // Makes grayscale, also use grayscale to lose less data
		cv::threshold(img_gry,  img_bw,  180, 255, cv::THRESH_BINARY_INV);
		
		// Level out
		img_bw = levelImage(img_bw);

		const uint8_t lineLen = 69;
		cv::Mat lineSE, img_horLines, img_vertLines;
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
				horLineWidth = row - start;
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
				vertLineWidth = col - start;
				vertLineOrigins[lineInd] = (start + col-1) / 2.0f;
				++lineInd;
				start = 0;
			}
		}
		if (start > 0) {
			vertLineOrigins[lineInd] = (start + col-1) / 2.0f;
			start = 0;
		}

		// Find grid
		cv::Mat compMat = label_components(img_bw);
		ushort gridId = 0;
		cv::Mat grid = 255*cv::Mat::ones(compMat.rows, compMat.cols, CV_8UC1);
		for (int row = 0; row < compMat.rows; row++) {
			for (int col = 0; col < compMat.cols; col++) {
				if (gridId == 0) {
					if (compMat.at<ushort>(row, col) > 0) {
						gridId = compMat.at<ushort>(row, col);
						grid.at<uint8_t>(row, col) = 0;
					}
				}
				else if (compMat.at<ushort>(row, col) == gridId) {
					grid.at<uint8_t>(row, col) = 0;
				}
			}
		}
		// Clone/Fix Grid
		cv::Mat SE = 255 * cv::Mat::ones(5, 5, CV_8UC1);
		cv::morphologyEx(grid, grid, cv::MORPH_ERODE, SE);
		cv::morphologyEx(grid, grid, cv::MORPH_DILATE, SE);
		// Remove Grid
		cv::bitwise_not(img_bw, img_bw);
		cv::bitwise_not(grid - img_bw, img_bw);
		cv::bitwise_not(img_bw, img_bw);

		// Thin down
		// SE = 255 * cv::getStructuringElement(cv::MORPH_CROSS, cv::Size2i(3,3));
		// cv::morphologyEx(img_bw, img_bw, cv::MORPH_ERODE, SE);

		// Find all filled squares
		ushort numFilledSquaresPerRow[numRows];
		for (row = 0; row < numRows; row++) {
			numFilledSquaresPerRow[row] = 0;
			cv::Range rowRange = cv::Range(horLineOrigins[row]+horLineWidth+2, horLineOrigins[row+1]-horLineWidth-2);
			for (col = 0; col < numCols; col++) {
				cv::Range colRange = cv::Range(vertLineOrigins[col]+vertLineWidth+2, vertLineOrigins[col+1]-vertLineWidth-2);
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

		cv::imwrite(imageFiles[searchNum], img_bw);
	}

	return 0;
}