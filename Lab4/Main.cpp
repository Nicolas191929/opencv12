#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/calib3d/calib3d.hpp"
#include <iostream>

using namespace cv;
using namespace std;

Mat src_gray;
Mat dst;

int threshold_value = 140; // ������� �������� ������ ��� ����� ������ �������� ��������
int const max_value = 255;
int const max_type = 4;
int const max_binary_value = 255;


// ������� ��� ��������� �������� ���������� � Grayscale �� ��������� ������ - ����� � code samples OpenCV
static void Threshold_Demo(int type, void*)
{
	/* 0: Binary
	1: Binary Inverted
	2: Threshold Truncated
	3: Threshold to Zero
	4: Threshold to Zero Inverted
	*/
	threshold(src_gray, dst, threshold_value, max_binary_value, type);
	imshow("threshold", dst);
}

int main(int argc, char** argv) {

	//���� � �� ������ ���������
	const int minRectDim = 200;
	const int maxRectDim = 500;

	Mat src;
	src = imread("2.jpg", IMREAD_COLOR);

	imshow("original", src);

	cvtColor(src, src_gray, COLOR_BGR2GRAY); // ���������� � Grayscale
	Threshold_Demo(0, 0); // ������ ������������ � ������ ���������� 

	Mat dst2;
	src.copyTo(dst2);
	//���������� �� ����� ���������� dst (������)
	for (int y = 0; y < dst.rows; y++) {

		for (int x = 0; x < dst.cols; x++) {

			int value = dst.at<uchar>(y, x);

			if (value == 255) { // ���� ������� ���� ����
				Rect rect;
				int count = floodFill(dst, Point(x, y), Scalar(200), &rect);
				//���������� ������ ������ ������
				if (rect.width >= minRectDim && rect.width <= maxRectDim && rect.height >= minRectDim && rect.height <= maxRectDim)
				{
					rectangle(dst2, rect, Scalar(255, 0, 0)); // ������� ����������� ������� ���������

					
				}
			}
		}
	}

	imshow("detected", dst2);

	Mat image; // ���� ��������������� ��� ������� ���������� ��� ������� �������
	src_gray.copyTo(image);

	int rows = image.rows;
	int cols = image.cols;
	int size = rows * cols;

	Mat skel = Mat::zeros(image.rows, image.cols, CV_8U); // ���� ���������� � ���������
	threshold(image, image, 140, 255, THRESH_BINARY);
	Mat element = getStructuringElement(MORPH_CROSS, Size(3, 3));

	bool done = false;
	while (!done) { // �������� ������� ���������� ��� ��������� �������
		Mat eroded;
		erode(image, eroded, element);
		Mat temp;
		dilate(eroded, temp, element); // ��������� ���� ���糿 = ��������
		subtract(image, temp, temp);
		bitwise_or(skel, temp, skel);
		eroded.copyTo(image);

		int zeros = size - countNonZero(image); // ��������� ������� ���� �� ��������� ����
		if (zeros == size) { // ����� ������ � �����, ���� ������� ���� = ������ ���������� 
			done = true;
		}
	}

	imshow("skel", skel); // ���� ���������� ���������� � ��������� 

	waitKey(0);
	return 0;
}