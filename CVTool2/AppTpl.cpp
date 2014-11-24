#include "AppTpl.h"
#include "ObjMan.h"

AppTpl::AppTpl(void)
{
	//templateName = name;
	contentRow = -1;
	urlRow = -1;

	surf_inst.hessianThreshold = GobalParam::HESSIAN_THRESHOLD;
	surf_inst.nOctaves         = GobalParam::OCTAVES;
	surf_inst.nOctaveLayers    = GobalParam::OCTAVELAYERS;
	surf_inst.extended         = GobalParam::EXTENDED;
	surf_inst.upright          = GobalParam::UPRIGHT;

	maxMenuHeight = 90; //defalut
	firstDetect = false;
}

AppTpl::~AppTpl(void)
{
}

string AppTpl::BASE_FOLDER = "../tpls/";

bool AppTpl::loadTemplate(string path, string name)
{
	FILE* fconfig;

	if(path.at(path.length()-1) != '/')
	{
		path += '/';
	}

	string configFile = path + "config.txt";
	ifstream ifs(configFile);
	if(!ifs.is_open())
	{
		cout<<"config file open error: "<<name<<endl;
		return false;
	}
	templateName = name;

	string line;
	while(getline(ifs, line))
	{
		trim(line);
		if(line.size() <=0) continue;

		vector<string> temp = split(line, " ");
		if(temp[0] == "ROWS")
		{
			rowNum = stringToNumber<int>(temp[1]);
		}
		else if(temp[0] == "CONTENT")
		{
			contentRow = stringToNumber<int>(temp[1]);
		}
		else if(temp[0] == "URL")
		{
			urlRow = stringToNumber<int>(temp[1]);
		}
		else if(temp[0] == "MAX_MENU_HEIGHT")
		{
			maxMenuHeight = stringToNumber<int>(temp[1]);
		}
		else if(temp[0] == "FIRST_DETECT")
		{
			firstDetect = (temp[1] == "TRUE");
			//int num = stringToNumber<int>(temp[2]);
			getline(ifs, line);
			firstDetectItems = split(line," ");
		}
	}

	/*
	if((fconfig=fopen(configFile.c_str(),"r")) == NULL)
	{
		cout<<"config file open error: "<<name<<endl;
		return false;
	}

	while(!feof(fconfig))
	{
		char strType[MAXSLEN];
		int value;
		fscanf(fconfig,"%s %d", strType, &value);
		if(strcmp(strType,"ROWS") == 0)
		{
			rowNum = value;
		}
		else if(strcmp(strType, "CONTENT") == 0)
		{
			contentRow = value;
		}
		else if(strcmp(strType, "URL") == 0)
		{
			urlRow = value;
		}
		else if(strcmp(strType, "MAX_MENU_HEIGHT") == 0)
		{
			maxMenuHeight = value;
		}
	}
	fclose(fconfig);
	*/
	int i,j;

	for(i=1; i<=rowNum;i++)
	{
		string rowConfig;
		string rowFolder;

		ostringstream convert;
		convert<<path<<"ROW_"<<i<<"/";
		rowFolder = convert.str();
		convert.clear();

		vector<AppItem> rowItems;
		//scan the row folder
		_finddata_t fileInfo;
		long lfDir;
		if((lfDir=_findfirst((rowFolder + "*").c_str(),&fileInfo)) == -1)
		{
			LogUtil::debug(rowFolder+" cannot be found");
		}
		else
		{
			do{
				string fileName(fileInfo.name);
				int idx = fileName.find(".png");
				int idx2 = fileName.find("mask.png");
				if(idx < 0 || idx2 >= 0) continue;

				string itemName = fileName.substr(0,idx);
				string itemFile = rowFolder+fileInfo.name;

				Mat item = imread(itemFile,0);
				vector<KeyPoint> item_kps;
				Mat item_des;
				Mat item_edge;

				Canny(item, item_edge, GobalParam::LOWTHRESHOLD, 
				GobalParam::LOWTHRESHOLD * GobalParam::RATIO, GobalParam::KERNELSIZE, false );

				FAST(item, item_kps,GobalParam::FAST_CORNER_THRESH, GobalParam::FAST_CORNER_THRESH_SUPRESS);
				surf_inst(item, Mat(), item_kps, item_des, true);

				if(item_des.empty())
				{
					continue;
				}

				AppItem appItem;
				appItem.name = fileName.substr(0,fileName.find("."));
				appItem.row = i;
				appItem.item = item;
				appItem.item_kps = item_kps;
				appItem.item_des = item_des;
				appItem.item_edge = item_edge;
				
				string itemMaskFile = rowFolder + itemName + "_mask.png";
				Mat itemMask = imread(itemMaskFile,0);
				if(!itemMask.empty())
				{
					appItem.item_msk = itemMask;
				}

				rowItems.push_back(appItem);
			}while( _findnext( lfDir, &fileInfo ) == 0 );
		}

		items.push_back(rowItems);
	}
	return true;
}

