#pragma once
#include "opencv2/core/core.hpp"
#include <opencv2/contrib/contrib.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv/cv.h"

#include "MyDefines.h"
#include "EdgeInfo.h"
#include "AppTpl.h"
#include "AppInstance.h"
#include "LogUtil.h"
#include <functional>

using namespace std;
using namespace cv;

class FrameProcesser
{
public:
	FrameProcesser(int taskbar=-1);
	~FrameProcesser(void);

	void setOutputFolder(string outputFolder);

	void init_surf_inst();
	bool feedFrame(string folder, string name, int seq=1);
	void collectEdgeInfo();
	void matchTplCompsToFrame(vector<AppTpl>& tpls);

	int distanceBetweenRow(Mat row1, Mat row2);
	int getWeightBetween(DetectedItem r1,DetectedItem r2,int maxHeight);
	void groupInstance(vector<AppTpl>& tpls);

	void getBoundary();
	void crop();
	void crop(AppInstance& inst);
	void cropChrome(AppInstance& inst, int count=1);
	void cropEclipse(AppInstance& inst);

	void run(vector<AppTpl>& tpls);

	static void test();
public:
	string outputFolder;
	FILE* resultFile;

	int frame_seq;
	Mat frame;
	Mat frame_gray;
	Mat frame_paint;
	Mat frame_edge;

	vector<KeyPoint> frame_kps;
	Mat				 frame_des;

	SURF surf_inst;

	EdgeInfo edgeInfo;
	vector<vector<DetectedItem>> detectedItems;

	vector<AppInstance> instances;

	int upper_taskbar;

	static int TASKBAR;
};

