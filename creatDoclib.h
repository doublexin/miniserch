#ifndef CREATDOCLIB_H
#define CREATDOCLIB_H 

#include "creatStopDict.h"
//#include "Rio.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <vector>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <map>
#include <queue>
#include <cstdio>
#include "include/MixSegment.hpp"
#include <cstdlib>
#include <set>
#include <fstream>





//========结巴=====
using namespace CppJieba;  
const char * const dict_path =  "dict/jieba.dict.utf8";
const char * const model_path = "dict/hmm_model.utf8";

//=================


//========优先级队列=======
struct qtopten
{
    string word_;
    int frequence_;

    friend bool operator< (const qtopten &va, const qtopten &vb);
   
};


inline bool operator< (const qtopten &va, const qtopten &vb)

{    
    return va.frequence_ < vb.frequence_;
}
//=========================



//=====索引的结构体======
struct docinfo
{
	int docid_;
    off_t origin_;
    long int length_;
};

//========================


class DocLib
{
public:
	DocLib(creatStopDict &StopDict, const string &dirname);
	~DocLib();
	//void readfile(const std::string filename);
	void createdoclib();
	void getTop10(std::map<std::string, int> &HighFreWord);
	void fun(std::string &s);//用于过滤\r\n
	void buileFile();//建库
	void DuplicateRemoval();//去重
	
private:
	//停止词的对象
	creatStopDict &StopDict_;
	//用来读取文件夹下的相关资料
	void ReadPageDir(const std::string &dirname);
	std::map<std::string, int> HighFreWord_;//定义的当前的高频词
	std::map<std::string , struct docinfo> libindex_;//定义索引 
	std::vector<std::string> doclib_;//定义网页库
	std::vector<std::set<std::string> > allTopTenWords_;//所有文章的top10的集合
	std::string dirname_;//定义的用来存文件的目录
    std::vector<std::string> filenames_;//文件夹的文件名字

	/* data */
};
inline std::ifstream & open_file(std::ifstream &in, const std::string &s)
{
    in.close();
    in.clear();
    in.open(s.c_str());

    return in;
}





#endif
