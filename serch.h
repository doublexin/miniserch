#ifndef SERCH_H
#define SERCH_H
#include "creatStopDict.h"
#include "creatInvertedIndex.h"
#include <queue>


struct Similar
{
    int docid_;
    double sim_;
};

inline bool operator < (const Similar &a, const Similar &b)
{
    return a.sim_ < b.sim_;
}


struct Page
{
    int id_;
    std::string title_;
    std::string content_;

};

/*struct offset
{
    int docid_;
    int beg_;
    int end_;
};*/


class Serch
{
public:
	Serch(InvertedIndex &tinvertedIndex, 
		creatStopDict &StopDict,
		std::string pagelib,
		std::string offsetdat);
	void dealWithCin();
	//void ReadOffset();
	void calQueryWordTF(const std::map<std::string, int> &twordsMap1);
	void startQuery();
	double findWeight(std::string s, int id);
	void calSimiler();
	void returnRES(Similar similar);
	~Serch();
private:
	InvertedIndex &cinvertedIndex_;
	creatStopDict &StopDict_;
	std::map<std::string, double> queryWords_;//查询单词的权重
	std::map<std::string, std::map<int, double> > queryIndex_;//去重后的索引
	std::priority_queue<Similar> q_;//优先级队列
	std::set<int> queryId_;//用于存放公共词的id容器
	//保存网页库路径
    std::string pagelib_;
    //保存偏移量路径
    std::string offsetdat_;
    //保存文件id和偏移量
    std::vector<offset> docoffset_;


	/* data */
};

#endif