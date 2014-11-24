#include "EdgeInfo.h"

EdgeInfo::EdgeInfo(void)
{
}


EdgeInfo::~EdgeInfo(void)
{
}

void EdgeInfo::setRectLineImages(Mat frame_canny)
{
	int op_size = GobalParam::LINE_OP_SIZE;
	rows = frame_canny.rows;
	cols = frame_canny.cols;

	img_lines_h = frame_canny.clone();
	img_lines_v = frame_canny.clone();

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

	if(GobalParam::IS_DEBUG)
	{
		imwrite("frame_origin_edges.png",img_lines_h+img_lines_v);
		imwrite("frame_canny.png",frame_canny);
	}
}

void EdgeInfo::countLines()
{
	int i, j;
	int tot_h, tot_v;
	int cnt_h, cnt_v;
	int ptFlag = 0;

	lines_h.clear();
	lines_h.resize(rows);
	lines_h[0].push_back(0);
	lines_h[0].push_back(cols);

	ptFlag = 0;
	tot_h = 2;
	for( i=1; i<rows-1; i++ )
	{//start from second row to the second last row
		cnt_h = 0;
		for( j=0; j< cols; j++ )
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
	lines_h[rows-1].push_back(0);
	lines_h[rows-1].push_back(cols);


	lines_v.clear();
	lines_v.resize(cols);
	lines_v[0].push_back(0);
	lines_v[0].push_back(rows);
	
	tot_v = 2;
	ptFlag = 0;
	for( j=1; j<cols-1; j++ )
	{//start from second col to the second last col
		cnt_v  = 0;
		for( i=0; i< rows; i++ )
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
	lines_v[cols-1].push_back(0);
	lines_v[cols-1].push_back(rows);

	printf("total horizontal and vertical lines: %d and %d\n", tot_h,tot_v);
}

void EdgeInfo::adjustLines(vector<HVLine>& c_lines, int limit)
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
		if(c_lines[i].level - preLevel <= limit)
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
				c_lines[j].start > end + 25)
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

		cc_lines.push_back(l);
		i = j;
	}

	c_lines = cc_lines;
}

void EdgeInfo::convertToMap(hash_map<int,vector<HVLine>>& lines_map,vector<HVLine>& c_lines)
{
	for(int i=0; i<c_lines.size(); i++)
	{
		if(lines_map.find(c_lines[i].level) != lines_map.end())
		{
			lines_map[c_lines[i].level].push_back(c_lines[i]);
		}
		else
		{
			vector<HVLine> tmp;
			tmp.push_back(c_lines[i]);
			lines_map.insert(pair<int,vector<HVLine>>(c_lines[i].level,tmp));
		}
	}
}

void EdgeInfo::mergeLines(vector<HVLine>& m_lines, vector<vector<int>>& lines)
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

			if(j<lines[i].size()-2 && lines[i][j+2]-lines[i][j+1]< GobalParam::MERGE_LINE_LIMIT)
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

void EdgeInfo::mergeLines()
{
	mergeLines(m_lines_h,lines_h);
	mergeLines(m_lines_v,lines_v);
	//printEdgeMap(m_lines_h,m_lines_v,frame,"m_edge_map.png");
}

void EdgeInfo::classifyLines()
{
	classifyLines(c_lines_h,m_lines_h,10);
	classifyLines(c_lines_v,m_lines_v,10);
}

void groupLines(vector<int> group, vector<HVLine>& m_lines, vector<HVLine>& c_lines,hash_map<int, int>& lenMap, int limit)
{
	int level = 2e15-1;
	int start = 2e15-1;
	int end = 0;

	int m = group[0];
	int n = group[group.size()-1];

	int len = 0;
	for(int j=0; j<group.size(); j++)
	{
		HVLine line = m_lines[group[j]];

		if(lenMap[line.level] > len)
		{
			level =  line.level;
			len = lenMap[line.level];
		}
		start = min(line.start,start);
		end = max(line.end,end);
	}

	if(m_lines[n].level - m_lines[m].level > 2*limit)
	{
 		vector<int> group1 = vector<int>(group.begin(),group.begin()+group.size()/2);
		vector<int> group2 = vector<int>(group.begin()+group.size()/2+1,group.end());
		groupLines(group1,m_lines,c_lines,lenMap,limit);
		groupLines(group2,m_lines,c_lines,lenMap,limit);
		return;
	}

	HVLine line;
	line.start = start;
	line.end = end;
	line.level = level;
	c_lines.push_back(line);
}

