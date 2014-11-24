#include "FrameProcesser.h"


FrameProcesser::FrameProcesser(int taskbar)
{
	init_surf_inst();
	outputFolder = "./";
	upper_taskbar = taskbar;
}

FrameProcesser::~FrameProcesser(void)
{
	if(!resultFile)
	{
		fclose(resultFile);
	}
}

int FrameProcesser::TASKBAR = 3;

void FrameProcesser::setOutputFolder(string outputFolder)
{
	this->outputFolder = outputFolder;

	
	resultFile = fopen((outputFolder + "result.txt").c_str(), "a+");
}

void FrameProcesser::init_surf_inst()
{
	surf_inst.hessianThreshold = GobalParam::HESSIAN_THRESHOLD;
	surf_inst.nOctaves         = GobalParam::OCTAVES;
	surf_inst.nOctaveLayers    = GobalParam::OCTAVELAYERS;
	surf_inst.extended         = GobalParam::EXTENDED;
	surf_inst.upright          = GobalParam::UPRIGHT;
}

bool FrameProcesser::feedFrame(string folder,string name,int seq_in)
{
	frame_seq = seq_in;	
	string filePath = folder + name + ".png"; 

	frame = imread(filePath);
	if(frame.empty())
	{
		return false;
	}

	frame_paint = frame.clone();
	cvtColor(frame, frame_gray, CV_BGR2GRAY); 

	Canny( frame_gray, frame_edge, GobalParam::LOWTHRESHOLD, GobalParam::LOWTHRESHOLD * GobalParam::RATIO, GobalParam::KERNELSIZE, false );

	FAST(frame_gray, frame_kps, GobalParam::FAST_CORNER_THRESH, GobalParam::FAST_CORNER_THRESH_SUPRESS ); //key point detector
	surf_inst(frame_gray, Mat(), frame_kps, frame_des, true);

	if(frame_des.empty())
	{
		return false;
	}

	return true;
}

void FrameProcesser::collectEdgeInfo()
{
	edgeInfo.setRectLineImages(frame_edge);
	edgeInfo.countLines();
	edgeInfo.mergeLines();
	edgeInfo.classifyLines();
	edgeInfo.adjustLines(edgeInfo.c_lines_h,10);
	edgeInfo.adjustLines(edgeInfo.c_lines_v,10);
	edgeInfo.convertToMap(edgeInfo.line_map_h,edgeInfo.c_lines_h);
	edgeInfo.convertToMap(edgeInfo.line_map_v,edgeInfo.c_lines_v);
	
	if(GobalParam::IS_DEBUG)
	{
		edgeInfo.printEdge(frame_paint, outputFolder, frame_seq);
	}
}

void FrameProcesser::matchTplCompsToFrame(vector<AppTpl>& tpls)
{
	for(int i=0; i<tpls.size(); i++ )
	{
		if(TASKBAR<=0 && upper_taskbar>0 && tpls[i].templateName == "TaskBar")
			continue;

		detectedItems.push_back(tpls[i].matchTplComp(frame_gray, frame_kps, frame_des,frame_edge));		
	}

	if(GobalParam::IS_DEBUG)
	{
		for(int i=0; i<detectedItems.size(); i++)
		{
			for(int j=0; j<detectedItems[i].size(); j++)
			{
				rectangle(frame_paint,detectedItems[i][j].rect,Scalar(0,0,255),2);
				stringstream ss;
				ss<<outputFolder << frame_seq<<"_visualcues.png";
				imwrite(ss.str(), frame_paint);
			}
		}
	}
}

int FrameProcesser::getWeightBetween(DetectedItem r1,DetectedItem r2, int maxHeight)
{
	vector<HVLine> c_lines_v = edgeInfo.c_lines_v;

	//const int MAX_GAP = 90;
	
	if(abs(r1.rect.y - r2.rect.y) > maxHeight)
	{
		return 0;
	}

	if(r1.row == r2.row && abs(r1.rect.y - r2.rect.y) > maxHeight/2)
	{
		return 0;
	}
	else if((r1.row - r2.row) * (r1.rect.y - r2.rect.y) < 0)
	{
		return 0;
	}

	int w1 = 0;
	int w2 = 0;
	for(int i=0; i<c_lines_v.size(); i++)
	{
		if((c_lines_v[i].level-r1.rect.x) * (c_lines_v[i].level-r2.rect.x) < 0)
		{
			if(r1.rect.y >= c_lines_v[i].start && r1.rect.y <= c_lines_v[i].end)
			{
				w1  = max(w1,c_lines_v[i].end - c_lines_v[i].start);
			}
			if(r2.rect.y >= c_lines_v[i].start && r2.rect.y <= c_lines_v[i].end)
			{
				w2 = max(w2,c_lines_v[i].end - c_lines_v[i].start);
			}
		}
	}

	int w;
	w = max(w1,w2);
	double ra = exp(0-(double)w/frame.rows);
	
	if(ra > 0.7) 
	{
		return 1;
	}
	else 
	{
		return 0;
	}
}

