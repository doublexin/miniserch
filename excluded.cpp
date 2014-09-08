#include "excluded.h"
#include "ProcessDictCn.h"
#include "ProcessDictEn.h"
#include <stdexcept>
using namespace std;


/*excluded::excluded(const string &filename)
{
    readfile(filename);
}*/

void excluded::readfile(const string &filename)
{
    ifstream in;
    in.close();
    in.clear();
    in.open(filename.c_str());
    if(!in)
        throw runtime_error("open fial");
    string line;
  
    while(getline(in, line))
    {   
       
        istringstream iss(line);
        string word;
        while(iss >> word)
        {
            excludeWord_.insert(word);
        }
    }
   
    in.close(); 
    EnDict::readFile("testen.txt", WordDict_);
    CnDict::readFile("list.txt", excludeWord_
            , WordDict_);

}

map<string, int> &excluded::getmap()
{
    return WordDict_;
}

set<string> &excluded::getset()
{
    return excludeWord_;      
}
