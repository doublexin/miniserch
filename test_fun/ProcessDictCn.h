#ifndef PROCESSDICTCN_H
#define PROCESSDICTCN_H

#include <map>
#include <vector>
#include <fstream>
#include <set>

class CnDict
{
    public:
        typedef std::map<std::string , int>::iterator mwviter;
        
       static void  readFile(const std::string &filename,
                std::set<std::string> &excludeWord,std::map<std::string , int > &WordDict_);
       
};

#endif
