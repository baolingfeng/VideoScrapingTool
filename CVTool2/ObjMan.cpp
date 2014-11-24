#include"ObjMan.h"

ObjMan::ObjMan()
{
	init_surf_inst();
}

ObjMan::~ObjMan()
{
	if(!fresult)
		fclose(fresult);
}

MyConfig ObjMan::config = MyConfig();

void ObjMan::setOutputFolder(char* outputFolder_in)
{
	strcpy(outputFolder, outputFolder_in);

	char fileName[MAXSLEN];
	strcpy(fileName,outputFolder);
	strcat(fileName,"result.txt");

	fresult = fopen(fileName,"a+");
}

void ObjMan::init_surf_inst()
{
	surf_inst.hessianThreshold = config.HESSIAN_THRESHOLD;
	surf_inst.nOctaves         = config.OCTAVES;
	surf_inst.nOctaveLayers    = config.OCTAVELAYERS;
	surf_inst.extended         = config.EXTENDED;
	surf_inst.upright          = config.UPRIGHT;
}

void ObjMan::setTemplates(vector<WinTpl> tpls)
{
	templates = tpls;
}

bool ObjMan::feedFrame(char* folder,char* name,int seq_in)
{
	char filePath[MAXSLEN];

	frame_seq = seq_in;
	
	strcpy( filePath, folder);
	strcat( filePath, name );
	strcat( filePath, ".png" );

	frame = imread(filePath);
	if(frame.empty())
	{
		return false;
	}

	frame_paint = frame.clone();

	cvtColor(frame, frame_gray, CV_BGR2GRAY); 

	Canny( frame_gray, img_edge, config.LOWTHRESHOLD, config.LOWTHRESHOLD * config.RATIO, config.KERNELSIZE, false );

	FAST(frame_gray, frame_kps, config.FAST_CORNER_THRESH, config.FAST_CORNER_THRESH_SUPRESS ); //key point detector
	surf_inst(frame_gray, Mat(), frame_kps, frame_des, true);

	if(frame_des.empty())
	{
		return false;
	}

	return true;
}

void ObjMan::collectEdgeMapLines(int op_size)
{
	setRectLineImages(op_size);
	countLines();
	mergeLines(4);
	clusterLines();
	cornerDetect();
	divideIntoRect();
}

void printEdgeMap(vector<HVLine>& c_lines_h,vector<HVLine>& c_lines_v,Mat frame,char* name)
{
	Mat lineMap = Mat::zeros(frame.rows, frame.cols,CV_8U);
	for(int i=0; i<c_lines_h.size();i++)
	{
		line(lineMap,Point(c_lines_h[i].start,c_lines_h[i].level),Point(c_lines_h[i].end,c_lines_h[i].level),Scalar(255),1);
	}

	
	for(int i=0; i<c_lines_v.size();i++)
	{
		line(lineMap,Point(c_lines_v[i].level,c_lines_v[i].start),Point(c_lines_v[i].level,c_lines_v[i].end),Scalar(255),1);
	}
	imwrite(name,lineMap);
}

void ObjMan::setRectLineImages(int op_size)
{
	img_lines_h = frame_gray.clone();
	
	//Canny( img_lines_h, img_lines_h, lowThreshold, lowThreshold*ratio, kernel_size, false );
	
	img_lines_h = img_edge.clone();
	img_lines_v = img_lines_h.clone();

	Mat op_elem_h = Mat::ones(1, op_size*2+1, CV_8UC1 );
	Mat op_elem_v = Mat::ones(op_size*2+1, 1, CV_8UC1 );

	erode( img_lines_h, img_lines_h, op_elem_h );
	dilate(img_lines_h, img_lines_h, op_elem_h );
	
	erode( img_lines_v, img_lines_v, op_elem_v );
	dilate(img_lines_v, img_lines_v, op_elem_v );

	img_lines_h(Rect(0,0,img_lines_h.cols,1)) = 255;
	img_lines_h(Rect(0,img_lines_h.rows-1,img_lines_h.cols,1)) = 255;
	img_lines_v(Rect(0,0,1,img_lines_v.rows)) = 255;
	img_lines_v(Rect(img_lines_v.cols-1,0,1,img_lines_v.rows)) = 255;

	if(DEBUGGING)
	{
		imwrite("frame_origin_edges.png",img_lines_h+img_lines_v);
		imwrite("frame_canny.png",img_edge);
	}
}

void ObjMan::countLines()
{
	int i, j;
	int tot_h, tot_v;
	int cnt_h, cnt_v;
	int ptFlag = 0;

	lines_h.clear();
	lines_h.resize(frame.rows);
	lines_h[0].push_back(0);
	lines_h[0].push_back(frame.cols);

	ptFlag = 0;
	tot_h = 2;
	for( i=1; i<frame.rows-1; i++ )
	{//start from second row to the second last row
		cnt_h = 0;
		for( j=0; j< frame.cols; j++ )
		{
			if((int)(img_lines_h.at<unsigned char>(i,j)) > 0 )
			{//Edge pixel
				if( ptFlag == 0 )
				{
					ptFlag = 1;
					lines_h[i].push_back(j);
				}
			}else
			{//Non-edge pixel
				if( ptFlag == 1 )
				{
					ptFlag = 0;
					lines_h[i].push_back(j);
					cnt_h ++;
				}
			}
		}
		if( ptFlag == 1 )
		{
			ptFlag = 0;
			lines_h[i].push_back(j);
			cnt_h ++;
		}
		tot_h += cnt_h;
	}
	lines_h[frame.rows-1].push_back(0);
	lines_h[frame.rows-1].push_back(frame.cols);


	lines_v.clear();
	lines_v.resize(frame.cols);
	lines_v[0].push_back(0);
	lines_v[0].push_back(frame.rows);
	
	tot_v = 2;
	ptFlag = 0;
	for( j=1; j<frame.cols-1; j++ )
	{//start from second col to the second last col
		cnt_v  = 0;
		for( i=0; i< frame.rows; i++ )
		{
			if((int)(img_lines_v.at<unsigned char>(i,j)) > 0 )
			{//Edge pixel
				if( ptFlag == 0 )
				{
					ptFlag = 1;
					lines_v[j].push_back(i);
				}
			}else
			{//Non-edge pixel
				if( ptFlag == 1 )
				{
					ptFlag = 0;
					lines_v[j].push_back(i);
					cnt_v ++;
				}
			}
		}
		if( ptFlag == 1 )
		{
			ptFlag = 0;
			lines_v[j].push_back(i);
			cnt_v ++;
		}
		tot_v += cnt_v;
	}
	lines_v[frame.cols-1].push_back(0);
	lines_v[frame.cols-1].push_back(frame.rows);

	printf("total horizontal and vertical lines: %d and %d\n", tot_h,tot_v);
}