void EdgeInfo::classifyLines(vector<HVLine>& c_lines, vector<HVLine>& m_lines, int limit)
{
	int num = m_lines.size();
	Mat corrMat = Mat::zeros(num,num,CV_64FC1);

	hash_map<int,vector<int>> corrMap;
	vector<vector<int>> corrSet;
	for(int i=0; i<num; i++)
	{
		vector<int> tmpSet;
		tmpSet.push_back(i);
		for(int j=i+1; j<num; j++)
		{
			if(abs(m_lines[i].level - m_lines[j].level) < limit)
			{
				int s1 = m_lines[i].start;
				int e1 = m_lines[i].end;
				int s2 = m_lines[j].start;
				int e2 = m_lines[j].end;
				
				double ratio = ((double)(min(e1,e2) - max(s1,s2)))/ min((e1 - s1),(e2-s2));
				if(ratio > 0.7)
				{
					tmpSet.push_back(j);
				}
			}
		}
		corrSet.push_back(tmpSet);
		//corrMap.insert(pair<int,vector<int>>(i,tmpSet));
	}

	vector<vector<int>> groups;
	vector<int> hasGroup;
	for(int i=0; i<corrSet.size(); i++)
	{	
		if(find(hasGroup.begin(),hasGroup.end(),i) != hasGroup.end()) continue;

		vector<int> curSet = corrSet[i];
		hasGroup.push_back(i);
		int maxValue = *max_element(curSet.begin(),curSet.end());
		bool isFirst = true;
		for(int j=i+1; j<corrSet.size(); j++)
		{	
			if(maxValue < j) 
			{
				break;
			}

			vector<int> intersect;
			vector<int> resultSet;
			
			set_intersection( curSet.begin(), curSet.end(), corrSet[j].begin(), corrSet[j].end(),back_inserter(intersect));

			if(intersect.size()>0)
			{
				set_union(curSet.begin(),curSet.end(),corrSet[j].begin(),corrSet[j].end(),
					insert_iterator<vector<int> >( resultSet, resultSet.begin() ));
				curSet = resultSet;
				hasGroup.push_back(j);
			}
		
			maxValue = *max_element(curSet.begin(),curSet.end());
		}
		groups.push_back(curSet);
	}

	for(int i=0; i<groups.size(); i++)
	{
		int level = max(rows,cols) + 1;
		int start = max(rows,cols) + 1;
		int end = 0;

		hash_map<int, int> lenMap;
		for(int j=0; j<groups[i].size(); j++)
		{
			HVLine line = m_lines[groups[i][j]];
			if(lenMap.find(line.level) != lenMap.end())
			{
				lenMap[line.level] += line.end - line.start;
			}
			else
			{
				lenMap[line.level] = line.end - line.start;
			}
		}

		groupLines(groups[i],m_lines,c_lines,lenMap,limit);
		
	}
	std::sort(c_lines.begin(),c_lines.end(),line_sort());
}

void EdgeInfo::cornerDetect(vector<HVLine>& c_lines_h, vector<HVLine>& c_lines_v, int limit)
{
	//const int interval = 20;
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
			if(vlevel>=hstart-limit && vlevel<=hend+limit && 
                hlevel>=vstart-limit && hlevel<=vend+limit)
			{
				Corner cor1,cor2;
				cor1.value = vlevel;
				cor2.value = hlevel;

				int c1 = 0; //corner type 1
				if(hstart+limit<vlevel && vstart+limit<hlevel)
				{
					c1 = 1;
				}

				int c2 = 0;
				if(hend-limit>vlevel && vstart+limit<hlevel)
				{
					c2 = 2;
				}

				int c3 = 0;
				if(hstart+limit<vlevel && vend-limit>hlevel)
				{
					c3 = 4;
				}

				int c4 = 0;
				if(hend-limit>vlevel && vend-limit>hlevel)
				{
					c4 = 8;
				}

				int type = c1 | c2 | c3 | c4;

				cor1.type = type;
				cor2.type = type;
				c_lines_h[i].corners.push_back(cor1);
				c_lines_v[j].corners.push_back(cor2);
			}
		}
	}
}

