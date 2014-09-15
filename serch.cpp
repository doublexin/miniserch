#include "serch.h"

#include <iostream>
#include <vector>
#include <cstdlib>      //jieba
#include <cstdio>
#include "include/MixSegment.hpp"//jieba end

#include <sys/types.h>   //open
#include <sys/stat.h>
#include <fcntl.h>  
#include <unistd.h>
#include <stdio.h>
#include <sstream>
#include <fstream>

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


Serch::Serch(InvertedIndex &tinvertedIndex, 
             creatStopDict &StopDict,
             string pagelib,
             string offsetdat)

	:cinvertedIndex_(tinvertedIndex),
	StopDict_(StopDict),
    pagelib_(pagelib),
    offsetdat_(offsetdat)

	{
       // ReadOffset();
	}

/*void Serch::ReadOffset()
{
    struct offset node;
    ifstream fin;
    if (!open_file(fin, "offset.dat"))
        ERR_EXIT("open offset error");
    while (fin >> node.docid_)
    {
        fin >> node.beg_;
        fin >> node.end_;
        docoffset_.push_back(node);
    }
}*/

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
    	cout << "Input your words ：" << endl;
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
    cout << "end dealWithCin" << endl;

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
//检查查询词是否在索引里面
void Serch::startQuery()
{

    for(map<string, double>::iterator it = queryWords_.begin();
        it != queryWords_.end(); ++it)
    {
        map<string, map<int, double> >::iterator ix = cinvertedIndex_.getIndex().find(it->first);

        if(ix != cinvertedIndex_.getIndex().end())
        {
            queryIndex_.insert(make_pair(ix->first, ix->second));
        }
        else
            continue;
        
    }


    //把Id去重 
    //set<int> queryId_;
    queryId_.clear();
    map<string, map<int, double> >::iterator it = queryIndex_.begin();
    for(; it != queryIndex_.end(); ++it)
    {
        map<int, double>::iterator ix = (it->second).begin();
        for(; ix != (it->second).end() ; ++ix)
        {
            queryId_.insert(ix->first);
        }
    }

    //把索引里面的多余的去掉
   /* map<string, map<int, double> >::iterator itk = queryIndex_.begin();
    for(; itk != queryIndex_.end(); ++itk)
    {
        map<int, double>::iterator ixk = (it->second).begin();

        for(; ixk != (it->second).end() ; ++ ixk)
        {
            set<int>::iterator ii = queryId_.begin();
            for(; ii != queryId_.end(); ++ii)
            {
                if(*ii == ixk->first)
                    continue;
                else
                {
                    (itk->second).erase(*ii);
                }
            }
        }
    }*/


    //此处测试去重后的索引

for (map<string, map<int , double> >::iterator i = queryIndex_.begin();
    i != queryIndex_.end() ; ++i)
    {
        cout << "word " << i->first;
        for (map<int, double>::iterator j = i->second.begin();
            j != i->second.end(); ++ j)
        {
            cout << " id " << j->first << " weight " << j->second ;
        }
        cout << endl;
    }


    calSimiler();
    returnRES(q_.top());

}



double Serch::findWeight(string s, int id)
{
    std::map<std::string, std::map<int, double> >
    ::iterator it = queryIndex_.find(s) ;
    if(it == queryIndex_.end())
        return 0;
    map<int, double>::iterator it2 = (it->second).find(id);
    if(it2 == (it->second).end())
        return 0;
    return it2->second;
    
}

//把交集取出来之后开始计算相似度

void Serch::calSimiler()
{
    for(set<int>::iterator set_it =queryId_.begin(); 
        set_it != queryId_.end(); ++set_it)
    {
        map<string, double>::iterator it = queryWords_.begin();
        double similar = 0;
        for(; it != queryWords_.end(); ++it)
            {
                similar = similar + it->second * 
                findWeight(it->first, *set_it);
            }
        Similar s;
        cout << " *set_it = " << *set_it << endl;
        cout << " similar = " << similar << endl;
        s.docid_ = *set_it;
        s.sim_ = similar;
        q_.push(s);

    }
}


void Serch::returnRES(Similar similar)
{
    int fd = open(pagelib_.c_str(), O_RDONLY); 
    if(fd == -1)
        ERR_EXIT("open pagelib error");
    ifstream offin;
    offin.close();
    offin.clear();
    offin.open(offsetdat_.c_str());
    if (!offin)
        ERR_EXIT("open offset error");

    Page page;
    page.id_ = similar.docid_;
    int beg = cinvertedIndex_.getdocoffset()[page.id_].beg_;
    int end = cinvertedIndex_.getdocoffset()[page.id_].end_;
    char buf[end - beg + 1];
    lseek(fd, beg, SEEK_SET);
    read(fd, buf, end - beg + 1);
    string s = buf;

    int t_beg = s.find("<title>") + 7;
    int t_end = s.find("</title>");
    page.title_ = s.substr(t_beg, t_end - t_beg);
    int c_beg = s.find("<content>") + 9;
    int c_end = s.find("</content>");
    page.content_ = s.substr(c_beg, c_end - c_beg);
    cout << "the pageid you find is :" << page.id_ << endl;
    cout << "the title is :" << page.title_ << endl;
    cout << "press any key to view the content" << endl;
    getchar();
    getchar();
    cout << page.content_ << endl;
}



Serch::~Serch()
{

}





