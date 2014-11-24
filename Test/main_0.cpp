#include <stdio.h>
#include "opencv2/core/core.hpp"
#include <opencv2/contrib/contrib.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv/cv.h"
#include <vector>

#include "lbp_test.h"
#include "MyDetector.h"

using namespace std;
using namespace cv;

void printMat(Mat mat)
{
	for(int i=0; i<mat.rows; i++)
	{
		for(int j=0; j<mat.cols; j++)
		{
			cout<<(int)mat.at<uchar>(i,j)<<' ';
		}
		cout<<endl;
	}
}

bool skeletonImage(Mat img, Mat& skel)
{
	const int MAX_ITERATION = 10000;
	cv::threshold(img, img, 127, 255, cv::THRESH_BINARY); 
	skel = Mat::zeros(img.size(), CV_8UC1);
	//cv::Mat skel(img.size(), CV_8UC1, cv::Scalar(0));
	cv::Mat temp;
	cv::Mat eroded;
 
	cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));
 
	bool done;
	int iterations = 0;
	do
	{
	  cv::erode(img, eroded, element);
	  cv::dilate(eroded, temp, element); // temp = open(img)
	  cv::subtract(img, temp, temp);
	  cv::bitwise_or(skel, temp, skel);
	  eroded.copyTo(img);
		
	  iterations++;

	  if(iterations > MAX_ITERATION) return false;

	  done = (countNonZero(img) == 0);
	} while (!done);

	//skel = skel(Range(1,skel.rows-1),Range(1,skel.cols-1));

	return true;
}

void test_lbp()
{
	char* item_path = "C:/Users/LingFeng/Documents/Visual Studio 2010/Projects/CVTool2/data/Chrome/APP_blf/ROW_2/ITEM_7.png";
	char* img_path = "D:/temp/img-00006.png";
    Mat img = cv::imread(img_path, 0);
    Mat item = imread(item_path,0);

    int radius, neighbors;
    radius = 1;
    neighbors = 8;

    //创建一个LBP
    //注意为了溢出，我们行列都在原有图像上减去2个半径
    Mat img_dst = Mat(img.rows-2*radius, img.cols-2*radius,CV_8UC1, Scalar(0));
    elbp1(img,img_dst);
    imshow("img_normal", img_dst);
	
	 Mat item_dst = Mat(item.rows-2*radius, item.cols-2*radius,CV_8UC1, Scalar(0));
    elbp1(item,item_dst);
    imshow("item_normal", item_dst);

	printMat(item_dst);
	/*
    Mat dst1 = Mat(img.rows-2*radius, img.cols-2*radius,CV_8UC1, Scalar(0));
    elbp(img,dst1,1,8);
    imshow("circle", dst1);
	*/

    cv::waitKey(0);
}

int main()
{
	cv::Mat frame = imread("D:/Bao Lingfeng/Dropbox/ICSE2015Demo - scvRipper/sample/img-00004.png");
	cv::Mat frame_gray, frame_edge;

	cv::cvtColor(frame, frame_gray, CV_BGR2GRAY); 

	Canny(frame_gray, frame_edge, 18, 18 * 3, 3, false );
	imwrite("D:/temp/canny.png", frame_edge);

	//MyDetector detector;
	//detector.test();

	return 0;
}