vector<Corner> getAllCorner(vector<HVLine>& lines, int level)
{
	vector<Corner> result;
	for(int i=0; i<lines.size(); i++)
	{
		if(lines[i].level == level)
		{
			result.insert(result.end(),lines[i].corners.begin(),lines[i].corners.end());
		}
	}

	return result;
}

vector<Rect> EdgeInfo::divideIntoRect(vector<HVLine>& lines_h, vector<HVLine>& lines_v)
{
	int preLevel = -1;

	vector<Rect> rects;
	int sz = lines_v.size()-1;
	for(int i=0; i < sz; i++)
	{
		if(preLevel == lines_v[i].level) continue;

		vector<Corner> cross_v = getAllCorner(lines_v,lines_v[i].level);

		for(int j=0; j<cross_v.size(); j++)
		{
			if((cross_v[j].type & 8) == 0) continue; 

			int v2 = -1;
			for(int k=j+1; k<cross_v.size(); k++)
			{
				int flag = cross_v[k].type & 2;
				if((cross_v[k].type & 2) == 0) continue; 

				int v1 =  lines_v[i].level;
				vector<Corner> cross_h1 = getAllCorner(lines_h, cross_v[j].value);
				vector<Corner> cross_h2 = getAllCorner(lines_h, cross_v[k].value);

				vector<Corner> cross_h3, cross_h4;
				for(int m=0; m<cross_h1.size(); m++)
				{
					int test = cross_h1[m].value;
					int test2 = cross_h1[m].type&4;
					if(cross_h1[m].value>v1 && (cross_h1[m].type&4)>0)
					{
						cross_h3.push_back(cross_h1[m]);
					}
				}

				for(int m=0; m<cross_h2.size(); m++)
				{
					if(cross_h2[m].value>v1 && (cross_h2[m].type&1)>0)
					{
						cross_h4.push_back(cross_h2[m]);
					}
				}
				
				int m,n;
				m=n=0;
				while(m<cross_h3.size() && n<cross_h4.size())
				{
					if(cross_h3[m].value == cross_h4[n].value)
					{
						v2 = cross_h3[m].value;
						break;
					}
					else if(cross_h3[m].value > cross_h4[n].value)
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
					break;
				}
			}
		}

		preLevel = lines_v[i].level;
	}

	return rects;
}


vector<HVLine> EdgeInfo::getLinesByLevel(int level, bool horizontal)
{
	hash_map<int, vector<HVLine>> line_map = horizontal ? line_map_h : line_map_v;
	

	return line_map[level];
}

HVLine EdgeInfo::getMaxLenLine(int level, int gap,int start, int end, bool horizontal)
{	
	hash_map<int, vector<HVLine>> line_map = horizontal ? line_map_h : line_map_v;
	start = start<0 ? 0 : start;
	int tempEnd = horizontal ? cols : rows;
	end = end<0 ? tempEnd : end;

	int maxLen = 0;
	HVLine line;
	line.level = -1;
	line.start = -1;
	line.end = 2e15-1;
	for(int i = level-gap; i<level+gap; i++)
	{
		if(line_map.find(i) == line_map.end()) continue;

		vector<HVLine> lines = line_map[i];
		for(int j=0; j<lines.size(); j++)
		{
			if(lines[j].start < start || lines[j].end > end) continue;

			int len = lines[j].end - lines[j].start;
			if(len > maxLen)
			{
				line = lines[j];
				maxLen = len;
			}
		}
	}

	return line;
}

Mat EdgeInfo::getEdgeMap(vector<HVLine>& lines, int h)
{
	Mat lineMap = Mat::zeros(rows, cols,CV_8U);
	for(int i=0; i<lines.size();i++)
	{
		if(h == 1)
		{
			line(lineMap,Point(lines[i].start,lines[i].level),Point(lines[i].end,lines[i].level),Scalar(255),1);
		}
		else
		{
			line(lineMap,Point(lines[i].level,lines[i].start),Point(lines[i].level,lines[i].end),Scalar(255),1);
		}
	}

	return lineMap;
}

