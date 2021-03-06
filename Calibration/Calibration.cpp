#include "stdafx.h"
#include <opencv2\opencv.hpp>
#include <opencv2\core.hpp>
#include <iostream>
#include <vector>

#define IMAGE_NUM 10

int main()
{
	cv::VideoCapture cap(0);
	if (!cap.isOpened()) {
		std::cerr << "Can not open camera. " << std::endl;
		system("pause");
	}

	//c270
	//cap.set(CV_CAP_PROP_FRAME_HEIGHT, 960);
	//cap.set(CV_CAP_PROP_FRAME_WIDTH, 1280);

	// web cam
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 1080);

	std::cout << "HEIGHT:" << cap.get(CV_CAP_PROP_FRAME_HEIGHT) << std::endl;
	std::cout << "WIDTH:" << cap.get(CV_CAP_PROP_FRAME_WIDTH) << std::endl;

	int c;
	cv::Mat captured,smallImage;
	int count = 0;

	cv::Size patternSize(10, 7);
	cv::TermCriteria criteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.001);
	std::vector<std::vector<cv::Point2f>> points(IMAGE_NUM);

	while (count < IMAGE_NUM)
	{
		cap >> captured;
		cv::resize(captured, smallImage, cv::Size(), 0.5, 0.5);
		cv::imshow("Camera", smallImage);
		c = cv::waitKey(1);

		if (c == 'c') {
			//コーナー点検出
			std::cout << "finding chess board corners." << std::endl;
			cv::Mat gray;
			cv::cvtColor(captured, gray, CV_RGB2GRAY);
			cv::resize(gray, smallImage, cv::Size(), 0.5, 0.5);
			cv::imshow("Camera", smallImage);
			cv::waitKey(1);

			if (cv::findChessboardCorners(gray, patternSize, points[count])) {
				cv::cornerSubPix(gray, points[count], cv::Size(11, 11), cv::Size(-1, -1), criteria);
				cv::drawChessboardCorners(gray, patternSize, (cv::Mat)(points[count]), true);
				cv::resize(gray, smallImage, cv::Size(), 0.5, 0.5);
				cv::imshow("Camera", smallImage);
				cv::waitKey(500);
				std::cout << "...Find corners from image (" << (count + 1) << "/" << IMAGE_NUM << ")" << std::endl;
				count++;
			}
			else {
				std::cout << "...least 1 corner not found. Please retry." << std::endl;
			}
		}
	}

	cv::destroyAllWindows();

	std::cout << "calibrating..." << std::endl;

	// 世界座標を決める
	std::vector<std::vector<cv::Point3f>> worldPoints(IMAGE_NUM);
	for (int i = 0; i < IMAGE_NUM; i++) {
		for (int j = 0; j < patternSize.area(); j++) {
			worldPoints[i].push_back(cv::Point3f(static_cast<float>(j % patternSize.width * 24), static_cast<float>(j / patternSize.width * 24), 0.0));
		}
	}

	//内部パラメータの計算
	cv::Mat cameraMatrix;
	cv::Mat distCoeffs;
	std::vector<cv::Mat> rotation;
	std::vector<cv::Mat> translation;
	double ret = cv::calibrateCamera(worldPoints, points, captured.size(), cameraMatrix, distCoeffs, rotation, translation);
	std::cout << "re-projection error:" << ret << std::endl;

	//パラメータの保存
	std::string filename;
	std::cout << "filename:";
	std::cin >> filename;
	cv::FileStorage fs(filename, CV_STORAGE_WRITE);
	fs << "distCoeffs" << distCoeffs;
	fs << "cameraMatrix" << cameraMatrix;
	fs.release();
	std::cout << "Save as " << filename << std::endl;

	system("pause");

    return 0;
}

