#pragma once
#include "opencv2/core/core.hpp"
#include <opencv2/contrib/contrib.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv/cv.h"

#include <io.h>
#include <sstream>
#include "MyDefines.h"
#include "AppItem.h"
#include "AppTpl.h"
#include "EdgeInfo.h"

using namespace std;
using namespace cv;

class AppInstance
{
public:
	AppInstance(void);
	~AppInstance(void);

	void getBoundary(EdgeInfo edgeInfo);

	Rect getTightestRect(vector<Rect> rects);
	Rect getTightestRect(vector<DetectedItem> detects);

	vector<DetectedItem> getDetectedItemsByRow(int row);
	bool isBorderValid();

public:
	string name;

	vector<DetectedItem> menus;
	vector<DetectedItem> others;

	Rect border;
	int contentLine;

	int contentRow;
	int urlRow;
};

