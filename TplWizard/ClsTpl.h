#pragma once
#include <stdio.h>
#include <vector>
#include "opencv2/core/core.hpp"
#include <opencv2/contrib/contrib.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv/cv.h"

using namespace std;

class ClsItem
{
public:
	ClsItem() {};
	~ClsItem() {};

	bool load(string folder, string name);
	bool write(string folder);

	inline bool isEmpty()
	{
		return img.empty();
	}

	inline bool hasMask() 
	{
		return !img_mask.empty();
	}

	string name;
	cv::Mat img;
	cv::Mat img_mask;
};

class ClsRow
{
public:
	ClsRow() {index = -1;}
	ClsRow(int row) {index = row;}

	~ClsRow() {}

	bool loadItems(string folder, int row);
	bool write(string folder);
	bool del(string folder);
	bool renameFolder(string folder,int newIndex);

	inline void addItem(ClsItem item)
	{
		items.push_back(item);
	}

public:
	int index;
	vector<ClsItem> items;
};

class ClsApp
{
public:
	ClsApp(void)
	{ 
		rowNum = 0;
		contentRow = -1;
		urlRow = -1;
	};

	ClsApp(string name_in, int contentRow_in=-1, int urlRow_in=-1) 
	{
		name = name_in;
		contentRow = contentRow_in;
		urlRow = urlRow_in;
	}
	~ClsApp(void) {};
	
	void initRows(int rowNum);

	bool loadRows(string folder, string name);
	bool write(string folder);
	bool writeConfig(string folder);
	bool addRow(string folder, int rowIndex);
	bool deleteARow(string folder,int rowIndex);

	inline void addRow(ClsRow row)
	{
		rows.push_back(row);
	}

public:
	string name;
	int rowNum;
	int contentRow;
	int urlRow;

	vector<ClsRow> rows;
};

class ClsTpl
{
public:
	ClsTpl(void) {};
	ClsTpl(string name) { this->name = name;}
	~ClsTpl(void) {};

	bool loadApp(string folder, string name);
	bool write(string folder);
	bool delTemplate(string folder);
public:
	string name;
	vector<ClsApp> apps;
};