void adjustLines(vector<HVLine>& c_lines)
{
	int preLevel = -1;
	int preIndex = -1;
	for(int i=0; i<c_lines.size(); i++)
	{
		if(preLevel<0)
		{
			preLevel = c_lines[i].level;
			continue;
		}

		if(c_lines[i].level == preLevel) 
		{
			continue;
		}
		if(c_lines[i].level - preLevel<=3)
		{
			c_lines[i].level = preLevel;
		}
		else
		{
			preLevel = c_lines[i].level;
		}
	}

	std::sort(c_lines.begin(),c_lines.end(),line_sort());

	vector<HVLine> cc_lines;
	int i=0;
	while(i<c_lines.size())
	{
		int start = c_lines[i].start;
		int end = c_lines[i].end;
		int level = c_lines[i].level;
		int j=i+1;
		for( ;j<c_lines.size(); j++)
		{
			if(c_lines[j].level != level || 
				c_lines[j].start>end+20)
			{
				break;
			}

			if(start<=c_lines[j].start && end>=c_lines[j].end)
			{
				continue;
			}
			else if(start>=c_lines[j].start && end<=c_lines[j].end)
			{
				start = c_lines[j].start;
				end = c_lines[j].end;
				continue;
			}
			else
			{	
				start = min(c_lines[j].start,start);;
				end = c_lines[j].end;
			}
		}
		HVLine l;
		l.start = start;
		l.end = end;
		l.level = level;
		
		if(l.end - l.start > 25)
		{
			cc_lines.push_back(l);
		}
		
		i = j;
	}

	c_lines = cc_lines;
}

void ObjMan::mergeLines(vector<HVLine>& m_lines, vector<vector<int>>& lines, int limit)
{
	int i,j;
	bool flag;
	int start, end;
	for(i=0; i<lines.size(); i++)
	{
		flag = 0;
		for(j=0; j<lines[i].size(); j+=2)
		{
			if(!flag)
			{
				start = lines[i][j];
			}

			if(j<lines[i].size()-2 && lines[i][j+2]-lines[i][j+1]<limit)
			{
				flag = true;
			}
			else
			{
				end = lines[i][j+1];
				flag = false;

				HVLine aline;
				aline.level = i;
				aline.start = start;
				aline.end = end;
				m_lines.push_back(aline);
			}
		}
	}
}

void ObjMan::mergeLines(int limit)
{
	mergeLines(m_lines_h,lines_h,limit);
	mergeLines(m_lines_v,lines_v,limit);

	adjustLines(m_lines_h);
	adjustLines(m_lines_v);

	printEdgeMap(m_lines_h,m_lines_v,frame,"m_edge_map.png");
}

void ObjMan::clusterLines(vector<HVLine>& c_lines, vector<HVLine>& m_lines, int limit)
{
	int num = m_lines.size();
	Mat m = Mat::zeros(num,num,CV_64FC1);

	for(int i=0; i<num; i++)
	{
		for(int j=0; j<num; j++)
		{
			if(i==j)
			{
				m.at<double>(i,j) = 0;
			}
			else if(abs(m_lines[i].level - m_lines[j].level) < limit)
			{
				int s1 = m_lines[i].start;
				int e1 = m_lines[i].end;
				int s2 = m_lines[j].start;
				int e2 = m_lines[j].end;
				m.at<double>(i,j) = ((double)(min(e1,e2) - max(s1,s2)))/ (e1 - s1);
			}
		}
	}

	hash_map<int, int> hsum;
	for(int i=0; i<num; i++)
	{
		if(hsum.find(m_lines[i].level) != hsum.end())
		{
			hsum[m_lines[i].level] += m_lines[i].end - m_lines[i].start;
		}
		else
		{
			hsum[m_lines[i].level] = m_lines[i].end - m_lines[i].start;
		}
	}

	vector<int> del;
	for(int i=0; i<num; i++)
	{
		for(int j=i+1;j<num; j++)
		{
			if(m.at<double>(i,j) > 0.8 || m.at<double>(j,i) > 0.8)
			{
				if(hsum[m_lines[i].level] < hsum[m_lines[j].level])
				{
					if(m_lines[i].start < m_lines[j].start)
					{
						m_lines[j].start = m_lines[i].start;
					}

					if(m_lines[i].end > m_lines[j].end)
					{
						m_lines[j].end = m_lines[i].end;
					}

					del.push_back(i);
				}
				else
				{
					if(m_lines[j].start < m_lines[i].start)
					{
						m_lines[i].start = m_lines[j].start;
					}

					if(m_lines[j].end > m_lines[i].end)
					{
						m_lines[i].end = m_lines[j].end;
					}

					del.push_back(j);
				}
			}
		}
	}

	for(int i=0; i<m_lines.size(); i++)
	{
		int len = m_lines[i].end - m_lines[i].start;
		if(std::find(del.begin(),del.end(),i) == del.end()) 
		{
			c_lines.push_back(m_lines[i]);
		}
	}
}

void ObjMan::clusterLines()
{
	//c_lines_h = m_lines_h;
	//c_lines_v = m_lines_v;
	clusterLines(c_lines_h,m_lines_h,10);
	clusterLines(c_lines_v,m_lines_v,10);
	
	//adjust lines if the level distance between two lines is very small
	//adjustLines(c_lines_h);
	//adjustLines(c_lines_v);

	if(DEBUGGING)
	{
		printEdgeMap(c_lines_h,c_lines_v,frame,"edge_map.png");
	}
	
}

vector<Corner> getAllCross(vector<HVLine> lines, int level)
{
	vector<Corner> cross;
	for(int i=0; i<lines.size(); i++)
	{
		if(lines[i].level == level)
		{
			for(int j=0; j<lines[i].corners.size(); j++)
			{
				Corner cor;
				cross.push_back(lines[i].corners[j]);
			}
		}
	}

	sort(cross.begin(),cross.end(),corner_sort());

	return cross;
}

