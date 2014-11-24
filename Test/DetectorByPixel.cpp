#pragma once
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

using namespace std;
using namespace cv;

Rect tightenMask(Mat item_msk)
{
	Rect msk_rect_tight;
	msk_rect_tight.x = item_msk.cols-1;
	msk_rect_tight.y = item_msk.rows-1;
	msk_rect_tight.width  = 0;
	msk_rect_tight.height = 0;

	int tmpx = 0; 
	int tmpy = 0;
	for(int i=0; i<item_msk.rows; i++)
	{
		uchar* msk_i = (uchar*)item_msk.ptr<uchar>(i);//uchar type
		for(int j=0; j<item_msk.cols; j++)
		{
			if( msk_i[j] != 0 )
			{
				if( msk_rect_tight.x > j ){ msk_rect_tight.x = j;}
				if( msk_rect_tight.y > i ){ msk_rect_tight.y = i;}
				if( tmpx    < j ){ tmpx    = j;}
				if( tmpy    < i ){ tmpy    = i;}
			}
		}
	}

	msk_rect_tight.width = tmpx - msk_rect_tight.x + 1;
	msk_rect_tight.height= tmpy - msk_rect_tight.y + 1;

	msk_rect_tight.width = msk_rect_tight.width>0 ? msk_rect_tight.width : 0;
	msk_rect_tight.height = msk_rect_tight.height>0 ? msk_rect_tight.height : 0;

	//item_tight = msk_rect_tight;
	return msk_rect_tight;
}

bool im2double2(Mat& img_u, Mat& img_lf)
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

void getAlphaTplMatchingMap(Size tar_img_sz, Size com_tpl_sz, Size own_tpl_sz, Mat& map, Mat& F_a2, Mat& F_a2T, double aT, Mat& F_I, Mat& F_I2)
{
	//Convolution
	Mat tmpMat;
	map.create(abs(tar_img_sz.height - own_tpl_sz.height)+1, abs(tar_img_sz.width - own_tpl_sz.width)+1, F_a2.type());

	mulSpectrums(F_a2T, F_I, tmpMat, 0, false);
	dft(tmpMat, tmpMat, DFT_INVERSE+DFT_SCALE, tar_img_sz.height+com_tpl_sz.height-own_tpl_sz.height);//
	tmpMat(Rect(com_tpl_sz.width-1, com_tpl_sz.height-1, map.cols, map.rows)).copyTo(map);
	

	//Calculate the normalization of the cost
	Mat M_norm_a2I2(map.rows, map.cols, F_a2.type());

	mulSpectrums(F_a2, F_I2, tmpMat, 0, false);
	dft(tmpMat, tmpMat, DFT_INVERSE+DFT_SCALE, tar_img_sz.height+com_tpl_sz.height-own_tpl_sz.height);//tar_img_sz.height
	tmpMat(Rect(com_tpl_sz.width-1, com_tpl_sz.height-1, map.cols, map.rows)).copyTo(M_norm_a2I2);

	sqrt(M_norm_a2I2, M_norm_a2I2);

	map = map/M_norm_a2I2 * (1/aT);
}


