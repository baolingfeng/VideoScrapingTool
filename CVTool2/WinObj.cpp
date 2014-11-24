#include "WinObj.h"

Rect getTightestRect(vector<Rect> listRect)
{
	int tempx,tempy,tempw,temph;
	int x1,x2,y1,y2;
	bool first = true;
	for(int i=0; i<listRect.size(); i++)
	{
		tempx = listRect[i].x;
		tempy = listRect[i].y;
		tempw = listRect[i].width;
		temph = listRect[i].height;

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

Rect getTightestRect(vector<RectDetect> listRect)
{
	vector<Rect> temp;

	int tempx,tempy,tempw,temph;
	int x1,x2,y1,y2;
	bool first = true;
	for(int i=0; i<listRect.size(); i++)
	{
		temp.push_back(listRect[i].rect);
	}

	return getTightestRect(temp);
}

void WinObj::findTightestRect()
{
	const int MAX_ROW_HEIGHT = 50;
	tightest = getTightestRect(compList);
	vector<RectDetect> menuRects;
	for(int i=1; i<=row_num; i++)
	{
		vector<RectDetect> tempList = getCompListByRow(i);
		Rect temp = getTightestRect(tempList);

		rowCompList.push_back(tempList);

		/*
		if(temp.height > MAX_ROW_HEIGHT && i == url_row)
		{
			vector<RectDetect> tempList2;
			for(int m=0; m<tempList.size(); m++)
			{
				int count = 0;
				for(int n=0; n<tempList.size(); n++)
				{
					if(m == n) continue;
					if(abs(tempList[m].rect.y - tempList[n].rect.y) > MAX_ROW_HEIGHT)
					{
						count++;
					}
				}
				if(count < tempList.size()/2) 
				{
					tempList2.push_back(tempList[m]);
				} 
			}
			rowCompList.push_back(tempList2);
			temp = getTightestRect(tempList2);
		}
		else
		{
			rowCompList.push_back(tempList);
		}
		*/

		if(i < content_row)
		{
			menuRects.insert(menuRects.end(),rowCompList[i-1].begin(),rowCompList[i-1].end());
		}

		temp.x = tightest.x;
		temp.width = tightest.width;
		rowRect.push_back(temp);
	}
	menu_tightest = getTightestRect(menuRects);
	/*
	if(menuRects.size() > 0)
	{
		menu_tightest = getTightestRect(menuRects);
		if(menu_tightest.height > 200)
		{
			vector<RectDetect> tempList2;
			for(int m=0; m<menuRects.size(); m++)
			{
				int count = 0;
				for(int n=0; n<menuRects.size(); n++)
				{
					if(m == n) continue;
					if(abs(menuRects[m].rect.y - menuRects[n].rect.y) > 200)
					{
						count++;
					}
				}
				if(count < menuRects.size()/2) 
				{
					tempList2.push_back(menuRects[m]);
				} 
			}
			menuRects = tempList2;
			menu_tightest = getTightestRect(tempList2);
		}
	}
	*/
}

vector<RectDetect> WinObj::getCompListByRow(int row)
{
	vector<RectDetect> rdList;
	for(int i=0; i<compList.size(); i++)
	{
		if(compList[i].row_id == row)
		{
			rdList.push_back(compList[i]);
		}
	}
	return rdList;
}

vector<RectDetect> WinObj::getCompListByRow2(int row)
{
	return rowCompList[row-1];
}