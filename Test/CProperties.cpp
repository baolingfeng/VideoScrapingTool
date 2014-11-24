#include "CProperties.h"

CProperty::CProperty(string key, string value)  
{  
	_key = key;
    _value = value;  
	_valid = true;
}  
 
bool CProperty::isValid()
{
	return _valid;
}

string CProperty::getKey()
{
	return _key;
}

string CProperty::getValueString(void)  
{  
    return _value;  
}  
 
int CProperty::getValueInteger(void)  
{  
	return atoi(_value.c_str());  
}  
 
bool CProperty::getValueBoolean(void)  
{  
	return string("true").compare(_value) == 0;  
} 

CProperty  CProperties::getProperty(string name)  
{  
	vector<CProperty>::iterator it;
	for(it=properties.begin(); it != properties.end(); it++)
	{
		string key = (*it).getKey();
		if(key.compare(name) == 0)
		{
			return (*it);
		}
	}

    return CProperty();  
} 

int CProperties::loadFrom(string fileName)  
{  
    int lines = 0;  
	
	ifstream in;
	in.open(fileName,'r');

    while(!in.eof())  
    {  
        //读取其中的一行  
        char line[256];  
        in.getline(line, 255);  
          
        string s = line;  
 
        //空白行，跳过  
        if(s.empty())  
            continue;  
 
        //#为注释标记，跳过  
        if(s[0] == '#')  
            continue;  
 
        //不包含=，跳过  
        int i = s.find("=");  
        if(i < 0)  
            continue;  
 
        //拆分成key=value  
        string key = s.substr(0, i);  
        string value = s.substr(i + 1);  
		
		CProperty p(key,value);
		properties.push_back(p);

		//_map.insert(pair<string,string>(key,value));
        lines++;  
    }  
 
    return lines;  
} 