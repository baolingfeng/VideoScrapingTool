#include "ClsTpl.h"
#include <io.h>
#include <direct.h>
#include <strstream>
#include <qdebug.h>
#include <sys/stat.h>
#include <Windows.h>
#include "util.h"

bool ClsItem::load(string folder, string name)
{
	this->name = name;
	img = cv::imread(folder + "/" + name + ".png");
	img_mask = cv::imread(folder + "/" + name + "_mask.png");

	return true;
}

bool ClsItem::write(string folder)
{
	cv::imwrite(folder + "/" + name + ".png", img);

	if(hasMask())
	{
		cv::imwrite(folder + "/" + name + "_mask.png",img_mask);
	}
	return true;
}

bool ClsRow::loadItems(string folder, int row)
{
	this->index = row;
	stringstream ss;
	ss<<folder<<"/ROW_"<<row;
	string tempFolder = ss.str();
	string pattern = tempFolder + "/*.png";

	_finddata_t fileDir;
	long lfDir;

	if((lfDir=_findfirst(pattern.c_str(),&fileDir)) == -1)
	{
		 return false;
	}
	else
	{
		do
		{
			string tempStr(fileDir.name);

			int idx = tempStr.find(".");
			int idx2 = tempStr.find("mask");
			if(idx2 > 0) continue;

			string itemName = tempStr.substr(0,idx);
			ClsItem item;
			item.load(tempFolder,itemName);

			items.push_back(item);

		}while( _findnext( lfDir, &fileDir ) == 0 );
	}
	_findclose(lfDir);

	return true;
}

bool ClsRow::write(string folder)
{
	stringstream ss;
	ss<<folder<<"/ROW_"<<index;
	string tempFolder = ss.str();

	if(_access(tempFolder.c_str(),0) < 0)
	{
		mkdir(tempFolder.c_str());
	}

	for(int i=0; i<items.size(); i++)
	{
		items[i].write(tempFolder);
	}

	return true;
}

bool ClsRow::del(string folder)
{
	stringstream ss;
	ss<<folder<<"/ROW_"<<index;
	string tempFolder = ss.str();

	if(_access(tempFolder.c_str(),0) >= 0)
	{
		if(!deleteFolder(tempFolder)) return false;
	}

	return true;
}

bool ClsRow::renameFolder(string folder, int newIndex)
{
	stringstream ss;
	ss<<folder<<"/ROW_"<<index;
	string oldFolder = ss.str();

	stringstream ss2;
	ss2<<folder<<"/ROW_"<<newIndex;
	string newFolder = ss2.str();

	index = newIndex;

	rename(oldFolder.c_str(),newFolder.c_str());

	return true;
}

void ClsApp::initRows(int rowNum)
{
	this->rowNum = rowNum;

	for(int i=1; i<=rowNum; i++)
	{
		ClsRow row(i);
		rows.push_back(row);
	}
}

bool ClsApp::loadRows(string folder, string name)
{
	this->name = name;
	string configFile = folder + "/" + name + "/" + "config.txt";
	FILE* fid = fopen(configFile.c_str(),"r");
	if(fid == NULL)
	{
		cout<<"open config file failed"<<endl;
		return false;
	}

	int value;
	char tmp[256];
	while(!feof(fid))
	{
		fscanf(fid,"%s %d",tmp,&value);

		if(strcmp(tmp,"ROWS") == 0)
		{
			rowNum = value;
		}
		else if(strcmp(tmp,"CONTENT") == 0)
		{
			contentRow = value;
		}
		else if(strcmp(tmp,"URL") == 0)
		{
			urlRow = value;
		}
	}
	fclose(fid);

	for(int i=1; i<=rowNum; i++)
	{
		ClsRow row;
		row.loadItems(folder + "/" + name, i);

		rows.push_back(row);
	}

	return true;
}

bool ClsApp::writeConfig(string folder)
{
	string configFile = folder + "/" + name + "/config.txt";
	//delete if exists
	if(_access(configFile.c_str(),0) >= 0)
	{
		remove(configFile.c_str());
	}

	FILE* fid = fopen(configFile.c_str(),"w");
	fprintf(fid, "ROWS %d\n", rowNum);
	fprintf(fid, "CONTENT %d\n", contentRow);
	fprintf(fid, "URL %d\n", urlRow);
	fclose(fid);

	return true;
}

bool ClsApp::write(string folder)
{
	string tempFolder = folder + "/" + name;

	if(_access(tempFolder.c_str(),0) < 0)
	{
		mkdir(tempFolder.c_str());
	}

	writeConfig(folder);

	for(int i=0; i<rows.size(); i++)
	{
		rows[i].write(tempFolder);
	}

	return true;
}

bool ClsApp::deleteARow(string folder,int rowIndex)
{
	string tempFolder = folder + "/" + name;
	ClsRow row = rows[rowIndex];
	row.del(tempFolder);

	for(int i=rowIndex+1; i<rowNum; i++)
	{
		rows[i].renameFolder(tempFolder, i);
	}

	rowNum--;
	std::vector<ClsRow>::iterator it = rows.begin();
	rows.erase(it+rowIndex);

	writeConfig(folder);

	return true;
}

bool ClsApp::addRow(string folder, int rowIndex)
{
	string tempFolder = folder + "/" + name;

	for(int i=rowNum-1; i>=rowIndex; i--)
	{
		rows[i].renameFolder(tempFolder,i+2);
	}

	ClsRow row(rowIndex+1);
	row.write(tempFolder);

	std::vector<ClsRow>::iterator it = rows.begin();
	rows.insert(it+rowIndex,row);
	rowNum++;
	
	writeConfig(folder);

	return true;
}

bool ClsTpl::loadApp(string folder, string name)
{
	string tempFolder = folder + "/" + name;
	string pattern = tempFolder + "/*";

	_finddata_t fileDir;
	long lfDir;

	if((lfDir=_findfirst(pattern.c_str(),&fileDir)) == -1)
	{
		 return false;
	}
	else
	{
		do
		{
			string tempStr(fileDir.name);
			if(tempStr == "." || tempStr == "..") continue;

			ClsApp app;
			app.loadRows(tempFolder,fileDir.name);
			//fileDir.name

			apps.push_back(app);
		}while( _findnext( lfDir, &fileDir ) == 0 );
		
	}
	_findclose(lfDir);

	this->name = name;

	return true;
}

bool ClsTpl::write(string folder)
{
	string tempFolder = folder + "/" + name;
	if(_access(tempFolder.c_str(),0) < 0)
	{
		mkdir(tempFolder.c_str());
	}

	for(int i=0; i<apps.size(); i++)
	{
		apps[i].write(tempFolder);
	}

	return true;
}

bool ClsTpl::delTemplate(string folder)
{
	string tempFolder = folder + "/" + name;
	//string cmd = "rmdir /S /Q " + tempFolder;
	
	if(_access(tempFolder.c_str(),0) >= 0)
	{
		deleteFolder(tempFolder);
	}

	return true;
}