#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/photo.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	VideoCapture cap;
	cap.open("/dev/video0", CAP_V4L2);
	if (!cap.isOpened()) {
		printf("Can't open Camera\n");
		return -1;
	}

	int width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
	int height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);	

	VideoWriter video("sobel.avi", cv::VideoWriter::fourcc('M','J','P','G'), 10, Size(width,height));

	printf("Open Camera\n");
	printf("width : %d, height : %d\n", width, height);
	float R_val, G_val, B_val;
	float average_gray;
	Mat img;
	int count = 0;
	int max;

	printf("argc : %d\n", argc);

	Mat gray(height, width, CV_8UC1);
	Mat sobelX(height, width, CV_8UC1);
	Mat sobelY(height, width, CV_8UC1);
	Mat sobel(height, width, CV_8UC1);
	Mat dst(height, width, CV_8UC3);

	if (argc > 1) {
		max = int(argv[1]);
	}
	else {
		max = 50;
	}
	printf("max : %d\n",max);
	
	while (count<=max) {
		cap.read(img);
		if (img.empty())
			break;

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				R_val = img.at<Vec3b>(i, j)[2];
				G_val = img.at<Vec3b>(i, j)[1];
				B_val = img.at<Vec3b>(i, j)[0];

				average_gray = (int)(R_val + G_val + B_val) / 3;
				gray.at<uchar>(i, j) = average_gray;
			}
		}

		Sobel(gray, sobelX, CV_8U, 1, 0);
		Sobel(gray, sobelY, CV_8U, 0, 1);
		sobel = abs(sobelX)+abs(sobelY);

		cvtColor(sobel, dst, COLOR_GRAY2BGR);	

		video.write(dst);
		count++;
		if (count%10 == 0){
			printf("%d sec\n", count/10);
		}
	}

	cap.release();
	video.release();
	return 0;
}