void ObjMan::cornerDetect()
{
	const int interval = 20;
	int i,j,u,v;
	Mat img_lines = img_lines_h + img_lines_v;

	vector<Point> v_corners;
	
	for(i=0; i<c_lines_h.size(); i++)
	{
		int hlevel = c_lines_h[i].level;
		int hstart = c_lines_h[i].start;
		int hend = c_lines_h[i].end;
		for(j=0; j<c_lines_v.size(); j++)
		{
			int vlevel = c_lines_v[j].level;
			int vstart = c_lines_v[j].start;
			int vend = c_lines_v[j].end;

			/*
				corner type:  
				1 | 2
				------
				3 | 4
			*/
			if(vlevel>=hstart-interval && vlevel<=hend+interval && 
                hlevel>=vstart-interval && hlevel<=vend+interval)
			{
				Corner cor1,cor2;
				cor1.value = vlevel;
				cor2.value = hlevel;

				int c1 = 0; //corner type 1
				if(hstart+interval<vlevel && vstart+interval<hlevel)
				{
					c1 = 1;
				}

				int c2 = 0;
				if(hend-interval>vlevel && vstart+interval<hlevel)
				{
					c2 = 2;
				}

				int c3 = 0;
				if(hstart+interval<vlevel && vend-interval>hlevel)
				{
					c3 = 4;
				}

				int c4 = 0;
				if(hend-interval>vlevel && vend-interval>hlevel)
				{
					c4 = 8;
				}

				int type = c1 | c2 | c3 | c4;

				cor1.type = type;
				cor2.type = type;
				c_lines_h[i].corners.push_back(cor1);
				c_lines_v[j].corners.push_back(cor2);

				if(DEBUGGING)
				{
					circle( frame, Point(vlevel,hlevel), 5,  Scalar(0,0,255), 2, 8, 0 );
				}
			}
		}
	}
}

void ObjMan::divideIntoRect()
{
	int preLevel = -1;

	int sz = c_lines_v.size()-1;
	for(int i=0; i<sz; i++)
	{
		if(preLevel != c_lines_v[i].level)
		{
			vector<Corner> cross_v = getAllCross(c_lines_v,c_lines_v[i].level);

			for(int j=0; j<cross_v.size(); j++)
			{
				int v2 = -1;
				for(int k=j+1; k<cross_v.size(); k++)
				{
					int v1 =  c_lines_v[i].level;
					vector<Corner> cross_h1 = getAllCross(c_lines_h, cross_v[j].value);
					vector<Corner> cross_h2 = getAllCross(c_lines_h, cross_v[k].value);

					int m=0;
					int n=0;
					while(m<cross_h1.size()&&cross_h1[m].value<=v1) { m++;}
					while(n<cross_h2.size()&&cross_h2[n].value<=v1) { n++;}

					while(m<cross_h1.size() && n<cross_h2.size())
					{
						if(cross_h1[m].value == cross_h2[n].value)
						{
							v2 = cross_h1[m].value;
							break;
						}
						else if(cross_h1[m].value > cross_h2[n].value)
						{
							n++;
						}
						else
						{
							m++;
						}
					}
					if(v2 != -1)
					{
						rects.push_back(Rect(v1,cross_v[j].value,v2-v1+1,cross_v[k].value-cross_v[j].value+1));

						/*
						if(DEBUGGING)
						{
							rectangle(frame,Rect(v1,cross_v[j].value,v2-v1+1,cross_v[k].value-cross_v[j].value+1),Scalar(255,0,255));
						}
						*/
						break;
					}
				}
			}
		}
		
		preLevel = c_lines_v[i].level;
	}
}

void ObjMan::matchTplCompsToFrame()
{
	int i; 

	for( i=0; i<templates.size(); i++ )
	{
		detects.push_back(templates[i].matchTplComp(frame_gray, frame_kps, frame_des,img_edge));		
	}

	if(DEBUGGING)
	{
		for(i=0; i<detects.size(); i++)
		{
			for(int j=0; j<detects[i].size(); j++)
			{
				rectangle(frame_paint,detects[i][j].rect,Scalar(0,0,255));
			}
		}
	}
}

void ObjMan::matchTplCompsToFrame(vector<AppTpl> tpls)
{
	for(int i=0; i<tpls.size(); i++ )
	{
		detectedItems.push_back(tpls[i].matchTplComp(frame_gray, frame_kps, frame_des,img_edge));		
	}

	if(GobalParam::IS_DEBUG)
	{
		for(int i=0; i<detectedItems.size(); i++)
		{
			for(int j=0; j<detectedItems[i].size(); j++)
			{
				rectangle(frame_paint,detectedItems[i][j].rect,Scalar(0,0,255));
			}
		}
	}
}

int ObjMan::getWeightBetween(RectDetect r1,RectDetect r2)
{
	const int MAX_GAP = 120;
	int w;
	if(abs(r1.rect.y - r2.rect.y) > MAX_GAP)
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

	w = max(w1,w2);
	double ra = exp(0-(double)w/frame.rows);
	
	if(ra > 0.8) return 1;
	else return 0;
}

int distanceBetweenRow(Mat row1, Mat row2)
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

void ObjMan::groupWinObj()
{	
	for(int i=0; i<templates.size(); i++)
	{
		int count = 0;
		for(int j=0; j<detects[i].size(); j++)
		{	
			WinTplComp comp = templates[i].getWinTplComp(detects[i][j].row_id,detects[i][j].item_id);
			if(comp.is_unique>0)
			{
				count++;
			}
		}

		if(templates[i].templateName == "TaskBar" && count>=2)
		{
			WinObj obj;
			obj.name = templates[i].templateName;
			obj.row_num =  templates[i].rowNum;
			obj.content_row = templates[i].content_row;
			obj.url_row = templates[i].browser_url_row;
			obj.compList = detects[i];
			objs.push_back(obj);
		}
		else if(count>3) 
		 { 
			vector<RectDetect> menus;
			vector<RectDetect> others;
			for(int m=0; m<detects[i].size(); m++)
			{
				if(detects[i][m].row_id < templates[i].content_row || templates[i].content_row == -1)
				{
					menus.push_back(detects[i][m]);
				}
				else
				{
					others.push_back(detects[i][m]);
				}
			}

			if(menus.size() <= 0) return;

			Mat W = Mat::zeros(menus.size(),menus.size(),CV_16U);
			for(int j=0; j<menus.size(); j++)
			{
				W.at<ushort>(j,j) = 1;
				for(int k=j+1; k<menus.size();k++)
				{
					W.at<ushort>(j,k) = getWeightBetween(menus[j],menus[k]);
					W.at<ushort>(k,j) = W.at<ushort>(j,k);
				}
			}

			for(int j=0; j<W.rows; j++)
			{
				for(int k=0; k<W.cols; k++)
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
				vector<RectDetect> group;
				for(int k=0; k<groups[j].size();k++)
				{
					group.push_back(menus[groups[j][k]]);
				}

				if(group.size()>3)
				{
				
					for(int k=0; k<group.size(); k++)
					{
						rectangle(frame,group[k].rect,Scalar(0,255,0));
					}

					WinObj obj;
					obj.name = templates[i].templateName;
					obj.row_num =  templates[i].rowNum;
					obj.content_row = templates[i].content_row;
					obj.url_row = templates[i].browser_url_row;
					obj.compList.insert(obj.compList.begin(),group.begin(),group.end());
					obj.compList.insert(obj.compList.begin(),others.begin(),others.end());
					objs.push_back(obj);
				}
			}	
			/*
			vector<RectDetect> group1;
			vector<RectDetect> group2;
			group1.push_back(menus[0]);
			for(int j=1; j<menus.size(); j++)
			{
				if(W.at<ushort>(0,0) == W.at<ushort>(0,j))
				{
					group1.push_back(menus[j]);
				}
				else
				{
					group2.push_back(menus[j]);
				}
			}

			if(group1.size() > 3)
			{
				WinObj obj;
				obj.name = templates[i].templateName;
				obj.row_num =  templates[i].rowNum;
				obj.content_row = templates[i].content_row;
				obj.url_row = templates[i].browser_url_row;
				obj.compList.insert(obj.compList.begin(),group1.begin(),group1.end());
				obj.compList.insert(obj.compList.begin(),others.begin(),others.end());
				objs.push_back(obj);
			}

			if(group2.size() > 3)
			{
				WinObj obj;
				obj.name = templates[i].templateName;
				obj.row_num =  templates[i].rowNum;
				obj.content_row = templates[i].content_row;
				obj.url_row = templates[i].browser_url_row;
				obj.compList.insert(obj.compList.begin(),group2.begin(),group2.end());
				obj.compList.insert(obj.compList.begin(),others.begin(),others.end());
				objs.push_back(obj);
			}
			*/
		 }
		 
	}
}

