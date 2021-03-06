#include "stdafx.h"
#include <iostream>
#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\aruco.hpp>

int main()
{
	std::vector<cv::Mat> markerImages;
	cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_100);

	int num;
	std::cout << "marker num : ";
	std::cin >> num;

	markerImages.resize(num);

	for (int i = 0; i < num; i++) {
		cv::aruco::drawMarker(dictionary, i, 400, markerImages[i], 1);
	}

	for (int i = 0; i < num; i++ ) {
		cv::imshow("marker" + std::to_string(i), markerImages[i]);
	}

	cv::waitKey(0);

    return 0;
}

