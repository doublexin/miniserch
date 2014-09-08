#include "creatStopDict.h"
#include <stdexcept>
using namespace std;

creatStopDict::creatStopDict(const string &filename)
{

	readStopList(filename);
}

void creatStopDict::readStopList(const string &filename)
{


	//string filename = "stopList.txt";
    //set<std::string> excludeWord_;
      ifstream in;
      in.close();                                         
      in.clear();                                         
      in.open(filename.c_str());                          
      if(!in)
          throw runtime_error("open fial");               
      string line;                                        
                                                                                                                      
      while(getline(in, line))
      {   
         //fun(line); 
          istringstream iss(line);                        
          string word;
          while(iss >> word)
          {
              excludeWord_.insert(word);
          }
      }
      excludeWord_.insert(" ");
      excludeWord_.insert("  ");
      excludeWord_.insert("\r\n");
      excludeWord_.insert("\r");

    in.close();
}

set<std::string> &creatStopDict::getexcludeWord()
{
	return excludeWord_;

}


creatStopDict::~creatStopDict()
{

}
