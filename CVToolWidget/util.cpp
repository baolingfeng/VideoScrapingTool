#include "util.h"

void trim_blf(string& str)
{
  string::size_type pos = str.find_last_not_of(' ');
  if(pos != string::npos) {
    str.erase(pos + 1);
    pos = str.find_first_not_of(' ');
    if(pos != string::npos) str.erase(0, pos);
  }
  else str.erase(str.begin(), str.end());
}

vector<string> split_blf(string& str, string delim)
{
    string::size_type start = 0;
    string::size_type end = str.find(delim);
	vector<string> result;
    while (end != std::string::npos)
    {
        //std::cout << s.substr(start, end - start) << std::endl;
		result.push_back(str.substr(start,end-start));
		start = end + delim.length();
        end = str.find(delim, start);
    }

	result.push_back(str.substr(start,end));

	return result;
}

string join_blf(vector<string> sarray, string sep)
{
	std::stringstream ss;
	for(size_t i = 0; i < sarray.size(); ++i)
	{
	  if(i != 0)
		ss << sep;
	  ss << sarray[i];
	}
	std::string s = ss.str();

	return ss.str();
}

void replaceAll_blf(string& str, string from,string to)
{
	string::size_type start_pos = 0;
	while((start_pos = str.find(from, start_pos)) != std::string::npos) {
         str.replace(start_pos, from.length(), to);
         start_pos += to.length(); // ...
	}
}




char easytolower(char in){
  if(in<='Z' && in>='A')
    return in-('Z'-'z');
  return in;
} 

void toLowcase_blf(string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}