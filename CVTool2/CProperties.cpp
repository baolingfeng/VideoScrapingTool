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
		
		MyProperty p(key,value);
		properties.push_back(p);

		//_map.insert(pair<string,string>(key,value));
        lines++;  
    }  
	
	in.close();

    return lines;  
} 