#pragma once

#include "opencv2/core/core.hpp"
#include <opencv2/contrib/contrib.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv/cv.h"

#include <string.h>

#include "MyDefines.h"
#include "WinTpl.h"

class WinObj
{
public:
	string name;
	int row_num;
	int content_row;
	int url_row;
	
	Rect border;
	int menu_line;

	vector<Rect> rowRect;
	Rect tightest;
	Rect menu_tightest;
	vector<RectDetect> compList;
	vector<vector<RectDetect>> rowCompList;

public:
	WinObj(){};
	~WinObj() {rowRect.clear(); compList.clear(); rowCompList.clear();}

	void findTightestRect();
	vector<RectDetect> getCompListByRow(int row);
	vector<RectDetect> getCompListByRow2(int row);
};