void EdgeInfo::printEdge(Mat& frame, string outputFolder, int seq)
{
	stringstream ss;
	ss << outputFolder << seq <<"_m_edge.png";

	Mat m_edge_h = getEdgeMap(m_lines_h,1);
	Mat m_edge_v = getEdgeMap(m_lines_v,0);
	imwrite(ss.str(),m_edge_h+m_edge_v);

	stringstream ss2;
	ss2<< outputFolder << seq <<"_c_edge.png";
	Mat c_edge_h = getEdgeMap(c_lines_h,1);
	Mat c_edge_v = getEdgeMap(c_lines_v,0);
	imwrite(ss2.str(), c_edge_h+c_edge_v);

	//stringstream ss3;
	//ss3<< outputFolder << seq <<"_canny.png";
	//imwrite(ss3.str(), );

	for(int i=0; i<c_lines_h.size(); i++)
	{
		for(int j=0; j<c_lines_h[i].corners.size(); j++)
		{
			circle( frame, Point(c_lines_h[i].corners[j].value,c_lines_h[i].level), 5,  Scalar(0,0,255), 2, 8, 0 );
		}
	}
}

int EdgeInfo::getHLineLengthBetween(int level, int start, int end,int& left, int& right)
{
	vector<HVLine> lines = c_lines_h;
	
	int len = 0;
	left = max(rows,cols)+1;
	right = -1;
	for(int i=0; i<lines.size(); i++)
	{
		if(level == lines[i].level)
		{
			if(lines[i].start < end && lines[i].end > start)
			{
				len += min(end,lines[i].end)- max(start,lines[i].start);

				left = min(left,lines[i].start);
				right = max(right,lines[i].end);
			}
		}
	}

	return len;
}

int EdgeInfo::getVLineLengthBelow(int vlevel, int upper, int& start, int& end)
{
	const int MAXGAP = 200;
	int preEnd = -1;
	int len = 0;
	start = 2e15-1;
	end = -1;
	for(int i=0; i<c_lines_v.size(); i++)
	{
		if(c_lines_v[i].level == vlevel && c_lines_v[i].end > upper)
		{
			if(preEnd != -1 && c_lines_v[i].start - preEnd > MAXGAP) break;

			len += c_lines_v[i].end - c_lines_v[i].start;

			start = min(c_lines_v[i].start,start);
			end = max(c_lines_v[i].end,end);

			preEnd = c_lines_v[i].end;
		}
	}

	return len;
}

void EdgeInfo::test()
{
	EdgeInfo edgeInfo;
	//Mat frame = imread("d:/temp/img-00009.png",0);
	Mat frame = imread("D:/experimental video/20121019/chenxiao/chenxiao/img-00003.png",0);
	Mat frame_canny;
	Canny( frame, frame_canny, GobalParam::LOWTHRESHOLD, GobalParam::LOWTHRESHOLD * GobalParam::RATIO, GobalParam::KERNELSIZE, false );

	edgeInfo.setRectLineImages(frame_canny);
	edgeInfo.countLines();
	edgeInfo.mergeLines();
	edgeInfo.classifyLines();
	edgeInfo.adjustLines(edgeInfo.c_lines_h);
	edgeInfo.adjustLines(edgeInfo.c_lines_v);
	//edgeInfo.printEdge(frame, );
	
	vector<HVLine> e_lines_h;
	vector<HVLine> e_lines_v;
	edgeInfo.adjustLines(edgeInfo.c_lines_v,10);
	edgeInfo.classifyLines(e_lines_h,edgeInfo.c_lines_h,50);
	edgeInfo.classifyLines(e_lines_v,edgeInfo.c_lines_v,50);

	Mat m1 = edgeInfo.getEdgeMap(e_lines_h,1);
	Mat m2 = edgeInfo.getEdgeMap(e_lines_v,0);

	imwrite("eclipse_edge.png",m1+m2);
	//waitKey(0);
}