int FrameProcesser::distanceBetweenRow(Mat row1, Mat row2)
{
	if(row1.rows != 1 && row2.rows != 1) return 999;

	if(row1.cols != row2.cols) return 999;

	int dist = 0;
	for(int i=0; i<row1.cols; i++)
	{
		int tmp1 = row1.at<ushort>(0,i);
		int tmp2 = row2.at<ushort>(0,i);
		if(row1.at<ushort>(0,i) != row2.at<ushort>(0,i))
		{
			dist++;
		}
	}

	return dist;
}

void FrameProcesser::groupInstance(vector<AppTpl>& tpls)
{
	for(int i=0; i<tpls.size(); i++)
	{
		if((TASKBAR<=0 && tpls[i].templateName == "TaskBar") || 
			detectedItems[i].size() < 3) 
			continue;

		vector<DetectedItem> menus;
		vector<DetectedItem> others;
		for(int j=0; j<detectedItems[i].size(); j++)
		{
			if(detectedItems[i][j].row < tpls[i].contentRow || tpls[i].contentRow == -1)
			{
				menus.push_back(detectedItems[i][j]);
			}
			else
			{
				others.push_back(detectedItems[i][j]);
			}
		}

		if(menus.size() <= 0) continue;

		Mat W = Mat::zeros(menus.size(),menus.size(),CV_16U);
		for(int j=0; j<menus.size(); j++)
		{
			W.at<ushort>(j,j) = 1;
			for(int k=j+1; k<menus.size();k++)
			{
				W.at<ushort>(j,k) = getWeightBetween(menus[j],menus[k],tpls[i].maxMenuHeight);
				W.at<ushort>(k,j) = W.at<ushort>(j,k);
			}
		}

		for(int j=0; j<menus.size(); j++)
		{
			for(int k=0; k<menus.size(); k++)
			{
				cout<<W.at<ushort>(j,k)<<' ';
			}
			cout<<endl;
		}

		int m=0;
		vector<vector<int>> groups;
		vector<int> hasInGroup;
		while(m<menus.size())
		{
			vector<int> g;
			g.push_back(m);
			hasInGroup.push_back(m);
			int next = -1;
			for(int n=m+1; n<menus.size(); n++)
			{
				if(find(hasInGroup.begin(),hasInGroup.end(),n) == hasInGroup.end()
					&& distanceBetweenRow(W.row(m),W.row(n)) <= 1)
				{
					g.push_back(n);
					hasInGroup.push_back(n);
				}
				else
				{
					if(next < 0
						&& find(hasInGroup.begin(),hasInGroup.end(),n) == hasInGroup.end())
					{
						next = n;
					}
				}
			}
				
			groups.push_back(g);

			if(next < 0) break;
			m = next;			
		}

 		for(int j=0; j<groups.size(); j++)
		{
			vector<DetectedItem> group;
			for(int k=0; k<groups[j].size();k++)
			{
				group.push_back(menus[groups[j][k]]);
			}

			if(group.size()>3 || (tpls[i].templateName == "TaskBar" && group.size()>=3))
			{
				//for(int k=0; k<group.size(); k++)
				//{
				//	rectangle(frame_paint,group[k].rect,Scalar(0,255,0));
				//}

				AppInstance inst;
				inst.name = tpls[i].templateName;
				inst.menus = group;
				inst.others = others;
				inst.contentRow = tpls[i].contentRow;
				inst.urlRow = tpls[i].urlRow;

				instances.push_back(inst);
			}
		}	
	}
}

void FrameProcesser::getBoundary()
{
	for(int i=0; i<instances.size(); i++)
	{
		instances[i].getBoundary(edgeInfo);
		if(instances[i].name == "TaskBar" && TASKBAR>0)
		{
			upper_taskbar = instances[i].border.y;
		}
		
	}
}

