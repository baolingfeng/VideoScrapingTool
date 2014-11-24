#include "CProperties.h"

MyProperty::MyProperty(string key, string value)  
{  
	_key = key;
    _value = value;  
	_valid = true;
}  
 
bool MyProperty::isValid()
{
	return _valid;
}

string MyProperty::getKey()
{
	return _key;
}

string MyProperty::getValueString(void)  
{  
    return _value;  
}  
 
int MyProperty::getValueInteger(void)  
{  
	return atoi(_value.c_str());  
}  
 
double MyProperty::getValueDouble()
{
	return atof(_value.c_str());
}

bool MyProperty::getValueBoolean(void)  
{  
	return string("true").compare(_value) == 0;  
} 

MyProperty  MyProperties::getProperty(string name)  
{  
	vector<MyProperty>::iterator it;
	for(it=properties.begin(); it != properties.end(); it++)
	{
		string key = (*it).getKey();
		if(key.compare(name) == 0)
		{
			return (*it);
		}
	}

    return MyProperty();  
} 

int MyProperties::loadFrom(string fileName)  
{  
    int lines = 0;  
	
	ifstream in;
	in.open(fileName,ios::in);

	if(!in.is_open()) return -1;

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
		
		MyProperty p(key,value);
		properties.push_back(p);

		//_map.insert(pair<string,string>(key,value));
        lines++;  
    }  
	
	in.close();

    return lines;  
} 