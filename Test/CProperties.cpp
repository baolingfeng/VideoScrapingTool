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
        //��ȡ���е�һ��  
        char line[256];  
        in.getline(line, 255);  
          
        string s = line;  
 
        //�հ��У�����  
        if(s.empty())  
            continue;  
 
        //#Ϊע�ͱ�ǣ�����  
        if(s[0] == '#')  
            continue;  
 
        //������=������  
        int i = s.find("=");  
        if(i < 0)  
            continue;  
 
        //��ֳ�key=value  
        string key = s.substr(0, i);  
        string value = s.substr(i + 1);  
		
		CProperty p(key,value);
		properties.push_back(p);

		//_map.insert(pair<string,string>(key,value));
        lines++;  
    }  
 
    return lines;  
} 