int getIndexOfLine(vector<HVLine>& lines, int level)
{
	int index=-1;
	for(int i=0; i<lines.size(); i++)
	{
		if(lines[i].level == level)
		{
			index = i;
			break;
		}
	}
	return index;
}

int getIndexOfLine(vector<HVLine>& lines, int level, int pos)
{
	int index=-1;
	for(int i=0; i<lines.size(); i++)
	{
		if(lines[i].level == level && lines[i].start-20<=pos && lines[i].end+20>=pos)
		{
			index = i;
			break;
		}
	}
	return index;
}

vector<HVLine> getLinesInLevel(vector<HVLine>& lines, int level)
{
	vector<HVLine> llines;
	for(int i=0; i<lines.size(); i++)
	{
		if(lines[i].level == level)
		{
			llines.push_back(lines[i]);
		}
	}
	return llines;
}

bool rectContain(Rect r1, Rect r2) // is r1 contain r2
{
	if(r1.contains(r2.tl())&&
		r1.contains(r2.br()-Point(1,1)))
	{
		return true;
	}
	return false;
}

int getLineLengthBetween(vector<HVLine>& lines, int level, int start, int end)
{
	int len = 0;
	for(int i=0; i<lines.size(); i++)
	{
		if(level == lines[i].level)
		{
			if(lines[i].start < end && lines[i].end > start)
			{
				len += min(end,lines[i].end)- max(start,lines[i].start);
			}
		}
	}

	return len;
}

bool ObjMan::getBoundary(WinObj& obj)
{
	const int MAXGAP = 50;

	Rect menu = obj.menu_tightest;
	int upper,down;
	upper = down = -1;

	int left, right;
	left = right = -1;
	int preLevel = -1;
	for(int i=0; i<c_lines_h.size(); i++)
	{
		if(preLevel == c_lines_h[i].level) continue;

		if(c_lines_h[i].level+MAXGAP < menu.y || c_lines_h[i].level > menu.y) continue;

		int len = getLineLengthBetween(c_lines_h,c_lines_h[i].level, menu.x, menu.x+menu.width-1);

		double ratio = len * 1.0 / menu.width;
		if(ratio < 0.5) continue;

		int tmpLeft, tmpRight;
		tmpLeft = tmpRight = -1;
		vector<Corner> crosses = getAllCross(c_lines_h,c_lines_h[i].level);

		for(int j=0; j<crosses.size(); j++)
		{
			if(menu.x>=crosses[j].value && (crosses[j].type & 8)>0)
			{
				tmpLeft = crosses[j].value;			
			}

			if(menu.x+menu.width-1<=crosses[j].value && (crosses[j].type & 4)>0)
			{
				tmpRight = crosses[j].value;		
				break;
			}
		}

		if(tmpLeft<0 && tmpRight<0) continue;
		
		left = tmpLeft;
		right = tmpRight;
		upper = c_lines_h[i].level;
		preLevel = c_lines_h[i].level;
		break;
	}

	for(int i=0; i<c_lines_h.size(); i++)
	{
		if(c_lines_h[i].level-MAXGAP >= menu.y+menu.height-1 || c_lines_h[i].level < menu.y+menu.height-1) continue;

		int len = getLineLengthBetween(c_lines_h,c_lines_h[i].level, menu.x, menu.x+menu.width-1);

		double ratio = len * 1.0 / menu.width;
		if(ratio < 0.5) continue;

		if(left<0 && right<0)
		{
			vector<Corner> crosses = getAllCross(c_lines_h,c_lines_h[i].level);

			for(int j=0; j<crosses.size(); j++)
			{
				if(menu.x>=crosses[j].value && (crosses[j].type & 8)>0)
				{
					left = crosses[j].value;			
				}

				if(menu.x+menu.width-1<=crosses[j].value && (crosses[j].type & 4)>0)
				{
					right = crosses[j].value;		
					break;
				}
			}
		}
		down = c_lines_h[i].level;
		break;
	}

	if(upper <0) upper = menu.y;
	if(down < 0) down = menu.y + menu.height - 1;

	obj.menu_line = down;

	if(left <0 && right<0)
	{
		return false;
	}
	else if(left >=0 && right>=0)
	{
		obj.border.x = left;
		obj.border.y = upper;
		obj.border.width = right - left+1;
		
		if(obj.border.width == frame.cols && upper_taskbar<frame.rows && objs.size()==2)
		{
			obj.border.height = upper_taskbar;
			return true;
		}

		vector<Corner> vcross1 = getAllCross(c_lines_v,left);
		vector<Corner> vcross2 = getAllCross(c_lines_v,right);

		int n = vcross1.size();
		int m = vcross2.size();
		bool flag = false;
		int bottom = -1;
		for(int i=n-1; i>=0; i--)
		{
			for(int j=m-1; j>=0; j--)
			{
				if(vcross2[j].value == vcross1[i].value && vcross1[i].value<=upper_taskbar
					&&(vcross1[i].type & 2) > 0 && (vcross2[j].type & 1) > 0)
				{
					if(vcross2[j].value > obj.menu_line)
					{
						obj.border.height = vcross2[j].value - upper+1;;
						return true;
					}
				}
			}
		}
		
		int u = n-1;
		while(vcross1[u].type & 2 <0) u--;

		int v = m-1;
		while(vcross2[v].type & 1 <0) v--;

		if(vcross1[u-1].value>vcross2[v-1].value) 
		{
			bottom = vcross1[u-1].value;
		}
		else
		{
			bottom = vcross2[v-1].value;
		}
		obj.border.height = bottom - upper+1; 
		return true;
	}
	else if(left>=0)
	{
		vector<Corner> crosses = getAllCross(c_lines_v, left);
		for(int i=crosses.size()-1; i>=0; i--)
		{
			if(crosses[i].value <= down) break;

			if( crosses[i].value>upper_taskbar) continue;

			int index2 = getIndexOfLine(c_lines_h,crosses[i].value,left);
			if(index2<0) return false;
			
			vector<Corner> crosses2 = getAllCross(c_lines_h, c_lines_h[index2].level);
			for(int j=0; j<crosses2.size(); j++)
			{
				if(crosses2[j].value - left > menu.width && crosses2[j].value>=menu.x+menu.width-1
					&& (crosses2[j].type & 1)>0)
				{
					obj.border.x = left;
					obj.border.y = upper;
					obj.border.width = crosses2[j].value - left+1;
					obj.border.height = crosses[i].value - upper + 1;
					return true;
				}
			}
		} 
		return false;
	}
	else
	{
		vector<Corner> crosses = getAllCross(c_lines_v, right);
		for(int i=crosses.size()-1; i>=0; i--)
		{
			if(crosses[i].value <= down) break;

			if( crosses[i].value>upper_taskbar || crosses[i].type & 1 <= 0) continue;

			int index2 = getIndexOfLine(c_lines_h, crosses[i].value,right);
			if(index2<0) return false;

			vector<Corner> crosses2 = getAllCross(c_lines_h, c_lines_h[index2].level);
			for(int j=crosses2.size()-1; j>=0; j--)
			{
				if(right -crosses2[j].value> menu.width && crosses2[j].value <= menu.x
					&& (crosses2[j].type & 2)>0 )
				{
					obj.border.x = crosses2[j].value;
					obj.border.y = upper;
					obj.border.width = right - crosses2[j].value + 1;
					obj.border.height = crosses[i].value - upper + 1;
					return true;
				}
			}
		} 
		return false;
	}

}

