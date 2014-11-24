#pragma once

#include <stdio.h>
#include <io.h>
#include <direct.h>

using namespace std;

inline bool deleteFolder(string folder)
{
	long handle;  
    struct _finddata_t fileInfo;  

	string pattern = folder + "/*";

	 if ((handle = _findfirst(pattern.c_str(), &fileInfo)) == -1)  
    {  
        cout<<pattern<<" is not directory..."<<endl;  
        return false;  
    }  
	do
	{
		 if(strcmp(fileInfo.name, ".") == 0 || strcmp(fileInfo.name, "..") == 0)  
            continue;  

		string temp = folder + "/" + fileInfo.name;
		if (fileInfo.attrib & _A_SUBDIR)  
        {	//is a directory 
			
            deleteFolder(temp);  
			if(rmdir(temp.c_str()) < 0)
			{
				cout<<"remove folder error"<<endl;
			}
        }  
        else  
        {	//is a normal file  
			remove(temp.c_str());
        }  

	}while (_findnext(handle, &fileInfo) == 0);  

	_findclose(handle);  

	int r = rmdir(folder.c_str());
}
