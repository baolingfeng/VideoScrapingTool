#pragma once

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <hash_map>
#include <algorithm>

using namespace std;

class CProperty  
{  
private:  
	string _key;
    string _value;
	bool _valid;
public:  
	CProperty() {_valid=false;}
    CProperty(string key,string value);  
	string getKey();
	string getValueString(void);  
    int getValueInteger(void);  
    bool getValueBoolean(void);  
	bool isValid();
};

class CProperties  
{  
private:  
	vector<CProperty> properties;
    //hash_map<string,string> _map;   
public:  
    CProperty getProperty(string name);  
    ~CProperties(void);  
    int loadFrom(string fileName);  
}; 