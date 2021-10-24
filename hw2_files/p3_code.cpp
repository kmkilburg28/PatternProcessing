//insert your code in Part_1/p1_opencv_code.cpp
//edit the file extension and web template to match your programing language

#include <iostream>
#include <ctime>
#include <opencv2/opencv.hpp>
#include "label_components.h"

int main(int argc, char** argv) {

	cv::Mat img_gry, img_bw;
	img_gry = cv::imread("../p3_search.png", 0); // flag = 0 => CV_LOAD_IMAGE_GRAYSCALE

	if (img_gry.empty()) {
		std::cout << "Not a valid image file" << std::endl;
		return -1;
	}

	cv::threshold(img_gry, img_bw, 0.5, 255, cv::THRESH_BINARY);
	cv::Mat compMat = label_components(img_bw);

	int rows = compMat.rows;
	int cols = compMat.cols;

	double _min, max;
	cv::minMaxIdx(compMat, &_min, &max);
	int numObjects = (int)max;
	cv::Mat colors = cv::Mat(1, numObjects, CV_8UC3);
	cv::RNG rng = cv::RNG(std::time(NULL));
	rng.fill(colors, rng.UNIFORM, cv::Scalar(0,0,0), cv::Scalar(255,255,255));
	cv::Mat coloredImg = cv::Mat::zeros(rows, cols, CV_8UC3);
	colors.at<cv::Vec3b>(0,0) = cv::Vec3b(255,255,255);
	for (int row = 0; row < rows; ++row) {
		for (int col = 0; col < cols; ++col) {
			if (int id = compMat.at<ushort>(row, col)) {
				coloredImg.at<cv::Vec3b>(row, col) = colors.at<cv::Vec3b>(0, id-1);
			}
		}
	}

	cv::imwrite("p3_image.png", coloredImg);

	return 0;
}