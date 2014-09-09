#include "serch.h"

#include <iostream>
#include <vector>
#include <cstdlib>      //jieba
#include <cstdio>
#include "include/MixSegment.hpp"//jieba end

//jieba
using namespace CppJieba;  
const char * const dict_path =  "dict/jieba.dict.utf8";
const char * const model_path = "dict/hmm_model.utf8";

using namespace std;

#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)


Serch::Serch(InvertedIndex &tinvertedIndex, creatStopDict &StopDict)
	:invertedIndex_(tinvertedIndex),
	StopDict_(StopDict)
	{

	}
void Serch::dealWithCin()
{
	MixSegment segment(dict_path, model_path);
    vector<string> words;
    map<string, int> twordsMap;
    words.clear();
    string lines;
    string word;
    cout << "cout " << endl;
    while(cin >> lines)
    {
    	cout << "cin " << endl;
    	twordsMap.clear();
    	segment.cut(lines, words);
    	for(int ix =0; ix != words.size(); ++ix)
        {
            if(StopDict_.getexcludeWord().count(words[ix]))
                continue; 
            else  
            {   
                ++twordsMap[words[ix]];
                
            }
        }
        calQueryWordTF(twordsMap);
    }

//此处为了测试queryWords_
   /* for(map<string, double>::iterator it = queryWords_.begin();
    	it != queryWords_.end(); ++it)
    {

    	cout << " word = " << it -> first 
    	     << " tf = "   << it -> second << endl; 
    }*/
}

void Serch::calQueryWordTF(const map<string, int> &twordsMap1)
{
	for(map<string, int>::const_iterator it = twordsMap1.begin();
		it != twordsMap1.end(); ++it)
	{
		double tf = 0;
		tf = double(it->second) / double(twordsMap1.size());
		queryWords_.insert(make_pair(it->first, tf));
	}
}

Serch::~Serch()
{

}





