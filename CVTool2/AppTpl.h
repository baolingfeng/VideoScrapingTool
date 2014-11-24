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
#include <iostream>
#include <fstream>

#include "MyDefines.h"
#include "MyFunctions.h"
#include "AppItem.h"
#include "LogUtil.h"

using namespace std;
using namespace cv;

class DetectedItem
{
public:
	string itemName;
	int row;
	Rect rect;

};

class AppTpl
{
public:
	AppTpl(void);
	~AppTpl(void);

	void calcFrameDFTs(Mat& frame_gray_in, Mat& F_I, Mat& F_I2);
	vector<DetectedItem> matchTplComp(Mat& tar_img_gray,vector<KeyPoint>& tar_kps, Mat& tar_des,Mat& tar_edge);
	bool loadTemplate(string path, string name);

	void setAlphaTplMatchingParameters(Size frame_sz, double dft_thr_com_in=0.99);

	//WinTplComp getWinTplComp(int row_id, int item_id);

	//static void loadTemplate(vector<AppTpl>& templates,char* username);

	

public:
	string templateName;
	int rowNum;
	int contentRow;
	int urlRow;  //if app is a browser
	int maxMenuHeight;
	bool firstDetect;
	vector<string> firstDetectItems;

	vector<vector<AppItem>> items;

	SURF			surf_inst;
	
	Size			dft_tpl_sz_com;
	Size			dft_sz_com;//common template size for dft operation among all templates using alpha template matching method
	double			dft_thr_com;

	int MAX_MENU_HEIGHT;
public:
	static string BASE_FOLDER;

	static void loadTemplates(vector<AppTpl>& tpls, string username);
	static void loadTemplatesFromFS(vector<AppTpl>& tpls, string path);
	static void test();
};