void ObjMan::getHorizontalBorderLine(Rect rect, int& upper, int& down) 
{
	const int interval = 2;
	const int MAXGAP = 30;//frame.rows / 20; // search gap 

	upper = down = -1;
	int preLevel = -1;
	for(int i=0; i<c_lines_h.size(); i++)
	{
		if(preLevel == c_lines_h[i].level) continue;

		vector<Corner> crosses = getAllCross(c_lines_h,c_lines_h[i].level);

		int uleft = -1;
		int uright = -1;
		int dleft = -1;
		int dright = -1;
		for(int j=0; j<crosses.size(); j++)
		{
			if(rect.x>=crosses[j].value && (crosses[j].type & 8)>0)
			{
				uleft = crosses[j].value;			
			}

			if(rect.x+rect.width-1<=crosses[j].value && (crosses[j].type & 4)>0)
			{
				uright = crosses[j].value;		
				break;
			}
		}

		for(int j=0; j<crosses.size(); j++)
		{
			if(rect.x>=crosses[j].value && (crosses[j].type & 10)>0)
			{
				dleft = crosses[j].value;			
			}

			if(rect.x+rect.width-1<=crosses[j].value && (crosses[j].type & 5)>0)
			{
				dright = crosses[j].value;	
				break;
			}
		}

		if(c_lines_h[i].level+MAXGAP>=rect.y && c_lines_h[i].level<=rect.y)
		{
			if(uleft>0 && uright>0)
			{
				upper = c_lines_h[i].level;
			}
			else if( (uleft>0 || uright>0) && upper<0)
			{
				upper = c_lines_h[i].level;
			}
		}

		if(c_lines_h[i].level-MAXGAP < rect.y+rect.height-1 && c_lines_h[i].level>=rect.y+rect.height-1 )
		{
			if(dleft>0 && dright>0)
			{
				down = c_lines_h[i].level;
				break;
			}
			else if( (dleft>0 || dright>0) && upper<0)
			{
				upper = c_lines_h[i].level;
			}
		}

		preLevel = c_lines_h[i].level;
	}
}

/*
void ObjMan::getHorizontalBorderLine(Rect rect, int& upper, int& down) 
{
	const int interval = 2;
	const int MAXGAP = 30;//frame.rows / 20; // search gap 

	upper = down = -1;
	int preLevel = -1;
	for(int i=0; i<c_lines_h.size(); )
	{
		int level = c_lines_h[i].level;
		int start = c_lines_h[i].start;
		int end = c_lines_h[i].end;

		int j;
		int sum = 0;
		
		if(end>rect.x && start<rect.x+rect.width-1)
		{
			sum += end - start;
		}

		for(j=i+1; j<c_lines_h.size(); j++)
		{
			if(c_lines_h[j].level == level)
			{
				if(c_lines_h[j].end>rect.x && c_lines_h[j].start<rect.x+rect.width-1)
				{
					sum += c_lines_h[j].end - c_lines_h[j].start;
				}
			}
			else
			{
				break;
			}
		}

		i = j;
		double ratio = (double)sum / rect.width;
		if(level+MAXGAP>=rect.y && level<=rect.y && ratio>0.4)
		{
			upper = level;
		}

		if(level-MAXGAP < rect.y+rect.height-1 && level>=rect.y+rect.height-1 && ratio>0.4)
		{
			down = level;
			break;
		}

	}
}
*/