void writeImage(Mat img,string outputFolder,string name,int& count, bool overwrite=false)
{	
	string fileName = "";
	if(count == 1)
	{
		fileName = outputFolder + name + ".png";
	}
	else
	{
		stringstream ss;
		ss<<outputFolder<<name<<"_"<<count<<".png";
		ss>>fileName;
		ss.clear();
	}

	if(_access((const char*)fileName.c_str(),0) == -1 || overwrite)
	{
		//LogUtil::info("write image: " + fileName);
		imwrite(fileName,img);
	}
	else
	{
		writeImage(img,outputFolder,name,++count);
	}
}

bool isContain(Rect outRect, Rect inRect) // is r1 contain r2
{
	if(outRect.contains(inRect.tl())&&
		outRect.contains(inRect.br()-Point(1,1)))
	{
		return true;
	}
	return false;
}

void FrameProcesser::crop()
{
	for(int i=0; i<instances.size(); i++)
	{
		if(instances[i].name == "TaskBar") continue;

		crop(instances[i]);
	}
}

void FrameProcesser::crop(AppInstance& inst)
{
	fprintf(resultFile,"%d %s\n",frame_seq,inst.name.c_str()); fflush(resultFile);

	if(!inst.isBorderValid())
	{
		LogUtil::info("application " + inst.name + "'s border can't be detected.");
		return;
	}

	if(inst.border.y + inst.border.height > upper_taskbar && upper_taskbar>0)
	{
		inst.border.height = upper_taskbar - inst.border.y;
	}

	Rect content = inst.border;
	content.y = inst.contentLine;
	content.height = content.height - (inst.contentLine - inst.border.y);

	//line(frame_paint,Point(inst.border.x,inst.contentLine-2),Point(inst.border.x+inst.border.width,inst.contentLine-2),Scalar(0,255,0),2);
	rectangle(frame_paint,inst.border,Scalar(97,187,157),3);
	rectangle(frame_paint,Rect(content.x+2, content.y, content.width-2, content.height-2),Scalar(86,157,245),2);	
		
	stringstream ss;
	ss<<frame_seq<<"_CONTENT";
	string fileName;
	ss>>fileName;

	int count = 1;
	writeImage(frame(content),outputFolder,fileName,count);

	if(inst.urlRow > 0)
	{
		cropChrome(inst,count);
	}
	else if(inst.name == "Eclipse")
	{
		cropEclipse(inst);
	}
	else
	{
		cout<<inst.name<<" isn't supported to crop for more detail"<<endl;	
	}
}

void FrameProcesser::cropChrome(AppInstance& inst, int count)
{
	const int SEARCH_DIST = 20;

	vector<DetectedItem> urlItems = inst.getDetectedItemsByRow(inst.urlRow);
	Rect urlRect = inst.getTightestRect(urlItems);

	int start = inst.border.x - 5;
	int end = inst.border.x + inst.border.width + 5;
	HVLine line1 = edgeInfo.getMaxLenLine(urlRect.y, 10,start,end, true);
	HVLine line2 = edgeInfo.getMaxLenLine(urlRect.y + urlRect.height, 10,start,end, true);

	if(line1.level<0 || line2.level<0) return;

	Rect rect;
	rect.x = max(line1.start,line2.start);
	rect.y = line1.level;
	rect.width = min(line1.end-line1.start,line2.end-line2.start);
	rect.height = line2.level - line1.level;

	if(rect.x >= 0 && rect.y >= 0 && rect.x + rect.width - 1 < frame.cols && rect.y + rect.height - 1 <= frame.rows)
	{
		stringstream ss;
		ss<<frame_seq<<"_URL";
		string fileName;
		ss>>fileName;
		writeImage(frame(rect),outputFolder,fileName,count, true);
		rectangle(frame_paint,rect,Scalar(125,71,31),2);	
	}

	vector<DetectedItem> contentItems = inst.getDetectedItemsByRow(inst.contentRow);
	if(contentItems.size() > 0)
	{
		Rect tempRect = contentItems[0].rect;
		if(isContain(inst.border,tempRect))
		{
			line1 = edgeInfo.getMaxLenLine(tempRect.y, 20,start,end, true);
			line2 = edgeInfo.getMaxLenLine(tempRect.y + urlRect.height, 20,start,end, true);

			rect.x = max(line1.start,line2.start);
			rect.y = line1.level;
			rect.width = min(line1.end-line1.start,line2.end-line2.start);
			rect.height = line2.level - line1.level;

			if(rect.x >= 0 && rect.y >= 0 && rect.x + rect.width - 1 < frame.cols && rect.y + rect.height - 1 <= frame.rows)
			{
				stringstream ss;
				ss<<frame_seq<<"_SEARCH";
				string fileName;
				ss>>fileName;
				writeImage(frame(rect),outputFolder,fileName,count, true);
				rectangle(frame_paint,rect,Scalar(125,71,31),2);	
			}
		}
	}

}

