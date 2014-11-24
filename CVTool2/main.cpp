#include <stdio.h>
#include <io.h>
#include "WinTpl.h"
#include "ObjMan.h"

void loadTemplate(vector<WinTpl>& templates,char* username)
{
	vector<char*> tplNames;//2
	vector<char*> tplPaths;//2
	
	char* basePath = "../data/";
	char* APPList[] = {"TaskBar", "Eclipse","Chrome","Word","360Browser","CHM","Explorer","IE","Firefox"};
	int len = sizeof(APPList)/sizeof(char*);

	for(int i=0; i<len; i++)
	{
		char* folder = new char[MAXSLEN];
		strcpy(folder,basePath);
		strcat(folder,APPList[i]);
		strcat(folder,"/APP_");
		strcat(folder,username);
		strcat(folder,"/");
		if(_access(folder,0) != -1)
		{
			tplNames.push_back(APPList[i]); tplPaths.push_back(folder);
		}
	}

	for(int i=0; i<tplNames.size(); i++)
	{
		WinTpl tpl(tplNames[i]);
		tpl.loadTemplate(tplPaths[i],tplNames[i]);
		templates.push_back(tpl);
	}
}

int main_0()
{
	FILE *stream ;
	if((stream = freopen("stdout.txt", "w", stdout)) == NULL)
			exit(-1);
	setbuf(stdout, NULL);

	//char fileFolder[MAXSLEN] = "D:/videos_diff/";
	//char fileFolder[MAXSLEN] = "D:/temp/";
	//char fileFolder[MAXSLEN] = "D:/experimental video/20121019/chenbihuan/14_2_chenbihuan/";
	//char fileFolder[MAXSLEN] = "D:/experimental video/20121019/xieyi/xieyi/";
	//  char fileFolder[MAXSLEN]  = "D:/experimental video/20120911/fukun/fukun/";
	//char fileFolder[MAXSLEN]  = "D:/temp/";
	//char fileFolder[MAXSLEN] = "D:/experimental video/20121019/qiucheng/qiucheng/";
	char fileFolder[MAXSLEN] = "D:/experimental video/20120926/LinYun/linyun_2/";
	//char fileFolder[MAXSLEN] = "D:/experimental video/20121019/xieyi/xieyi/";


	char fileName[MAXSLEN];
	vector<WinTpl> templates;
	WinTpl::loadTemplate(templates,"LinYun");
	
	int frame_seq_begin = 1;
	int frame_num = 1;
	
	for(int cur_frame=frame_seq_begin; cur_frame<frame_seq_begin+frame_num; cur_frame++)
	{
		fprintf(stderr,"begin to process %d-th frame\n", cur_frame);
		sprintf(fileName,"img-%05d", cur_frame);
		ObjMan objMan;
		objMan.setOutputFolder("../CropOut/test/");
		
		objMan.setTemplates(templates);

		time_t t1;
		t1 = time(NULL);
		objMan.feedFrame(fileFolder,fileName,cur_frame);
		if(objMan.frame_des.empty()) 
			continue;
		fprintf(stderr,"time for reading frame: %d\n", time(NULL)-t1);
		t1 = time(NULL);

		objMan.collectEdgeMapLines(10);
		fprintf(stderr,"time for processing frame edge info: %d\n", time(NULL)-t1);
		t1 = time(NULL);

		objMan.matchTplCompsToFrame();
		fprintf(stderr,"time for matching compenents to frame: %d\n", time(NULL)-t1);
		t1 = time(NULL);

		objMan.test();
		fprintf(stderr,"time for crop frame: %d\n\n", time(NULL)-t1);

		objMan.clear();
	}
}