/*
bool ObjMan::getBoundary(WinObj& obj)
{
	int upper, down;
	getHorizontalBorderLine(obj.menu_tightest,upper,down);

	if(down != -1)
		obj.menu_line = down;
	else
		obj.menu_line = obj.menu_tightest.y+obj.menu_tightest.height;

	if(upper<0)
	{
		return false;
	}

	Rect menu = obj.menu_tightest;

	int index = getIndexOfLine(c_lines_h,upper);
	if(index<0)
		return false;
	
	int left, right;
	int base = upper;
	left = right= -1;
	vector<Corner> crosses = getAllCross(c_lines_h,upper);
	
	if(crosses.size()<=0)
	{
		crosses = getAllCross(c_lines_h,down);
		base = down;
	}

	int indexLeft, indexRight;
	for(int i=0; i<crosses.size(); i++)
	{
		int index = getIndexOfLine(c_lines_v,crosses[i].value,base);

		int len = c_lines_v[index].end - c_lines_v[index].start;
		int flag = (crosses[i].type & 8);
		if(menu.x>=crosses[i].value && flag > 0  && len>30)
		{
			left = crosses[i].value;
			indexLeft = index;
		}

		flag = crosses[i].type & 4;
		if(menu.x + menu.width-1 <= crosses[i].value && (flag > 0) && len>30)
		{
			right = crosses[i].value;
			indexRight = index;
			break;
		}
	}

	if(left <0 && right<0)
	{
		return false;
	}
	else if(left >=0 && right>=0)
	{
		obj.border.x = left;
		obj.border.y = upper;
		obj.border.width = right - left+1;
		
		if(obj.border.width == frame.cols && upper_taskbar<frame.rows && objs.size()==2)
		{
			obj.border.height = upper_taskbar;
			return true;
		}


		vector<Corner> vcross1 = getAllCross(c_lines_v,left);
		vector<Corner> vcross2 = getAllCross(c_lines_v,right);

		int n = vcross1.size();
		int m = vcross2.size();
		bool flag = false;
		int bottom = -1;
		for(int i=n-1; i>=0; i--)
		{
			for(int j=m-1; j>=0; j--)
			{
				if(vcross2[j].value == vcross1[i].value && vcross1[i].value<=upper_taskbar
					&&(vcross1[i].type & 2) > 0 && (vcross2[j].type & 1) > 0)
				{
					if(vcross2[j].value > obj.menu_line)
					{
						obj.border.height = vcross2[j].value - upper+1;;
						return true;
					}
				}
			}
		}
		
		int u = n-1;
		while(vcross1[u].type & 2 <0) u--;

		int v = m-1;
		while(vcross1[v].type & 1 <0) v--;

		if(vcross1[u-1].value>vcross2[v-1].value) 
		{
			bottom = vcross1[u-1].value;
		}
		else
		{
			bottom = vcross2[v-1].value;
		}
		obj.border.height = bottom - upper+1; 
		return true;
	}
	else if(left>=0)
	{
		vector<Corner> crosses = getAllCross(c_lines_v, left);
		for(int i=crosses.size()-1; i>=0; i--)
		{
			if(crosses[i].value <= down) break;

			if( crosses[i].value>upper_taskbar) continue;

			int index2 = getIndexOfLine(c_lines_h,crosses[i].value,left);
			if(index2<0) return false;
			
			vector<Corner> crosses2 = getAllCross(c_lines_h, c_lines_h[index2].level);
			for(int j=0; j<crosses2.size(); j++)
			{
				if(crosses2[j].value - left > menu.width && crosses2[j].value>=menu.x+menu.width-1
					&& (crosses2[j].type & 1)>0)
				{
					obj.border.x = left;
					obj.border.y = upper;
					obj.border.width = crosses2[j].value - left+1;
					obj.border.height = crosses[i].value - upper + 1;
					return true;
				}
			}
		} 
		return false;
	}
	else
	{
		vector<Corner> crosses = getAllCross(c_lines_v, right);
		for(int i=crosses.size()-1; i>=0; i--)
		{
			if(crosses[i].value <= down) break;

			if( crosses[i].value>upper_taskbar || crosses[i].type & 1 <= 0) continue;

			int index2 = getIndexOfLine(c_lines_h, crosses[i].value,right);
			if(index2<0) return false;

			vector<Corner> crosses2 = getAllCross(c_lines_h, c_lines_h[index2].level);
			for(int j=crosses2.size()-1; j>=0; j--)
			{
				if(right -crosses2[j].value> menu.width && crosses2[j].value <= menu.x
					&& (crosses2[j].type & 2)>0 )
				{
					obj.border.x = crosses2[j].value;
					obj.border.y = upper;
					obj.border.width = right - crosses2[j].value + 1;
					obj.border.height = crosses[i].value - upper + 1;
					return true;
				}
			}
		} 
		return false;
	}
	
	return false;
}
*/


void ObjMan::cropChrome(WinObj& obj)
{
	char urlPath[MAXSLEN];
	char urlFile[MAXSLEN];
	strcpy(urlPath,outputFolder);

	const int MAX_ROW_HEIGHT = 50;

	int count = 0;
	//URL crop
	Rect rect = obj.rowRect[obj.url_row-1];
	
	for(int i=0; i<c_lines_h.size(); i++)
	{
		if(rect.y+5 >= c_lines_h[i].level && rect.y-5 <= c_lines_h[i].level)
		{
			int len = c_lines_h[i].end - c_lines_h[i].start;

			if(len > obj.border.width / 3)
			{
				sprintf(urlFile,"%d_URL.png",frame_seq);
				strcat(urlPath,urlFile);
				Rect urlRect;
				urlRect.x = c_lines_h[i].start;
				urlRect.y = c_lines_h[i].level;
				urlRect.width = len;
				urlRect.height = rect.height+2;
				//imwrite(urlPath,frame(urlRect));
				partImageWrite(urlPath, frame, urlRect);
				count++;
				break;
			}
		}
	}
	
	/*
	vector<RectDetect> rd = obj.getCompListByRow2(obj.url_row);
	sort(rd.begin(),rd.end(),left_rect_detect());
	bool isFind = false;
	if(rd.size() > 0)
	{
		for(int i=0; i<rd.size()-1; i++)
		{
			if(rd[i+1].rect.x - rd[i].rect.x>obj.border.width/3)
			{
				sprintf(urlFile,"%d_URL.png",frame_seq);
				strcat(urlPath,urlFile);
				Rect urlRect;
				urlRect.x = rd[i].rect.x+30;
				urlRect.y = rd[i].rect.y;
				urlRect.width = rd[i+1].rect.x - urlRect.x - 30;
				urlRect.height = rect.height;
				//imwrite(urlPath,frame(urlRect));
				partImageWrite(urlPath, frame, urlRect);
				isFind = true;
				count++;
			}
		}
		if(!isFind)
		{
			sprintf(urlFile,"%d_URL.png",frame_seq);
			strcat(urlPath,urlFile);
			Rect urlRect;
			urlRect.x = obj.border.x;
			urlRect.y = rd[0].rect.y;
			urlRect.width = obj.border.width;
			urlRect.height = rd[0].rect.height;
			//imwrite(urlPath,frame(urlRect));
			partImageWrite(urlPath, frame, urlRect);
			count++;
		}
	}
	*/

	Rect contentRect;
	contentRect.x = obj.border.x;
	contentRect.y = obj.menu_line;
	contentRect.width = obj.border.width;
	contentRect.height = obj.border.height + obj.border.y - obj.menu_line - 1;

	char contentPath[MAXSLEN];
	char contentFile[MAXSLEN];
	strcpy(contentPath,outputFolder);
	sprintf(contentFile,"%d_CONTENT.png",frame_seq);
	strcat(contentPath,contentFile);
	partImageWrite(contentPath,frame,contentRect);
	count++;

	char searchPath[MAXSLEN];
	char searchFile[MAXSLEN];
	strcpy(searchPath,outputFolder);
	vector<RectDetect> rd = obj.getCompListByRow(obj.content_row);
	if(rd.size()>0)
	{
		rect = rd[0].rect;

		for(int i=0; i<c_lines_h.size(); i++)
		{
			if(rect.y+10 >= c_lines_h[i].level && rect.y-10 <= c_lines_h[i].level)
			{
				if(c_lines_h[i].end - c_lines_h[i].start > 300)
				{
					sprintf(searchFile,"%d_SEARCH.png",frame_seq);
					strcat(searchPath,searchFile);
					Rect searchRect;
					searchRect.x = c_lines_h[i].start;
					searchRect.y = c_lines_h[i].level;
					searchRect.width = rect.x - c_lines_h[i].start - 20;
					searchRect.height = rect.height + 10;
					partImageWrite(searchPath,frame,searchRect);
					count++;
					break;
				}
			}
		}

		/*
		for(int i=0; i<rects.size(); i++)
		{
			int temp = rect.width / (rect.height+1);
			if(rectContain(contentRect,rects[i]) && 
				abs(rect.y - rects[i].y)<15 && rect.x > rects[i].x &&
				rects[i].width / (rects[i].height+1) > 10)
			{
				sprintf(searchFile,"%d_SEARCH.png",frame_seq);
				strcat(searchPath,searchFile);
				partImageWrite(searchPath,frame,rects[i]);
				count++;
				break;
			}
		}
		*/
	}

	fprintf(fresult,"%d %s %d\n",frame_seq,obj.name.c_str(),count);
}

