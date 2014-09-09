#ifndef SERCH_H
#define SERCH_H
#include "creatStopDict.h"
#include "creatInvertedIndex.h"

class Serch
{
public:
	Serch(InvertedIndex &tinvertedIndex, creatStopDict &StopDict);
	void dealWithCin();
	void calQueryWordTF(const std::map<std::string, int> &twordsMap1);
	~Serch();
private:
	InvertedIndex &invertedIndex_;
	creatStopDict &StopDict_;
	std::map<std::string, double> queryWords_;


	/* data */
};

#endif