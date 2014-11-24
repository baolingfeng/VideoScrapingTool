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
#include "WinTpl.h"
#include "WinObj.h"
#include "AppTpl.h"
#include "EdgeInfo.h"

using namespace std;
using namespace cv;

class ObjMan
{
private:
	char outputFolder[MAXSLEN];
	char tmpPath[MAXSLEN];

	FILE* fresult;
public:
	static MyConfig config;

	int frame_seq;
	Mat frame;
	Mat frame_gray;
	Mat frame_paint;

	vector<KeyPoint> frame_kps;
	Mat				 frame_des;

	SURF surf_inst;

	Mat img_edge;
	Mat img_lines_h;
	Mat img_lines_v;

	Mat w_lines_h;
	Mat w_lines_v;

	vector<vector<int>> lines_h;
	vector<vector<int>> lines_v;

	vector<HVLine> m_lines_h;
	vector<HVLine> m_lines_v;
	vector<HVLine> c_lines_h;
	vector<HVLine> c_lines_v;

	vector<Rect> rects;

	vector<vector<RectDetect>> detects;
	vector<WinTpl> templates;
	vector<WinObj> objs;

	vector<vector<DetectedItem>> detectedItems;
	//vector<AppTpl> tpls;

	int upper_taskbar;
public:
	ObjMan();
	~ObjMan();

	void init_surf_inst();
	void setOutputFolder(char* outputFolder_in);
	void setTemplates(vector<WinTpl> tpls);
	//void setAppTpls(vector<AppTpl> tpls);

	bool feedFrame(char* folder, char* name, int seq=1);

	void collectEdgeMapLines(int op_size);
	void setRectLineImages(int op_size = 15);
	void countLines();
	void mergeLines(int limit);
	void mergeLines(vector<HVLine>& m_lines, vector<vector<int>>& lines, int limit);
	void clusterLines();
	void clusterLines(vector<HVLine>& c_lines, vector<HVLine>& m_lines, int limit);
	void cornerDetect();
	void divideIntoRect();

	void matchTplCompsToFrame();
	void matchTplCompsToFrame(vector<AppTpl> tpls);

	int getWeightBetween(RectDetect r1,RectDetect r2);
	void groupWinObj();

	void getHorizontalBorderLine(Rect rect, int& upper, int& down);
	bool getBoundary(WinObj& obj);

	void getBrowserUrlRect(WinObj& obj);
	void test();

	void cropChrome(WinObj& obj);
	void cropEclipse(WinObj& obj);
	void cropTaskBar(WinObj& obj, int& taskbar_line);
	void cropWord(WinObj& obj);

	void clear();
};