vector<RectDetect> getDetectedInRect(Rect rect, vector<RectDetect> detects)
{
	vector<RectDetect> temp;
	for(int j=0; j<detects.size(); j++)
	{
		if(rectContain(rect,detects[j].rect) && abs(rect.y - detects[j].rect.y) <= 15)
		{
			temp.push_back(detects[j]);
		}
	}
	return temp;
}

void ObjMan::cropEclipse(WinObj& obj)
{
	Rect contentRect;
	contentRect.x = obj.border.x;
	contentRect.y = obj.menu_line;
	contentRect.width = obj.border.width;
	contentRect.height = obj.border.height + obj.border.y - obj.menu_line - 1;

	vector<RectDetect> detects = obj.getCompListByRow(obj.content_row);
	int count = 0;

	vector<Rect> filterRect;
	for(int i=0; i<rects.size(); i++)
	{
		if(rectContain(contentRect,rects[i]) && rects[i].width>6 && rects[i].height>6)
		{
			filterRect.push_back(rects[i]);
		}
	}

	vector<Rect> outRect;
	for(int i=0; i<filterRect.size(); i++)
	{
		vector<RectDetect> temp = getDetectedInRect(filterRect[i], detects);

		int flag = false;
		for(int j=0; j<temp.size(); j++)
		{
			if(temp[j].item_id == 7 || temp[j].item_id == 8 ||
				temp[j].item_id == 2 || temp[j].item_id == 3 ||
				temp[j].item_id == 9 || temp[j].item_id == 10 ||
				temp[j].item_id == 11)
			{
				flag = true;
			}
		}

		if(flag)
		{
			vector<Corner> cross1 = getAllCross(c_lines_v,filterRect[i].x);
			vector<Corner> cross2 = getAllCross(c_lines_v,filterRect[i].x+filterRect[i].width-1);

			int m = cross1.size()-1;
			int n = cross2.size()-1;
			bool isFind = false;
			while(m>=0&&n>=0&&
				(cross1[m].value>filterRect[i].y + 20 &&  
				 cross2[n].value>filterRect[i].y + 20 ))
			{
				if(cross1[m].value == cross2[n].value)
				{
					Rect temp;
					temp.x = filterRect[i].x;
					temp.y = filterRect[i].y;
					temp.width = filterRect[i].width;
					temp.height = cross1[m].value - filterRect[i].y + 1;
					outRect.push_back(temp);
					isFind = true;
					break;
				}
				else if(cross1[m].value > cross2[n].value)
				{
					m--;
				}
				else
				{
					n--;
				}
			}

			m = cross1.size()-1;
			n = cross2.size()-1;
			if(!isFind && m>=0 && n>=0)
			{
				Rect temp;
				temp.x = filterRect[i].x;
				temp.y = filterRect[i].y;
				temp.width = filterRect[i].width;
				temp.height = max(cross1[m].value,cross2[n].value) - filterRect[i].y + 1;
				outRect.push_back(temp);
			}
		}
	}

	vector<Rect> outRect2;
	for(int i=0; i<outRect.size(); i++)
	{
		bool isContained = false;
		for(int j=0; j<outRect.size(); j++)
		{
			if(i != j && rectContain(outRect[j],outRect[i])) 
			{
				isContained = true; break;
			}
		}
		if(!isContained) 
		{
			outRect2.push_back(outRect[i]);
		}
	}

	for(int i=1; i<=outRect2.size(); i++)
	{
		char fileName[MAXSLEN];
		char filePath[MAXSLEN];
		strcpy(filePath,outputFolder);
		sprintf(fileName,"%d_sub_%d.png",frame_seq,i);
		strcat(filePath,fileName);

		partImageWrite(filePath,frame,outRect2[i-1]);
	}

	fprintf(fresult,"%d %s %d\n",frame_seq, obj.name.c_str(),outRect2.size());
}
/*
void ObjMan::cropEclipse(WinObj& obj)
{
	//Item 1: package, 2: console, 3: problems, 4: task list, 5: outline, 6: help
	// 7: java, 8: java with warning, 9: declaration
	Rect contentRect;
	contentRect.x = obj.border.x;
	contentRect.y = obj.menu_line;
	contentRect.width = obj.border.width;
	contentRect.height = obj.border.height + obj.border.y - obj.menu_line - 1;

	vector<RectDetect> detects = obj.getCompListByRow(obj.content_row);
	int count = 0;

	vector<Rect> filterRect;
	for(int i=0; i<rects.size(); i++)
	{
		if(rectContain(contentRect,rects[i]) && rects[i].width>6 && rects[i].height>6)
		{
			filterRect.push_back(rects[i]);
		}
	}

	for(int i=0; i<filterRect.size(); i++)
	{
		vector<RectDetect> temp = getDetectedInRect(filterRect[i], detects);

		int flag = false;
		for(int j=0; j<temp.size(); j++)
		{
			if(temp[j].item_id == 7 || temp[j].item_id == 8 ||
				temp[j].item_id == 2 || temp[j].item_id == 3 ||
				temp[j].item_id == 9)
			{
				flag = true;
			}
		}

		if(flag)
		{
			for(int j=0; j<filterRect.size(); j++)
			{
				if(i==j) continue;

				
				if(rects[i].y>rects[j].y && rects[i].x<rects[j].x+rects[j].width-1 &&
					rects[j].x<rects[i].x+rects[i].width-1)
				{
					vector<RectDetect> temp2 = getDetectedInRect(rects[j],detects);
					if(temp2.size()>0) break;
				}


				if(filterRect[i].y+filterRect[i].height-1<=filterRect[j].y && 
					filterRect[j].x>=filterRect[i].x && 
					filterRect[j].x+filterRect[j].width<=filterRect[i].x+filterRect[i].width)
				{
					char fileName[MAXSLEN];
					char filePath[MAXSLEN];
					count++;
					strcpy(filePath,outputFolder);
					sprintf(fileName,"%d_sub_%d.png",frame_seq,count);
					strcat(filePath,fileName);

					Scalar v_sum;
					v_sum = cv::sum(img_edge(filterRect[j]));
					
					//cv::meanStdDev(frame_gray(filterRect[j]),v_mean,v_std);
					cout<<count<<" "<<v_sum.val[0]/(filterRect[j].width*filterRect[j].height)<<endl;

					imwrite(filePath,frame(filterRect[j]));
				}
			}
		}
	}
}
*/