void AppTpl::calcFrameDFTs(Mat& frame_gray_in, Mat& F_I, Mat& F_I2)
{
	Mat frame_gray;
	im2double(frame_gray_in, frame_gray);

	F_I  = Mat::zeros(dft_sz_com, frame_gray.type());
    F_I2 = Mat::zeros(dft_sz_com, frame_gray.type());

	Mat M_I2 = frame_gray.mul(frame_gray);
	Mat roiF_I (F_I,  Rect(0,0,frame_gray.cols,frame_gray.rows));
    frame_gray.copyTo(roiF_I);
    Mat roiF_I2(F_I2, Rect(0,0,frame_gray.cols,frame_gray.rows));
    M_I2.copyTo(roiF_I2);

	dft(F_I,  F_I,  0, frame_gray.rows);
    dft(F_I2, F_I2, 0, frame_gray.rows);
}

void AppTpl::setAlphaTplMatchingParameters(Size frame_sz, double dft_thr_com_in)
{
	vector<AppItem*> compList;
	dft_tpl_sz_com.width  = 0;
	dft_tpl_sz_com.height = 0;

	for(int i=0; i<items.size(); i++)
	{
		for(int j=0; j<items[i].size(); j++)
		{
			if(items[i][j].isUsingMask() == 1)
			{
				compList.push_back(&items[i][j]);
			}
		}
	}

	dft_tpl_sz_com.width  = 0;
	dft_tpl_sz_com.height = 0;
	for(int i=0; i<compList.size(); i++)
	{
		compList[i]->tightenMask();
		if(compList[i]->msk_rect_tight.width > dft_tpl_sz_com.width )
		{
			dft_tpl_sz_com.width  = compList[i]->msk_rect_tight.width;
		}
		if( compList[i]->msk_rect_tight.height > dft_tpl_sz_com.height )
		{
			dft_tpl_sz_com.height = compList[i]->msk_rect_tight.height;
		}
	}

	
	dft_sz_com.width  = getOptimalDFTSize(frame_sz.width  + dft_tpl_sz_com.width  - 1);
	dft_sz_com.height = getOptimalDFTSize(frame_sz.height + dft_tpl_sz_com.height - 1);

	dft_thr_com = dft_thr_com_in;
	
	for(int i=0; i<compList.size(); i++ )
	{
		compList[i]->initDFTParameters(dft_sz_com, dft_tpl_sz_com, dft_thr_com);
	}
}

