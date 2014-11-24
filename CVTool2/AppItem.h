#pragma once
#include "opencv2/core/core.hpp"
#include <opencv2/contrib/contrib.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv/cv.h"

#include <hash_map>
#include <vector>
#include <io.h>

#include "MyDefines.h"
#include "MyFunctions.h"

using namespace std;
using namespace cv;

enum ITEM_TYPE {MENU=1,CONTENT=2,BOTTOMBAR=3};


class AppItem
{
public:
	AppItem(void);
	~AppItem(void);

	vector<Rect> match(Mat& tar_img_gray, Mat& F_I, Mat& F_I2);
	vector<Rect> match(Mat tar_img_gray, vector<KeyPoint> tar_kps, Mat tar_des, Mat tar_edge);
	vector<Rect> clusterDetects_hough_fixedscale(Mat tar_img_gray, vector<KeyPoint> tar_kps, Mat tar_des, Mat tar_edge, vector< DMatch > matches);
	vector<Rect> clusterDetects_hough(Mat tar_img_gray, vector<KeyPoint> tar_kps, Mat tar_des, vector<DMatch> matches);

	bool tightenMask();
	void initDFTParameters(Size dft_sz_in, Size dft_tpl_sz_in, double dft_thr_in);
	void getAlphaTplMatchingMap(Size tar_img_sz, Size com_tpl_sz, Size own_tpl_sz, Mat& map, Mat& F_a2, Mat& F_a2T, double aT, Mat& F_I, Mat& F_I2);

	bool cannyMatch(Mat tar_canny, Mat item_canny);
	bool isUsingMask();
public:
	string name;
	int row;
	//ITEM_TYPE type;

	Mat item;
	vector<KeyPoint> item_kps;
	Mat item_des;
	Mat item_edge;

	Rect item_tight;
	int matchMethod; //default: 0, keypoint; 1: mask
	Mat item_msk;

	Rect			 msk_rect_tight;//dft_item( Rect(0,0, msk_rect_tight.width, msk_rect_tight.height) ) = im2double( item(msk_rect_tight) ).
	Size			 dft_sz;//common operating size for dft operations
	Size			 dft_tpl_sz;//common template size for dft operation among all templates using alpha template matching method
	double			 dft_thr;
	Mat				 F_a2;
	Mat				 F_a2T;
	double			 norm_aT;
};

