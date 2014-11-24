#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/contrib/contrib.hpp"
#include <iostream>
#include <stdio.h>
#include <vector>
#include "CProperties.h"
#include "MyDefines.h"

using namespace std;
using namespace cv;




extern void convolveDFT(Mat& A, Mat& B, Mat& C, int flag=1);

extern int myMatchAlphaTemplate(Size imgSize, Size tplSize, Mat& rst, 
	Mat& F_a2, Mat& F_a2T, double aT, 
	Mat& F_I, Mat& F_I2, int method=0);
extern int myMatchAlphaTemplate(Mat& img, Mat& tpl, Mat& msk, Mat& rst,  
	Mat& F_a2, Mat& F_a2T, double aT, Mat& F_I, Mat& F_I2, int method=0);
extern int myMatchAlphaTemplate(Mat& img, Mat& tpl, Mat& msk, Mat& rst,  
	Mat& F_a2, Mat& F_a2T, double aT, int method=0);
extern int myMatchAlphaTemplate(Mat& img, Mat& tpl, Mat& msk, Mat& rst, 
	int method=0, int dftWidth=0, int dftHeight=0);

Rect_<float> getOverlap( Rect_<float> rc1, Rect_<float> rc2 );
Rect         getOverlap( Rect         rc1, Rect         rc2 );
void getJetColors( vector<Scalar>& colors, int sz);


inline extern bool im2float(Mat& img_u, Mat& img_f)
{
	if(img_u.depth()!=CV_8U){
		return false;
	}
	int type_f = CV_32FC3;
	//int type_u = img_u.type();
	//if( type_u!=CV_8UC1 && type_u!=CV_8UC3){
	//	return false;
	//}
	if(img_u.channels() == 1)
		type_f = CV_32FC1;
	img_u.convertTo(img_f, type_f);
	img_f *= 1.0/255;
	return true;
}

inline extern bool im2double(Mat& img_u, Mat& img_lf)
{
	if(img_u.depth()!=CV_8U){
		return false;
	}
	int type_lf = CV_64FC3;

	if(img_u.channels() == 1)
		type_lf = CV_64FC1;

	img_u.convertTo(img_lf, type_lf);
	img_lf *= 1.0/255;
	return true;
}


template< class T > int vecMskErase(vector<T>& obj, vector<int> msk)
{
	if( msk.size() > obj.size())
	{
		return -1;
	}

	unsigned int i;
	int cnt=0;

	vector<int> msk_id(msk.size()+1);
	msk_id[0] = 0;
	for( i=0; i<msk.size(); i++ )
	{
		msk_id[i+1] = msk[i] + msk_id[i];
		if(msk[i]==0)
		{
			cnt++;
			obj.erase( obj.begin() + msk_id[i] );
		}
	}

	return cnt;
}
 
inline void partImageWrite(char* path, Mat image, Rect rect)
{
	if(rect.x<0) rect.x = 0;

	if(rect.y<0) rect.y = 0;

	if(rect.width <0) return;

	if(rect.height<0) return;

	if(rect.x + rect.width > image.cols) rect.width = image.cols - rect.x;

	if(rect.y + rect.height > image.rows) rect.height = image.rows - rect.y;

	imwrite(path,image(rect));
}

inline bool imgeSkeleton(Mat img, Mat& skel)
{
	const int MAX_ITERATION = 10000;
	threshold(img, img, 127, 255, cv::THRESH_BINARY); 
	skel = Mat::zeros(img.size(), CV_8UC1);
	//cv::Mat skel(img.size(), CV_8UC1, cv::Scalar(0));
	Mat temp;
	Mat eroded;
 
	Mat element = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));
 
	bool done;
	int iterations = 0;
	do
	{
	  erode(img, eroded, element);
	  dilate(eroded, temp, element); // temp = open(img)
	  subtract(img, temp, temp);
	  bitwise_or(skel, temp, skel);
	  eroded.copyTo(img);
		
	  iterations++;

	  if(iterations > MAX_ITERATION) return false;

	  done = (countNonZero(img) == 0);
	} while (!done);

	//skel = skel(Range(1,skel.rows-1),Range(1,skel.cols-1));

	return true;
}

inline void trim(string& str)
{
  string::size_type pos = str.find_last_not_of(' ');
  if(pos != string::npos) {
    str.erase(pos + 1);
    pos = str.find_first_not_of(' ');
    if(pos != string::npos) str.erase(0, pos);
  }
  else str.erase(str.begin(), str.end());
}

inline vector<string> split(string& str, string delim)
{
    string::size_type start = 0;
    string::size_type end = str.find(delim);
	vector<string> result;
    while (end != std::string::npos)
    {
        //std::cout << s.substr(start, end - start) << std::endl;
		result.push_back(str.substr(start,end-start));
		start = end + delim.length();
        end = str.find(delim, start);
    }

	result.push_back(str.substr(start,end));

	return result;
}

template<typename T>
T stringToNumber(const string& numberAsString)
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

inline void initConfigParam()
{
	MyProperties properties;
	if(properties.loadFrom("config.properties") < 0) return;

	GobalParam::FAST_CORNER_THRESH = properties.getProperty("FAST_CORNER_THRESH").getValueInteger();
	GobalParam::FAST_CORNER_THRESH_SUPRESS = properties.getProperty("FAST_CORNER_THRESH_SUPRESS").getValueBoolean();
	GobalParam::HESSIAN_THRESHOLD = properties.getProperty("HESSIAN_THRESHOLD").getValueInteger(); 
	GobalParam::OCTAVES = properties.getProperty("OCTAVES").getValueInteger(); 
	GobalParam::OCTAVELAYERS = properties.getProperty("OCTAVELAYERS").getValueInteger();  
	GobalParam::EXTENDED = properties.getProperty("OCTAVELAYERS").getValueBoolean();
	GobalParam::UPRIGHT = properties.getProperty("UPRIGHT").getValueBoolean();
	GobalParam::KERNELSIZE = properties.getProperty("KERNELSIZE").getValueInteger();
	GobalParam::LOWTHRESHOLD = properties.getProperty("LOWTHRESHOLD").getValueInteger();
	GobalParam::RATIO = properties.getProperty("RATIO").getValueInteger();
	GobalParam::DISTANCETHRESH = properties.getProperty("DISTANCETHRESH").getValueDouble();
	GobalParam::DETECTED_USE_CANNY = properties.getProperty("DETECTED_USE_CANNY").getValueBoolean();
	GobalParam::CANNY_MATACH_THRESHOLD = properties.getProperty("CANNY_MATACH_THRESHOLD").getValueDouble();
	GobalParam::DETECTED_KP_THRESHOLD = properties.getProperty("DETECTED_KP_THRESHOLD").getValueDouble();
	GobalParam::IS_DEBUG = properties.getProperty("IS_DEBUG").getValueBoolean();
	GobalParam::LINE_OP_SIZE = properties.getProperty("LINE_OP_SIZE").getValueInteger();
	GobalParam::MERGE_LINE_LIMIT = properties.getProperty("MERGE_LINE_LIMIT").getValueInteger();
	GobalParam::G_USING_MASK = properties.getProperty("G_USING_MASK").getValueBoolean();
}