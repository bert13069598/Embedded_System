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
	int height, width;
	int n_height, n_width;
	float R_val, G_val, B_val;
	float average_gray;
	Mat img;

	if (argc > 1)
	{
		img = imread(argv[1], IMREAD_COLOR);
	}
	else
	{
		img = imread("sample_image.bmp", IMREAD_COLOR);
	}

	if (img.empty())
	{
		printf("image load error\n");
		return -1;
	}

	height = img.rows;
	width = img.cols;
	Mat gray(height, width, CV_8UC1);
    Mat sobelX(height, width, CV_8UC1);
    Mat sobelY(height, width, CV_8UC1);
    Mat sobel(height, width, CV_8UC1);

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

	imwrite("sobel_image.bmp", sobel);
	return 0;
}
