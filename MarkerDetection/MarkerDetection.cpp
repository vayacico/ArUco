#include "stdafx.h"
#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2\aruco.hpp>
#include <iostream>

int main()
{
	//カメラ接続
	cv::VideoCapture cap(0);
	if (!cap.isOpened()) {
		std::cerr << "faild to open camera" << std::endl;
		system("pause");
	}

	//cap.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
	//cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);

	//内部パラメータ読み込み
	std::string fileName = "webcam.xml";
	cv::Mat cameraMatrix;
	cv::Mat distCoeffs;
	while (true)
	{
		cv::FileStorage fs(fileName, CV_STORAGE_READ);
		if (!fs.isOpened()) {
			std::cout << "Can not open " << fileName << std::endl;
			std::cout << "file name:";
			std::cin >> fileName;
			continue;
		}
		cv::FileNode node(fs.fs, NULL);
		cv::read(node["cameraMatrix"], cameraMatrix);
		cv::read(node["distCoeffs"], distCoeffs);
		break;
	}
	
	cv::Mat inputImage;
	cv::Mat copyImage;
	
	int key;
	while (key = cv::waitKey(1))
	{
		if (key == 'q') {
			break;
		}

		cap >> inputImage;
		copyImage = inputImage.clone();

		std::vector<int> markerIds;
		std::vector<std::vector<cv::Point2f> > markerCorners;
		cv::Ptr<cv::aruco::DetectorParameters> param = new cv::aruco::DetectorParameters();
		(*param).markerBorderBits = 2;

		cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_100);

		cv::aruco::detectMarkers(inputImage, dictionary, markerCorners, markerIds,param);

		//std::cout << "markerIds:" << markerIds.size() << std::endl;


		if (markerIds.size() > 0) {
			std::vector<cv::Vec3d> rvecs, tvecs;
			cv::aruco::estimatePoseSingleMarkers(markerCorners, 0.073, cameraMatrix, distCoeffs, rvecs, tvecs);
			for (int i = 0; i < markerIds.size(); i++) {
				cv::aruco::drawAxis(copyImage, cameraMatrix, distCoeffs, rvecs[i], tvecs[i], 0.05);
			}
		}

		

		/*if (markerIds.size() > 0) {
			cv::aruco::drawDetectedMarkers(copyImage, markerCorners, markerIds);
		}*/

		//cv::resize(copyImage, copyImage, cv::Size(), 0.5, 0.5);

		cv::imshow("detect", copyImage);
	}

	
    return 0;
}