void ObjMan::cropWord(WinObj& obj)
{
	Rect contentRect;
	contentRect.x = obj.border.x;
	contentRect.y = obj.menu_line;
	contentRect.width = obj.border.width;
	contentRect.height = obj.border.height + obj.border.y - obj.menu_line - 1;

	char contentPath[MAXSLEN];
	char contentFile[MAXSLEN];
	strcpy(contentPath,outputFolder);
	sprintf(contentFile,"%d_CONTENT.png",frame_seq);
	strcat(contentPath,contentFile);
	partImageWrite(contentPath,frame,contentRect);

	fprintf(fresult,"%d %s %d\n",frame_seq, obj.name.c_str(),1);
}

void ObjMan::cropTaskBar(WinObj& obj,int& taskbar_line)
{
	int upper, down;
	const int MAX_TASKBAR_HEIGHT = 50;

	vector<RectDetect> filterRects;
	for(int i=0; i<obj.compList.size(); i++)
	{
		if(frame.rows - obj.compList[i].rect.y<MAX_TASKBAR_HEIGHT)
		{
			filterRects.push_back(obj.compList[i]);
		}
	}
	obj.compList = filterRects;
	obj.findTightestRect();

	getHorizontalBorderLine(obj.tightest,upper,down);
	if(upper<0)
	{
		upper = obj.tightest.y;
	}

	taskbar_line = upper;
}

bool isRectSame(Rect r1, Rect r2)
{
	return r1.x == r2.x && r1.y == r2.y &&
		r1.width == r2.width && r1.height == r2.height;
}

void ObjMan::test()
{
 	char text[MAXSLEN];

	groupWinObj();

	Rect r;
	int upper, down;
	upper_taskbar=frame.cols+1;
	int i=0;
	if(objs.size()>0 && objs[0].name == "TaskBar")
	{
		cropTaskBar(objs[0],upper_taskbar);
		i++;
	}

	vector<WinObj> cropObjs;
	for(; i<objs.size(); i++)
	{
		objs[i].findTightestRect();
		if(objs[i].content_row > 0 && getBoundary(objs[i]))
		{
			cropObjs.push_back(objs[i]);
		}
	}

	for(int i=0; i<cropObjs.size(); i++)
	{
		if(upper_taskbar>0 && cropObjs[i].border.y+cropObjs[i].border.height > upper_taskbar)
		{
			cropObjs[i].border.height = upper_taskbar-cropObjs[i].border.y;
		}

		if(DEBUGGING)
		{
			rectangle(frame,cropObjs[i].border,Scalar(0,255,0),2);
		}

		bool flag = true;
		for(int j=0; j<cropObjs.size(); j++)
		{
			if(i==j) continue;

			if(isRectSame(cropObjs[i].border,cropObjs[j].border) == false &&
				rectContain(cropObjs[i].border,cropObjs[j].border) )
			{
				flag = false;  break;
			}
		}

		if(flag)
		{ 
			if(cropObjs[i].name == "Chrome" || cropObjs[i].name == "360Browser" || 
				cropObjs[i].name == "IE" || cropObjs[i].name == "Firefox")
			{
				cropChrome(cropObjs[i]);
			}
			else if(cropObjs[i].name == "Eclipse")
			{
				cropEclipse(cropObjs[i]);
			}
			else if(cropObjs[i].name == "Word")
			{
				cropWord(cropObjs[i]);
			}else if(cropObjs[i].name == "CHM")
			{
				cropWord(cropObjs[i]);
			}

				/*
				char borderPath[MAXSLEN];
				char borderFile[MAXSLEN];
				strcpy(borderPath,outputFolder);
				sprintf(borderFile,"%d_border.png",frame_seq);
				strcat(borderPath,borderFile);
				partImageWrite(borderPath,frame,Rect(objs[i].border.x,
					objs[i].menu_line,
					objs[i].border.width,
					objs[i].border.height-(objs[i].menu_line-objs[i].border.y)));
				*/
		}
	}

	imwrite("frame.png",frame);

	clear();
}

void ObjMan::clear()
{
	frame_kps.clear();

	for(int i=0; i<lines_h.size(); i++)
	{
		lines_h[i].clear();
	}
	lines_h.clear();

	for(int i=0; i<lines_v.size(); i++)
	{
		lines_v[i].clear();
	}
	lines_v.clear();

	m_lines_h.clear();
	m_lines_v.clear();
	c_lines_h.clear();
	c_lines_v.clear();

	rects.clear();

	for(int i=0; i<detects.size(); i++)
	{
		detects[i].clear();
	}
	detects.clear();


	objs.clear();

	//frame.release();
	//frame_gray.release();
	//frame_des.release();
	//img_lines_h.release();
	//img_lines_v.release();

	fflush(fresult);
}