bool listContain(vector<string> list, string name)
{
	for(int i=0; i<list.size(); i++)
	{
		int index = name.find(list[i]);
		if(index >= 0)
		{
			return true;
		}
	}
	return false;
}

int getTabIconLevel(vector<HVLine> c_lines_h,Rect itemRect)
{
	for(int i=0 ;i<c_lines_h.size(); i++)
	{
		if((itemRect.y - c_lines_h[i].level) < 20 && 
			(c_lines_h[i].end - c_lines_h[i].start) > 100 &&
			c_lines_h[i].start < itemRect.x &&
			c_lines_h[i].end > itemRect.x + itemRect.width)
		{
			return c_lines_h[i].level;
		}
	}

	return -1;
}

bool isLineInRect(Rect rect, HVLine line, bool horizontal=true)
{
	Point p1,p2;
	p1 = horizontal ? Point(line.start + 5,line.level) : Point(line.level,line.start + 5);
	p2 = horizontal ? Point(line.end - 5,line.level) : Point(line.level,line.end - 5);
	
	return rect.contains(p1) && rect.contains(p2);
}

vector<HVLine> getLinesInRect(Rect rect, vector<HVLine> lines, bool horizontal=true)
{
	vector<HVLine> temp;
	for(int i=0; i<lines.size(); i++)
	{
		if(isLineInRect(rect,lines[i],horizontal) &&
			lines[i].end - lines[i].start > 100)
		{
			temp.push_back(lines[i]);
		}
	}
	return temp;
}

bool isInVector(vector<string> v, string s)
{
	for(int i=0; i<v.size(); i++)
	{
		int index = s.find(v[i]);
		if(index >= 0)
		{
			return true;
		}
	}
	return false;
}

void FrameProcesser::cropEclipse(AppInstance& inst)
{
	vector<DetectedItem> eitems = inst.getDetectedItemsByRow(inst.contentRow);
	
	//get the vertical structure of eclipse
	Rect content = inst.border;
	content.y = inst.contentLine;
	content.height = content.height - (inst.contentLine - inst.border.y);
	
	vector<HVLine> e_lines_h = getLinesInRect(content,edgeInfo.c_lines_h,true);
	vector<HVLine> e_lines_v = getLinesInRect(content,edgeInfo.c_lines_v,false);
	e_lines_h.push_back(HVLine(content.y, content.x, content.x+content.width));
	e_lines_h.push_back(HVLine(content.y+content.height, content.x, content.x+content.width));
	e_lines_v.push_back(HVLine(content.x, content.y, content.y+content.height));
	e_lines_v.push_back(HVLine(content.x+content.width-1, content.y, content.y+content.height));

	std::sort(e_lines_h.begin(),e_lines_h.end(),line_sort());
	std::sort(e_lines_v.begin(),e_lines_v.end(),line_sort());

	vector<HVLine> e_lines_v2,e_lines_h2;
	edgeInfo.classifyLines(e_lines_h2,e_lines_h,40);
	edgeInfo.classifyLines(e_lines_v2,e_lines_v,80);
	edgeInfo.adjustLines(e_lines_h2,10);
	edgeInfo.adjustLines(e_lines_v2,10);
	
	vector<HVLine> e_lines_h3;
	for(int i=0; i<e_lines_h2.size(); i++)
	{
		for(int j=0; j<eitems.size(); j++)
		{
			Rect rect = eitems[j].rect;
			if(rect.y - e_lines_h2[i].level < 40 && rect.y - e_lines_h2[i].level >0 &&
				rect.x >= e_lines_h2[i].start && rect.x < e_lines_h2[i].end)
			{
				if(find_if(e_lines_h3.begin(),e_lines_h3.end(),
					std::bind2nd(line_equal(),e_lines_h2[i])) == e_lines_h3.end())
				{
					e_lines_h3.push_back(e_lines_h2[i]);	
				}
			}
		}
	}
	e_lines_h3.push_back(HVLine(content.y+content.height, content.x, content.x+content.width));
	
	std::sort(e_lines_h3.begin(),e_lines_h3.end(),line_sort());
	edgeInfo.adjustLines(e_lines_h3,10);

	Mat m1 = edgeInfo.getEdgeMap(e_lines_h3,1);
	Mat m2 = edgeInfo.getEdgeMap(e_lines_v2,0);
	imwrite("eclipse_s.png",m1+m2);
	//waitKey(0);

	edgeInfo.cornerDetect(e_lines_h3,e_lines_v2,60);
	vector<Rect> rects = edgeInfo.divideIntoRect(e_lines_h3,e_lines_v2);
	
	int sub = 0;
	vector<string> list;
	list.push_back("package"); list.push_back("outline"); 
	list.push_back("task");    list.push_back("resource"); 
	for(int i=0; i<rects.size(); i++)
	{
		//rectangle(frame_paint,rects[i],Scalar(0,0,255),1);

		bool isCut = true;
		for(int j=0; j<eitems.size(); j++)
		{
			if(isContain(rects[i],eitems[j].rect))
			{
				
				if(isInVector(list,eitems[j].itemName))
				{
					isCut = false;
					break;
				}
			}
		}

		if(!isCut) continue;

		stringstream ss;
		ss<<frame_seq<<"_SUB_"<<++sub;
		string fileName;
		ss>>fileName;
		int count=1;
		writeImage(frame(rects[i]),outputFolder,fileName,count, true);

		rectangle(frame_paint,Rect(rects[i].x+2,rects[i].y+2,rects[i].width-2,rects[i].height-2),Scalar(97,187,157),2);
	}

	for(int i=0; i<eitems.size(); i++)
	{
		if(eitems[i].itemName == "google_search")
		{
			Rect r = eitems[i].rect;
			HVLine h_line, v_line;
			for(int j=0; j<edgeInfo.c_lines_h.size(); j++)
			{
				HVLine temp = edgeInfo.c_lines_h[j];
				if(temp.level <= r.y && r.y - temp.level < 50 && r.x + r.width < temp.end )
				{
					h_line = temp;
				}
			}

			for(int j=0; j<edgeInfo.c_lines_v.size(); j++)
			{
				v_line = edgeInfo.c_lines_v[j];
				if(v_line.level > r.x && v_line.start <= r.y && v_line.end > r.y)
				{
					break;
				}
			}

			if(h_line.level < 0 || v_line.level < 0) continue;

			stringstream ss;
			ss<<frame_seq<<"_SEARCH";;
			string fileName;
			ss>>fileName;
			int count=1;
			Rect temp;
			temp.x = max(h_line.start, r.x+r.width);
			temp.y = h_line.level;
			temp.width = h_line.end - temp.x;
			temp.height = v_line.end - v_line.start;

			if(temp.x+temp.width>frame.cols || temp.y + temp.height > frame.rows) break;

			writeImage(frame(temp),outputFolder,fileName,count, false);
		}
	}
}

