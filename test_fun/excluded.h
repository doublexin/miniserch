#ifndef EXCLUDED_H
#define EXCLUDED_H


#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>

class excluded
{
   
    public:
        friend class correctWord;
//        excluded(const std::string &filname);
        void readfile(const std::string &filename);
      
        std::map<std::string, int > &getmap();
        std::set<std::string> &getset();
    private:
        std::set<std::string> excludeWord_;
        std::map<std::string , int > WordDict_;        
};

#endif
