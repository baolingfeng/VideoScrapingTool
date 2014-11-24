#include "AppInstance.h"


AppInstance::AppInstance(void)
{
}


AppInstance::~AppInstance(void)
{
}

bool AppInstance::isBorderValid()
{
	return border.x>=0 && border.y>=0 && border.width>0 && border.height>0;
}

Rect AppInstance::getTightestRect(vector<Rect> rects)
{
	int tempx,tempy,tempw,temph;
	int x1,x2,y1,y2;
	bool first = true;
	for(int i=0; i<rects.size(); i++)
	{
		tempx = rects[i].x;
		tempy = rects[i].y;
		tempw = rects[i].width;
		temph = rects[i].height;

		if(first)
		{
			x1 = tempx;
			x2 = tempx + tempw -1;
			y1 = tempy;
			y2 = tempy + temph -1;
			first = false;
		}
		else
		{
			if(tempx < x1)
			{
				x1 = tempx;
			}
			if(tempx+tempw > x2)
			{
				x2 = tempx+tempw-1;
			}
			if(tempy < y1)
			{
				y1 = tempy;
			}
			if(tempy+temph > y2)
			{
				y2 = tempy+temph-1;
			}
		}
	}
	if(first) return Rect(0,0,0,0);

	return Rect(x1,y1,x2-x1+1, y2-y1+1);
}

Rect AppInstance::getTightestRect(vector<DetectedItem> detects)
{
	vector<Rect> temp;

	int tempx,tempy,tempw,temph;
	int x1,x2,y1,y2;
	bool first = true;
	for(int i=0; i<detects.size(); i++)
	{
		temp.push_back(detects[i].rect);
	}

	return getTightestRect(temp);
}

void AppInstance::getBoundary(EdgeInfo edgeInfo)
{
	const int MAXGAP = 50;
	int temp1, temp2;

	Rect menuRect = getTightestRect(menus);

	vector<HVLine> c_lines_h = edgeInfo.c_lines_h;
	vector<HVLine> c_lines_v = edgeInfo.c_lines_v;

	int preLevel = -1;
	int upper = -1;
	int upperLeft, upperRight;
	upperLeft = upperRight = -1; 
	int maxLen = 0;
	for(int i=0; i<c_lines_h.size(); i++)
	{
		if(preLevel == c_lines_h[i].level) continue;

		if(c_lines_h[i].level+MAXGAP < menuRect.y || c_lines_h[i].level > menuRect.y) continue;

		int len = edgeInfo.getHLineLengthBetween(c_lines_h[i].level, 
			menuRect.x, menuRect.x+menuRect.width-1, temp1,temp2);

		double ratio = len * 1.0 / menuRect.width;
		if(ratio > 0.5 && maxLen < len)
		{
			upper = c_lines_h[i].level;
			upperLeft = temp1;
			upperRight = temp2;

			maxLen = len;
		}
	}
	upper = upper<0 ? menuRect.y : upper;

	preLevel = -1;
	int menuLine = -1;
	int menuLeft, menuRight;
	menuLeft = menuRight = -1; 
	for(int i=0; i<c_lines_h.size(); i++)
	{
		if(preLevel == c_lines_h[i].level) continue;

		if(c_lines_h[i].level-MAXGAP >= menuRect.y+menuRect.height-1 || 
			c_lines_h[i].level < menuRect.y+menuRect.height-1) 
			continue;

		int len = edgeInfo.getHLineLengthBetween(c_lines_h[i].level, 
			menuRect.x, menuRect.x+menuRect.width-1, temp1,temp2);

		double ratio = len * 1.0 / menuRect.width;
		if(ratio > 0.5)
		{
			menuLine = c_lines_h[i].level;
			menuLeft = temp1;
			menuRight =temp2;
			break;
		}
	}
	menuLine = menuLine<0 ? menuRect.y+menuRect.height-1 : menuLine;
		
	int uLen = upperRight - upperLeft;
	int mLen = menuRight - menuLeft;

	double ra = mLen*1.0/ uLen;
	int left, right;
	if(ra<0.5)
	{
		left = max(upperLeft,menuLeft);
		right = min(upperRight,menuRight);
	}
	else
	{
		left = min(upperLeft,menuLeft);
		right = max(upperRight,menuRight);
	}
	left = min(left,menuRect.x);
	right = max(right,menuRect.x+menuRect.width-1);

	contentLine = menuLine;

	if(name == "TaskBar")
	{
		border.x = 0;
		border.y = upper;
		border.width = right - left;
		border.height = menuLine - upper;
		return;
	}

	preLevel = -1;
	int len = 0;
	int leftLevel = -1;
	int leftUpper, leftBottom;
	leftUpper = leftBottom = -1;
	for(int i=0; i<c_lines_v.size(); i++)
	{
		if(preLevel == c_lines_v[i].level) continue;

		if(abs(left - c_lines_v[i].level) > 20) continue;

		int tmp = edgeInfo.getVLineLengthBelow(c_lines_v[i].level,upper,temp1,temp2);
		if(len < tmp)
		{
			len = tmp;
			leftLevel = c_lines_v[i].level;
			leftUpper = temp1;
			leftBottom = temp2;
		}

		preLevel = c_lines_v[i].level;
	}

	preLevel = -1;
	len = 0;
	int rightLevel = -1;
	int rightUpper,rightBottom;
	rightUpper = rightBottom = -1;
	for(int i=0; i<c_lines_v.size(); i++)
	{
		if(preLevel == c_lines_v[i].level) continue;

		if(abs(right - c_lines_v[i].level) > 20) continue;

		int tmp = edgeInfo.getVLineLengthBelow(c_lines_v[i].level,upper,temp1,temp2);
		if(len < tmp)
		{
			len = tmp;
			rightLevel = c_lines_v[i].level;
			rightUpper = temp1;
			rightBottom = temp2;
		}
	}

	int bottom = max(leftBottom,rightBottom);

	border.x = left;
	border.y = upper;
	border.width = right - left;
	border.height = bottom - upper;
}

vector<DetectedItem> AppInstance::getDetectedItemsByRow(int row)
{
	vector<DetectedItem> temp;

	if(row < contentRow)
	{
		for(int i=0; i<menus.size(); i++)
		{
			if(row == menus[i].row)
			{
				temp.push_back(menus[i]);
			}
		}
	}
	else
	{
		for(int i=0; i<others.size(); i++)
		{
			if(row == others[i].row)
			{
				temp.push_back(others[i]);
			}
		}
	}

	return temp;

}