void FrameProcesser::run(vector<AppTpl>& tpls)
{
	ostringstream ss;
	ss<<"Processing a Frame"<<endl; LogUtil::info(ss);
	FILE* ftime =  fopen("performance.txt","a");
	fprintf(ftime, "Frame: %d\n", this->frame_seq);
	time_t stime, etime, temp;
	time(&stime);
	temp = stime;

	//Collect Edge info
	collectEdgeInfo();
	
	time(&etime); 
	int interval = etime-stime;
	ss<<interval<<endl; LogUtil::info(ss);
	fprintf(ftime, "%d\n", interval);
	stime = etime;

	//template matching
	matchTplCompsToFrame(tpls);
	
	time(&etime); 
	interval = etime-stime;
	ss<<interval<<endl; LogUtil::info(ss);
	fprintf(ftime, "%d\n",interval);
	stime = etime;

	//group 
	groupInstance(tpls);
	
	time(&etime); 
	 interval = etime-stime;
	ss<<interval<<endl; LogUtil::info(ss);
	fprintf(ftime, "%d\n",interval);
	stime = etime;

	//boundary detection
	getBoundary();
	
	time(&etime); 
	interval = etime-stime;
	ss<<interval<<endl; LogUtil::info(ss);
	fprintf(ftime, "%d\n", interval);
	stime = etime;

	//extract app data
	crop();

	time(&etime);
	interval = etime-stime;
	ss<<interval<<endl; LogUtil::info(ss);
	fprintf(ftime, "%d\n", interval);
	stime = etime;

	interval = etime-temp;
	fprintf(ftime, "%d\n", interval);

	fclose(ftime);
	imwrite("frame.png",frame_paint);
}

void FrameProcesser::test()
{ 
	FrameProcesser processer;
	string folder = "D:/temp/";
	string name = "img-00006";

	processer.setOutputFolder("../CropOut/test/");
	vector<AppTpl> tpls;
	AppTpl::loadTemplates(tpls,"standard");

	processer.feedFrame(folder,name,6);

	processer.run(tpls);

	imshow("frame",processer.frame_paint);
	waitKey(0);
}