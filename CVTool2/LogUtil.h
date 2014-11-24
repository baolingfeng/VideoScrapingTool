#pragma once

#include "log4cpp/Category.hh"
#include "log4cpp/PropertyConfigurator.hh"
#include <sstream>

using namespace std;

class LogUtil
{
public:
    LogUtil() {}
	~LogUtil() {}

    static inline int init(string initFileName = "../log4cpp.conf") 
	{
        try {
            log4cpp::PropertyConfigurator::configure(initFileName);
			
        } catch(log4cpp::ConfigureFailure&f) {
            std::cout <<"Configure Problem " << f.what() << std::endl;
            return -1;
        }
        return 0;
    }
 
    static inline void close() {
        log4cpp::Category::shutdown();
    }
 
    /// debug
    static inline void d(string Tag, string msg) {
       log4cpp::Category& sub1 = log4cpp::Category::getInstance(Tag);// GetCategory(Tag);
       
	   LOG4CPP_DEBUG(sub1,msg);
    }

	static inline void i(string Tag, string msg) {
       log4cpp::Category& sub1 = log4cpp::Category::getInstance(Tag);// GetCategory(Tag);
       
	   LOG4CPP_INFO(sub1,msg);
    }
 
    /// debug
    static inline void debug(string msg) {
        d("sub1",msg);
	}

	static inline void debug(stringstream& ss)
	{
		string msg;
		ss>>msg;
		d("sub1",msg);
		ss.clear();
	}

	static inline void info(string msg) {
        i("sub1",msg);
	}

	static inline void info(ostringstream& ss)
	{
		string temp = ss.str();
		i("sub1",ss.str());
		ss.clear();
		ss.str("");
	}
};