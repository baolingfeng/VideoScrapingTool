#pragma once
#include<iostream>
#include<vector>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/contrib/contrib.hpp"
using namespace std;

class GUIComponent
{
public:
	GUIComponent(void);
	GUIComponent(string name, bool tobescraped=false) {this->name = name; this->tobescraped = tobescraped;}
	~GUIComponent(void);

public:
	string compType;
	string name;
	bool tobescraped;
	
	vector<GUIComponent*> components;
};

/*
class GUIWindow : public GUIComponent
{
public:
	GUIWindow();
	GUIWindow(string name, bool tobescraped=false) : GUIComponent(name,tobescraped) {}
	~GUIWindow();

public:
	vector<GUIComponent*> components;
};
*/

class GUIRow : public GUIComponent
{
public:
	GUIRow() { index=0;}
	GUIRow(string name, bool tobescraped=false, int index=0) : GUIComponent(name,tobescraped) { this->index = index;}
	~GUIRow() 
	{ 
		for(int i=0; i<this->components.size(); i++)
		{
			delete components[i];
		}
	}

public:
	int index;
};

class VisualCue
{
public:
	VisualCue() {}
	~VisualCue() {}
public:
	string name;
	bool isunique;
	vector<cv::Mat> icons;
};

class GUIItem : public GUIRow
{
public:
	GUIItem(){ index=0;}
	GUIItem(string name, bool tobescraped=false, int index=0) : GUIRow(name,tobescraped) { this->index = index;}
	~GUIItem(){}

public:
	vector<VisualCue> visualcues;
	int index;
};