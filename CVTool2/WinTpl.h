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

typedef struct RectDetect_
{
	int item_id;
	int row_id;
	Rect rect;
}RectDetect;

struct left_rect_detect
{
    inline bool operator() (const RectDetect& r1, const RectDetect& r2)
    {
		return (r1.rect.x < r2.rect.x);
    }
};

class WinTplComp
{
public:
	int item_id;
	int row_id;

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

	int is_unique;
public:
	WinTplComp();
	~WinTplComp();

	vector<Rect> match(Mat& tar_img_gray, Mat& F_I, Mat& F_I2);
	vector<Rect> match(Mat tar_img_gray, vector<KeyPoint> tar_kps, Mat tar_des, Mat tar_edge);
	vector<Rect> clusterDetects_hough_fixedscale(Mat tar_img_gray, vector<KeyPoint> tar_kps, Mat tar_des, Mat tar_edge, vector< DMatch > matches);
	vector<Rect> clusterDetects_hough(Mat tar_img_gray, vector<KeyPoint> tar_kps, Mat tar_des, vector<DMatch> matches);

	bool tightenMask();
	void initDFTParameters(Size dft_sz_in, Size dft_tpl_sz_in, double dft_thr_in);
	void getAlphaTplMatchingMap(Size tar_img_sz, Size com_tpl_sz, Size own_tpl_sz, Mat& map, Mat& F_a2, Mat& F_a2T, double aT, Mat& F_I, Mat& F_I2);

	bool cannyMatch(Mat tar_canny, Mat item_canny);
};

class WinTpl
{
public:
	vector<vector<WinTplComp>> items;
	int rowNum;
	string templateName;

	SURF			 surf_inst;
	int				 FastCornerThresh;
	bool			 FastCornerSupress;

	Size			 dft_tpl_sz_com;
	Size			 dft_sz_com;//common template size for dft operation among all templates using alpha template matching method
	double			 dft_thr_com;

	int browser_url_row;
	int content_row;
public:
	WinTpl(string name);
	~WinTpl();

	void calcFrameDFTs(Mat& frame_gray_in, Mat& F_I, Mat& F_I2);
	vector<RectDetect> matchTplComp(Mat& tar_img_gray,vector<KeyPoint>& tar_kps, Mat& tar_des,Mat& tar_edge);
	void loadTemplate(char* path, char* name);

	void setAlphaTplMatchingParameters(Size frame_sz, double dft_thr_com_in=0.99);

	WinTplComp getWinTplComp(int row_id, int item_id);

	static void loadTemplate(vector<WinTpl>& templates,char* username);
};