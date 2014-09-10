#ifndef CREATINVERTEDINDEX_H
#define CREATINVERTEDINDEX_H


#include <map>
#include <set>
#include <vector>
#include <string>
#include <errno.h>
#include <fstream>
#include "creatStopDict.h"

struct offset
{
    int docid_;
    int beg_;
    int end_;
};



class InvertedIndex
{
    public :
        InvertedIndex( const std::string &docoffset, const std::string &docname, creatStopDict &StopDict);
        ~InvertedIndex();
        void readDoclib(const std::string &docname);
        void readDocoffset(const std::string &docoffset);
        void dealWithDoc();
        void calIDF();
        std::map<std::string, std::map<int, double> > &getIndex();
        std::vector<std::string> &getdoclib();
    private :
        int docCont_;//用于记录一共有多少篇文档
        creatStopDict &StopDict_;
        std::string docoffsetname_;
        std::vector<offset> docoffset_;    
        std::vector<std::string> doclib_;
        std::map<std::string, int>  allWordsFrq_;//所有高频词的集合
        std::map<std::string, std::set<int> > wordsShowId_;//这个map中存放的是每一个单词出现过的文档的位置
        std::map<std::string, double>  everyDocWordsFrq_;
        std::vector<std::map<std::string, double> > everyWordsVec_;//用来存放每一篇的高频词
        std::map<std::string, std::map<int, double> > invertedIndex_;//倒排索引
};

inline std::ifstream & open_file(std::ifstream &in, const std::string &s)
{
    in.close();
    in.clear();
    in.open(s.c_str());

    return in;
}








#endif
