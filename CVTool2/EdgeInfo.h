#pragma once

#include "opencv2/core/core.hpp"
#include <opencv2/contrib/contrib.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv/cv.h"

#include <set>
#include <hash_map>
#include <algorithm>
#include <iterator>
#include "MyDefines.h"

using namespace std;
using namespace cv;

struct Corner
{
	int value;
	int type;
};

class HVLine
{
public:
	HVLine() {level = -1; start=-1; end=-1;}
	HVLine(int level_in, int start_in, int end_in) {level = level_in; start = start_in; end = end_in;}

	int level; // horizontal or vertical line relative position
	int start; 
	int end;

	vector<Corner> corners;
};

struct line_equal : public std::binary_function<HVLine, HVLine, bool>
{
    bool operator () (const HVLine &l1, const HVLine &l2) const
    {
		return l1.level == l2.level && l1.start == l2.start && l1.end == l2.end;
    }
};

struct corner_sort
{
	inline bool operator() (const Corner& c1, const Corner& c2)
    {
		return c1.value < c2.value;
    }
};

struct line_sort
{
    inline bool operator() (const HVLine& r1, const HVLine& r2)
    {
		if(r1.level == r2.level)
		{
			return r1.start < r2.start;
		}
		else
		{
			return r1.level < r2.level;
		}
    }
};

class EdgeInfo
{
public:
	EdgeInfo(void);
	~EdgeInfo(void);

	void setRectLineImages(Mat frame);
	void countLines();
	void mergeLines();
	void mergeLines(vector<HVLine>& m_lines, vector<vector<int>>& lines);
	void clusterLines();
	void clusterLines(vector<HVLine>& c_lines, vector<HVLine>& m_lines, int limit);
	
	void classifyLines();
	void classifyLines(vector<HVLine>& c_lines, vector<HVLine>& m_lines, int limit);
	
	void adjustLines();
	void adjustLines(vector<HVLine>& c_lines, int limit=3);
	void convertToMap(hash_map<int,vector<HVLine>>& lines_map,vector<HVLine>& c_lines);

	vector<HVLine> getLinesByLevel(int level, bool horizontal=true);
	HVLine getMaxLenLine(int level, int gap, int start=-1, int end=-1, bool horizontal=true);

	void cornerDetect(vector<HVLine>& c_lines_h, vector<HVLine>& c_lines_v, int limit=20);
	vector<Rect> divideIntoRect(vector<HVLine>& c_lines_h, vector<HVLine>& c_lines_v);

	int getHLineLengthBetween(int level, int start, int end, int& left, int& right);
	int getVLineLengthBelow(int vlevel, int upper, int& start, int& end);

	Mat getEdgeMap(vector<HVLine>& lines,int h=1);
	void printEdge(Mat& frame, string outputFolder, int seq);
	static void test();
public:
	Mat img_lines_h;
	Mat img_lines_v;

	vector<vector<int>> lines_h;
	vector<vector<int>> lines_v;

	vector<HVLine> m_lines_h;
	vector<HVLine> m_lines_v;

	vector<HVLine> c_lines_h;
	vector<HVLine> c_lines_v;

	hash_map<int,vector<HVLine>> line_map_h;
	hash_map<int,vector<HVLine>> line_map_v;
private:
	int rows;
	int cols;
};

