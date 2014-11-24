#pragma once

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <hash_map>
#include <algorithm>

using namespace std;

class MyProperty  
{  
private:  
	string _key;
    string _value;
	bool _valid;
public:  
	MyProperty() {_valid=false;}
    MyProperty(string key,string value);  
	string getKey();
	string getValueString(void);  
    int getValueInteger(void);  
    bool getValueBoolean(void); 
	double getValueDouble(void);
	bool isValid();
};

class MyProperties  
{  
private:  
	vector<MyProperty> properties;
    //hash_map<string,string> _map;   
public:  
    MyProperty getProperty(string name);  
    
	MyProperties(void) {}
	~MyProperties(void) {};  
    int loadFrom(string fileName);  
}; 