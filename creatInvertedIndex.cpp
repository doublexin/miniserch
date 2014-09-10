#include "creatInvertedIndex.h"


#include <iostream>
#include <stdio.h>
#include <stdlib.h>      //纯阳的ERR和atoi
#include <string.h>
#include <sys/types.h>   //open
#include <sys/stat.h>
#include <fcntl.h>       // endopen
#include <unistd.h>      //read
#include <math.h>        //sqrt



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


InvertedIndex::InvertedIndex( const string &docoffset, const string &docname, creatStopDict &StopDict)
    :docoffsetname_(docoffset),
     StopDict_(StopDict)

{
    readDocoffset(docoffset);
    readDoclib(docname);
    dealWithDoc();
    calIDF();
}

void InvertedIndex::readDocoffset(const string &docoffset)
{
    struct offset node;
    ifstream fin;
    docCont_ = 0;
    if (!open_file(fin, docoffset))
        ERR_EXIT("open offset error");
    while (fin >> node.docid_)
    {   docCont_ ++;
        fin >> node.beg_;
        fin >> node.end_;
        docoffset_.push_back(node);
    }
   // cout << "=======================================" << endl;
   // cout << "docCont_ "  << docCont_ << endl;
   fin.close(); 
}

void InvertedIndex::readDoclib(const std::string &docname)
{
    int length = 0;
    int fd = open(docname.c_str(), O_RDONLY);
        if(fd == -1)
            ERR_EXIT("open lib failed");

    
    string doctext;
    

    for(vector<offset>::iterator it = docoffset_.begin();
            it != docoffset_.end();
            ++ it)
    {
         length = (it->end_- it->beg_ + 1);
        // cout << "head " << it->beg_ << " end " << it->end_ << endl;
        char buf[length];
        memset(buf, 0, length);
        doctext.clear();
        read(fd, buf, length);
            doctext += buf;
        doclib_.push_back(doctext);
    
    }




 // 此处为了测试doclib_
  
  /* for(vector<string>::iterator it = doclib_.begin(); 
            it != doclib_.end();
            ++it)
    {
        cout << *it << endl;
        cout << "=============================" << endl;
    
    }*/

    close(fd);


 /*  
  * 此处用来测试readoffset是否成功
  *  for(vector<offset>::iterator it = docoffset_.begin();
            it != docoffset_.end(); 
            ++ it )
    {
        cout << "id " << it->docid_ << " begin " << it->beg_ << 
            " end " << it->end_ << endl; 
     
    }*/


}

void InvertedIndex::dealWithDoc()
{
    //=================结巴分词===============//
    MixSegment segment(dict_path, model_path);
    vector<string> words;
    set<int > tmp;//该临时变量用于存放临时单词出现的id号
   // map<string, double>  everyDocWordsFrq_;
    int id1  = 0;
    for(vector<string>::iterator it = doclib_.begin();
            it != doclib_.end() ; ++ it, ++id1)
    {
        //id1 = 0;
        allWordsFrq_.clear();
        everyDocWordsFrq_.clear();
        segment.cut(*it, words); 
        for(int ix =0; ix != words.size(); ++ix)
        {
            if(StopDict_.getexcludeWord().count(words[ix]))
                continue; 
            else  
            {   
                tmp.clear();

                ++ allWordsFrq_[words[ix]];//用于记录整个库的每个单词的TF 
                //每有一个单词就记录下这个单词出现过的Id号
                map<string, set<int> >::iterator ii = wordsShowId_.find(words[ix]);
                if(ii != wordsShowId_.end())
                {    
                    ii->second.insert(id1);
                }
                else
                {
                    tmp.clear();
                    tmp.insert(id1);
                    wordsShowId_.insert(make_pair(words[ix], tmp));
                }
                ++ everyDocWordsFrq_[words[ix]];//用于记录当前文档的单词的TF

                
            }
        }


         //计算TF此时读完第一篇文档
         /*  double wordCont = everyDocWordsFrq_.size();
         for(map<string, double>::iterator kk = everyDocWordsFrq_.begin(); 
            kk != everyDocWordsFrq_.end(); ++ kk)
         {
            kk->second = kk->second / wordCont ;
         }*/
    
         everyWordsVec_.push_back(everyDocWordsFrq_);//把每一篇的统计单词的map放入一个集合

    }


//用于测试everyWordsvec_
/*for(int j = 0 ;j != everyWordsVec_.size(); ++j)
{

    cout << "==================START======================" << endl;
    for (std::map<string, double>::iterator ix = everyWordsVec_[j].begin();
        ix != everyWordsVec_[j].end(); ++ix)
    {
        cout << "word = " << ix->first << " tf= " << ix->second <<endl;
        cout << "size " << everyWordsVec_[j].size() << endl;
       
    }
    cout << "====================END======================" << endl;
}*/




//此处用来测试wordshowId
/*for(map<string, set<int> >::iterator it = wordsShowId_.begin();
        it != wordsShowId_.end(); ++it)
{
    cout << it -> first << " " ;
    for(set<int>::iterator i = it->second.begin() ;i != it->second.end(); ++ i)
    {
        cout  << *i << " ";

    }
    cout << endl;
}*/


//此处用来测试TF
/*cout << "=========TEST TF=========" << endl;
for(map<string, int>::iterator it = allWordsFrq_.begin(); 
    it != allWordsFrq_.end(); ++ it)
    {
        cout << it ->first << "  " << it -> second << endl;
    }

*/
//此处用来测试 allwrodsdrq和everyDocWordsFrq
/*for(map<string, int>::iterator it = allWordsFrq_.begin();
        it != allWordsFrq_.end(); ++ it)
{
    cout << "----" <<  it->first  << " ----" << it -> second << endl;

}
for(vector<map<string , int > >::iterator ix = everyWordsVec_.begin();
        ix != everyWordsVec_.end(); ++ix)
{
   for(map<string, int>::iterator ii = (*ix).begin();ii != (*ix).end(); ++ ii)
    cout << "====" << ii -> first << "====" << ii ->second << endl;

}*/

}

