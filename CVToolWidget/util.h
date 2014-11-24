#pragma once
#include<Windows.h>
#include<iostream>
#include<string>
#include<vector>
#include<sstream>
#include<hash_map>
#include<hash_set>
#include<set>
#include<algorithm>
#include<qimage.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/contrib/contrib.hpp"
using namespace std;
using namespace cv;

void trim_blf(string& str);
vector<string> split_blf(string& str, string delim);
string join_blf(vector<string> sarray,string sep);
void replaceAll_blf(string& str, string from,string to);
void toLowcase_blf(string& str);

inline QImage Mat2QImage(cv::Mat const& src)
{
     cv::Mat temp; // make the same cv::Mat
     cvtColor(src, temp,CV_BGR2RGB); // cvtColor Makes a copt, that what i need
     QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
     dest.bits(); // enforce deep copy, see documentation 
     // of QImage::QImage ( const uchar * data, int width, int height, Format format )
     return dest;
}

inline cv::Mat QImage2Mat(QImage const& src)
{
     cv::Mat tmp(src.height(),src.width(),CV_8UC3,(uchar*)src.bits(),src.bytesPerLine());
     cv::Mat result; // deep copy just in case (my lack of knowledge with open cv)
     cvtColor(tmp, result,CV_BGR2RGB);
     return result;
}

template<typename T>
T stringToNumber_blf(const string& numberAsString)
{
    T val;

    std::stringstream stream(numberAsString);
    stream >> val;
    if (stream.fail()) {
        std::runtime_error e(numberAsString);
        throw e;
    }

    return val;
}

template<typename T>
vector<T> subvector(vector<T> v, int first, int last)
{
	vector<T>::const_iterator first_it = v.begin() + first;
	vector<T>::const_iterator last_it = v.begin() + last;

	vector<T> newV(first_it,last_it);
	return newV;
}

inline double corr2(Mat A, Mat B)
{
	int channel = A.channels();
	if(channel > 1)
	{
		cout<<"channel is bigger than 1"<<endl;
	}

	Scalar meanA = cv::mean(A);
	Scalar meanB = cv::mean(B);
	
	double ma = meanA[0];
	double mb = meanB[0];

	Mat doubleA;
	A.convertTo(doubleA,CV_64FC1);
	Mat doubleB;
	B.convertTo(doubleB,CV_64FC1);

	double sum = 0;
	double sum1 = 0;
	double sum2 = 0;
	for(int i=0; i<doubleA.rows; i++)
	{
		for(int j=0; j<doubleA.cols; j++)
		{
			sum += (doubleA.at<double>(i,j) - ma) * (doubleB.at<double>(i,j) - mb);
			sum1 += (doubleA.at<double>(i,j) - ma) * (doubleA.at<double>(i,j) - ma);
			sum2 += (doubleB.at<double>(i,j) - mb) * (doubleB.at<double>(i,j) - mb);
		}
	}

	double result = sum / (sqrt(sum1) * sqrt(sum2));

	return result;
}