vector<Rect> match(Mat& tar_img_gray, Mat& F_I, Mat& F_I2, 
	Mat item,double dft_thr,Size dft_tpl_sz,Rect msk_rect_tight,Mat& F_a2,Mat& F_a2T,double norm_aT)
{
	//-->debug
	bool debuging = false;//true;//
	Mat map_show;
	Mat tpl_show;
	Mat box_show;
	//<--debug

	Mat rst;

	double* rst_i;
	int i,j;
	int a, b, a_opt, b_opt;
	double v, v_opt;
	double ep = 0.002;
	double current_thr = dft_thr;
	bool isPresent = false;

	double minVal, maxVal;
	Point minLoc, maxLoc;

	getAlphaTplMatchingMap(Size(tar_img_gray.cols,tar_img_gray.rows), dft_tpl_sz, Size(msk_rect_tight.width, msk_rect_tight.height), 
							rst, F_a2, F_a2T, norm_aT, F_I, F_I2);
	minMaxLoc( rst, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
	
	vector<Rect> detects;
	if( maxVal>=current_thr )
	{
		if( maxVal-ep>=current_thr )
			current_thr = maxVal-ep;
		isPresent = true;
		for( i=0; i<rst.rows; i++ )
		{
			rst_i = (double*)rst.ptr<double>(i);//double type
			for( j=0; j<rst.cols; j++ )
			{
				if( rst_i[j] >= current_thr )
				{
					v_opt = rst_i[j];
					a_opt = i;
					b_opt = j;
					for( a=i; a<i+msk_rect_tight.height && a<rst.rows; a++ )
					{
						for( b=j; b<j+msk_rect_tight.width && b<rst.cols; b++ )
						{
							v = rst.at<double>(a,b);
							if( v>v_opt)
							{
								rst.at<double>(a_opt, b_opt) = 0;
								a_opt = a;
								b_opt = b;
								v_opt = v;
							}else
							{
								rst.at<double>(a,b) = 0;
							}
						}
					}
					detects.push_back( Rect( b_opt-msk_rect_tight.x, a_opt-msk_rect_tight.y, item.cols, item.rows) );
				
				}
			}
		}
	}
	
	if( !isPresent )
	{
		printf("Alpha Tpl Matching: No match found.\n");
	}

	///reset used vector
	return detects;
}


int main1()
{
	//Mat img = imread("C:\\Users\\LingFeng\\Desktop\\hengtian\\datacollecttool\\log\\screen\\2014-08-22-13-14-22-480.png",0);
	//Mat item = imread("C:\\Users\\LingFeng\\Desktop\\hengtian\\bluemorph\\saveall.png",0);
	//Mat item_mask = imread("C:\\Users\\LingFeng\\Desktop\\hengtian\\bluemorph\\saveall_mask.png",0);

	Mat img = imread("D:\\experimental video\\20121019\\chenxiao\\chenxiao\\img-00259.png",0);
	Mat item = imread("C:\\Users\\LingFeng\\Documents\\Visual Studio 2010\\Projects\\CVTool2\\tpls\\standard\\Eclipse\\ROW_4\\jedit_warning.png",0);
	Mat item_mask = imread("C:\\Users\\LingFeng\\Documents\\Visual Studio 2010\\Projects\\CVTool2\\tpls\\standard\\Eclipse\\ROW_4\\jedit_warning_mask.png",0);

	Size frame_sz;
	frame_sz = img.size();

	Rect msk_rect_tight = tightenMask(item_mask);

	Size dft_tpl_sz_com;
	dft_tpl_sz_com.width = msk_rect_tight.width;
	dft_tpl_sz_com.height = msk_rect_tight.height;

	Size dft_sz_com;
	dft_sz_com.width  = getOptimalDFTSize(frame_sz.width  + dft_tpl_sz_com.width  - 1);
	dft_sz_com.height = getOptimalDFTSize(frame_sz.height + dft_tpl_sz_com.height - 1);

	Mat img_gray, img_fp;
	Mat msk_gray, msk_fp;
	Mat M_a2;
	Mat M_a2T;
	Mat M_aT;

	img_gray = Mat::zeros(dft_tpl_sz_com, item.type());
	Mat roiImg_gray( img_gray, Rect(0,0,msk_rect_tight.width, msk_rect_tight.height) );
	item( msk_rect_tight ).copyTo( roiImg_gray);
	
	//cvtColor(img_gray, img_gray, CV_BGR2GRAY);//Currently only deel with gray images

	msk_gray = Mat::zeros(dft_tpl_sz_com, item_mask.type());
	Mat roiMsk_gray( img_gray, Rect(0,0,msk_rect_tight.width, msk_rect_tight.height) );
	item_mask( msk_rect_tight ).copyTo( roiMsk_gray);//already gray img

	im2double2( img_gray, img_gray );
	im2double2( msk_gray, msk_gray );

	flip(img_gray, img_fp, -1);
	flip(msk_gray, msk_fp, -1);
		
	M_a2  = msk_fp.mul(msk_fp);
	M_a2T = M_a2.mul(img_fp);
	M_aT  = msk_fp.mul(img_fp);

	double norm_aT = norm(M_aT, NORM_L2);

	Mat F_a2 = Mat::zeros(dft_sz_com, CV_64FC1);
	Mat F_a2T= Mat::zeros(dft_sz_com, F_a2.type());
	Mat roiF_a2 (F_a2,  Rect(0,0,msk_fp.cols,msk_fp.rows));
	M_a2.copyTo(roiF_a2);
	Mat roiF_a2T(F_a2T, Rect(0,0,msk_fp.cols,msk_fp.rows));
	M_a2T.copyTo(roiF_a2T);

	dft(F_a2,  F_a2,  0, msk_fp.rows);
	dft(F_a2T, F_a2T, 0, msk_fp.rows);

	//img dft
	Mat frame_gray;
	Mat F_I,F_I2;
	im2double2(img, frame_gray);

	F_I  = Mat::zeros(dft_sz_com, frame_gray.type());
    F_I2 = Mat::zeros(dft_sz_com, frame_gray.type());

	Mat M_I2 = frame_gray.mul(frame_gray);
	Mat roiF_I (F_I,  Rect(0,0,frame_gray.cols,frame_gray.rows));
    frame_gray.copyTo(roiF_I);
    Mat roiF_I2(F_I2, Rect(0,0,frame_gray.cols,frame_gray.rows));
    M_I2.copyTo(roiF_I2);

	dft(F_I,  F_I,  0, frame_gray.rows);
    dft(F_I2, F_I2, 0, frame_gray.rows);

	match(img,F_I,F_I2,item,0.8,dft_tpl_sz_com,msk_rect_tight,F_a2,F_a2T,norm_aT);

	return 0;
}