void InvertedIndex::calIDF()
{
    double idf;
    int id;
    vector<double> tempwegiht;
    double allwegiht = 0;
    map<int, double> tempidweight;
    invertedIndex_.clear();
    for(map<string, set<int> >::iterator it = wordsShowId_.begin(); 
            it != wordsShowId_.end(); ++it)
    {
        id = 0;
        tempwegiht.clear();
        tempidweight.clear();//这里一定要初始化要不然索引的结果会变成上一次记录的结果和这一次的结果的集合
        idf = log( double(docCont_) / double(it->second.size() ));
       
        for(int i = 0; i != everyWordsVec_.size() ; ++ i)
        {
            map<string, double>::iterator 
            ix = everyWordsVec_[i].find(it->first);
            if(ix != everyWordsVec_[i].end())
            {  
               id = i;
              
               tempwegiht.push_back((ix->second) * idf);//此时算出了Tf/IDF
            } 
            else
                tempwegiht.push_back(0 * idf);

        }
        //此处测试TF/IDF的值
       /* cout << "word= " <<  it->first << endl;
        for(vector<double>::iterator tjj = tempwegiht.begin();
            tjj != tempwegiht.end(); ++ tjj)
        {
            cout << " TF/IDFs " <<*tjj ;
        }
        cout << endl;*/
        
        allwegiht = 0;
        for(int k = 0; k != tempwegiht.size(); ++k)
        {

            allwegiht += tempwegiht[k];
        }
        allwegiht = allwegiht * allwegiht;
       // cout << " allwegiht= " << allwegiht << endl; 
        
        //对每篇文档的某一单词归一化
        set<int>::iterator it1 = it->second.begin();

        for(int j = 0; j != tempwegiht.size(); ++j)
        {
     
          if(tempwegiht[j] == 0)
            continue;
        else
        {
         double val =  tempwegiht[j] / sqrt(allwegiht);
        tempidweight[*it1] = val;
        invertedIndex_[it->first] = tempidweight;
         it1++; 
            }
         }
            
        
    }


//用来测试倒排索引

/*for(map<string, map<int, double> >::iterator p1 = invertedIndex_.begin();
    p1 != invertedIndex_.end(); ++p1)
    {
        cout << "words " << p1->first ;
        for(map<int ,double>::iterator p2 = p1->second.begin();
            p2 != p1 ->second.end(); ++ p2)
        {

            cout << " id= " << p2->first << " weight= " << p2->second << " | " ;
        }
        cout << endl;

    }*/

}
map<string, map<int, double> > &InvertedIndex::getIndex()
{
    return invertedIndex_;
}
std::vector<std::string> &InvertedIndex::getdoclib()
{
    return doclib_;
}

InvertedIndex::~InvertedIndex()
{
}