vector<DetectedItem> AppTpl::matchTplComp(Mat& tar_img_gray,vector<KeyPoint>& tar_kps, Mat& tar_des,Mat& tar_edge)
{
	ostringstream ss;
	ss<<"matching application template: "<<templateName; LogUtil::info(ss); ss.clear();

	int i,j;
	if( dft_sz_com.width <= 0 )
	{
		setAlphaTplMatchingParameters(Size(tar_img_gray.cols, tar_img_gray.rows));
	}

	Mat F_I,F_I2;
	if(GobalParam::G_USING_MASK)
	{
		calcFrameDFTs(tar_img_gray, F_I, F_I2);
	}

	vector<DetectedItem> detects;
	int round = firstDetect ? 2 : 1;
	for(int k=0; k<round; k++)
	{
		if(k == 1 && detects.size()<=0) break;
		
		for(i=0; i<items.size(); i++)
		{
			vector<Rect> temp;
			for(j=0; j<items[i].size();j++)
			{	
				if(k == 0 && firstDetect &&
					std::find(firstDetectItems.begin(), firstDetectItems.end(),items[i][j].name) == firstDetectItems.end())
				{
					continue;
				}
				else if(k == 1 && 
					std::find(firstDetectItems.begin(), firstDetectItems.end(),items[i][j].name) != firstDetectItems.end())
				{
					continue;
				}

				ss<<"match item image: (row:"<<items[i][j].row<<") "<<items[i][j].name; LogUtil::info(ss); ss.clear();
				if(items[i][j].isUsingMask() && GobalParam::G_USING_MASK)
				{
					LogUtil::info("matching method is template match with mask");
					temp = items[i][j].match(tar_img_gray, F_I, F_I2 );
				}
				else
				{
					temp = items[i][j].match(tar_img_gray,tar_kps,tar_des,tar_edge);
				}

				if(temp.size()>0)
				{
					ss<<items[i][j].name<<" is detected with number: "<<temp.size(); LogUtil::info(ss);
					for(int k=0; k<temp.size(); k++)
					{
						DetectedItem item;
						item.itemName = items[i][j].name;
						item.row = items[i][j].row;
						item.rect = temp[k];
						detects.push_back(item);
					}	
				}
			}
		}
	}
	
	return detects;
}

void AppTpl::loadTemplatesFromFS(vector<AppTpl>& tpls, string path)
{
	if(path.at(path.length()-1) != '/')
	{
		path += '/';
	}
	string pattern = path + "/*";

	_finddata_t fileInfo;
	long lfDir;
	if((lfDir=_findfirst(pattern.c_str(),&fileInfo)) == -1)
	{
		LogUtil::debug(path+" cannot be found");
		//cout<<"not a fold"<<endl;
	}
	else
	{
		do{
			string fileName(fileInfo.name);
			if(fileName == "." || fileName == "..") continue;

			if (fileInfo.attrib & _A_SUBDIR) 
			{
				AppTpl tpl;
				if(tpl.loadTemplate(path+fileInfo.name,fileInfo.name))
				{
					tpls.push_back(tpl);
				}
			}

		}while( _findnext( lfDir, &fileInfo ) == 0 );
	}
}

void AppTpl::loadTemplates(vector<AppTpl>& tpls, string name)
{
	if(BASE_FOLDER.at(BASE_FOLDER.length()-1) != '/')
	{
		BASE_FOLDER += '/';
	}
	string pattern = BASE_FOLDER + name + "/*";

	_finddata_t fileInfo;
	long lfDir;
	if((lfDir=_findfirst(pattern.c_str(),&fileInfo)) == -1)
	{
		LogUtil::debug(BASE_FOLDER+" cannot be found");
		//cout<<"not a fold"<<endl;
	}
	else
	{
		do{
			string fileName(fileInfo.name);
			if(fileName == "." || fileName == "..") continue;

			if (fileInfo.attrib & _A_SUBDIR) 
			{
				AppTpl tpl;
				if(tpl.loadTemplate(BASE_FOLDER+name+"/"+fileInfo.name,fileInfo.name))
				{
					tpls.push_back(tpl);
				}
			}

		}while( _findnext( lfDir, &fileInfo ) == 0 );
	}
}

//test function
void AppTpl::test()
{
	vector<AppTpl> tpls;
	loadTemplates(tpls,"standard");

}
