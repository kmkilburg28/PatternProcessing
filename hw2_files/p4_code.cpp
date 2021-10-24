//insert your code in Part_1/p1_opencv_code.cpp
//edit the file extension and web template to match your programing language

#include <iostream>
#include <opencv2/opencv.hpp>
#include "label_components.h"
#include "find_comp_origin.h"

int main(int argc, char** argv) {

	cv::Mat img_gry, img_bw;
	img_gry = cv::imread("../p4_search.png", 0); // flag = 0 => CV_LOAD_IMAGE_GRAYSCALE

	if (img_gry.empty()) {
		std::cout << "Input 1 is not a valid image file" << std::endl;
		return -1;
	}

	cv::Mat img_gryX, img_bwX;
	img_gryX = cv::imread("../../Symbol_Cutouts/X.png", 0); // flag = 0 => CV_LOAD_IMAGE_GRAYSCALE

	if (img_gryX.empty()) {
		std::cout << "Input 2 is not a valid image file" << std::endl;
		return -1;
	}

	cv::threshold(img_gry,  img_bw,  0.5, 255, cv::THRESH_BINARY_INV);
	cv::threshold(img_gryX, img_bwX, 0.5, 255, cv::THRESH_BINARY_INV);


	// Filter everything except Xs
	cv::Mat SE_X, img_X_erode, img_X;
	cv::Mat SE_disk = 255 * cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size2i(3,3));
	cv::erode(img_bwX, SE_X, SE_disk);
	cv::Point SE_X_origin = find_comp_origin(SE_X);
	cv::erode(img_bw, img_X_erode, SE_X, SE_X_origin);
	cv::dilate(img_X_erode, img_X, SE_X, SE_X_origin);

	// cv::Mat compMat = label_components(img_X);
	cv::Mat compMat = label_components(img_bw);
	cv::Mat compMatX = label_components(img_X);
	// Get locations of X's
	double _min, max;
	cv::minMaxIdx(compMatX, &_min, &max);
	ushort numX = (ushort)max;

	cv::Mat locationsX = cv::Mat::zeros(numX, 2, CV_16UC1);
	for (int row = 0; row < compMatX.rows; ++row) {
		for (int col = 0; col < compMatX.cols; ++col) {
			if (ushort id = compMatX.at<ushort>(row, col)) {
				locationsX.at<ushort>(id-1, 0) = row;
				locationsX.at<ushort>(id-1, 1) = col;
			}
		}
	}
	cv::Mat equivsX = cv::Mat::zeros(numX, 1, CV_16UC1);
	for (ushort xComp = 0; xComp < numX; ++xComp) {
		equivsX.at<ushort>(xComp, 0) = compMat.at<ushort>(
			locationsX.at<ushort>(xComp, 0),
			locationsX.at<ushort>(xComp, 1)
		);
	}

	int rows = compMat.rows;
	int cols = compMat.cols;

	cv::Mat coloredImg = cv::Mat(rows, cols, CV_8UC3);
	coloredImg.setTo(cv::Vec3b(255,255,255));
	for (int row = 0; row < rows; ++row) {
		for (int col = 0; col < cols; ++col) {
			if (int id = compMat.at<ushort>(row, col)) {
				cv::Vec3b color = cv::Vec3b(0,0,0);
				for (ushort equivXInd = 0; equivXInd < equivsX.rows; ++equivXInd) {
					if (equivsX.at<ushort>(equivXInd, 0) == id) {
						color = cv::Vec3b(0,0,255);
						goto assignColor;
					}
				}
				assignColor: coloredImg.at<cv::Vec3b>(row, col) = color;
			}
		}
	}

	cv::imwrite("p4_image.png", coloredImg);

	return 0;
}