//insert your code in Part_1/p1_opencv_code.cpp
//edit the file extension and web template to match your programing language

#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {

	cv::Mat img_gry, img_bw;
	img_gry = cv::imread("../p2_search.png", 0); // flag = 0 => CV_LOAD_IMAGE_GRAYSCALE

	if (img_gry.empty()) {
		std::cout << "Not a valid image file" << std::endl;
		return -1;
	}

	cv::threshold(img_gry, img_bw, 0.5, 255, cv::THRESH_BINARY_INV);

	cv::Point2i topLeft = cv::Point2i(0, 0);
	for (int row = 0; row < img_bw.rows; ++row) {
		for (int col = 0; col < img_bw.cols; ++col) {
			if (img_bw.at<uint8_t>(row, col) > 0) {
				topLeft.x = col;
				topLeft.y = row;
				goto foundTopLeft;
			}
		}
	}
	foundTopLeft: cv::Point2i bottomRight = cv::Point2i(img_bw.cols, img_bw.rows);
	for (int row = img_bw.rows-1; row >= 0; --row) {
		for (int col = img_bw.cols-1; col >= 0; --col) {
			if (img_bw.at<uint8_t>(row, col) > 0) {
				bottomRight.x = col+1;
				bottomRight.y = row+1;
				goto foundBottomRight;
			}
		}
	}
	foundBottomRight: cv::Range rowRange = cv::Range(topLeft.y, bottomRight.y);
	cv::Range colRange = cv::Range(topLeft.x, bottomRight.x);
	img_bw(rowRange, colRange).copyTo(img_bw);

	// Filter everything except horizontal lines
	cv::Mat img_erode;
	uint8_t lineLen = 69;
	cv::Mat img_horLines;
	cv::Mat horLineSE = cv::Mat::ones(1, lineLen, CV_8UC1);
	cv::erode(img_bw, img_erode, horLineSE);
	cv::dilate(img_erode, img_horLines, horLineSE);

	// Filter everything except vertical lines
	cv::Mat img_verLines;
	cv::Mat img_verLineSE = cv::Mat::ones(lineLen, 1, CV_8UC1);
	cv::erode(img_bw, img_erode, img_verLineSE);
	cv::dilate(img_erode, img_verLines, img_verLineSE);

	// Combine horizontal and vertical lines to create grid
	cv::Mat grid, symbols, img_bw_inv;
	cv::bitwise_not(img_horLines | img_verLines, grid);
	// Remove grid from original image to isolate the symbols
	cv::bitwise_not(img_bw, img_bw_inv);
	cv::bitwise_not(grid - img_bw_inv, symbols);

	cv::imwrite("p2_image_symbols.png", symbols);
	cv::imwrite("p2_image_grid.png", grid);

	return 0;
}