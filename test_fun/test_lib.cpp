//测试如何从语料文件中读取相应的信息，然后以xml的格式写在lib里
#include "Rio.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <unistd.h>
#include <map>
#include <queue>
#include <cstdio>
#include "include/MixSegment.hpp"
#include <cstdlib>
#include <set>
#include <fstream>
#include <sstream>


#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

using namespace std;

//========结巴
using namespace CppJieba;  
const char * const dict_path =  "dict/jieba.dict.utf8";
const char * const model_path = "dict/hmm_model.utf8";




//========优先级队列
struct qtopten
{
    string word_;
    int frequence_;

    friend bool operator< (const qtopten &va, const qtopten &vb);
   
};

inline bool operator< (const qtopten &va, const qtopten &vb)
// :qEditdis_(va),qFrquence_(vb)
{    
    return va.frequence_ < vb.frequence_;
}
//==================


struct docinfo
{

    off_t origin_;
    long int length_;
};



void fun(string &s)
{
    size_t n = s.find_last_not_of( " \r\n\t" ); 
    if( n != string::npos ) 
    { 
        s.erase( n + 1 , s.size() - n ); 
    }

    n = s.find_first_not_of ( " \r\n\t" ); 
    if( n != string::npos ) 
    { 
        s.erase( 0 , n ); 
    }

}

void fun1(char *buf)
{
    int n = 0;

    for(int i = 0; i != 1024 ; ++i)
    {
        if(i+1 < 1024 && (buf[i] == '\r' && buf[i + 1] == '\n'))
        {

            buf[i] = '\0';           
            buf[i+1] = '\0';           
        }
    }
}


int main(int argc, const char *argv[])
{
    //==========创建停止词字典==============
    string filename = "stopList.txt";
    set<std::string> excludeWord_;
    ifstream in;
        in.close();                                         
      in.clear();                                         
      in.open(filename.c_str());                          
      if(!in)
          throw runtime_error("open fial");               
      string line;                                        
                                                                                                                      
      while(getline(in, line))
      {   
         fun(line); 
          `
          while(iss >> word)
          {
              excludeWord_.insert(word);
          }
      }
      excludeWord_.insert(" ");
      excludeWord_.insert("  ");
      excludeWord_.insert("【");
     in.close(); 

    //============停止词词典结束============
    map<string, int> HighFreWord; //定义高频单词
    map<string , struct docinfo> libindex;//定义索引
    char file_name[15];
    vector<string> doclib;//定义网页库
    int i = 0;
    struct docinfo tdocinfo;// 创建索引信息的结构体包含读文件时开始的位置和[偏移量
    priority_queue<struct qtopten > topten;//优先级队列取top10
    struct qtopten tempstu;
	vector<set<string> > allTopTenWords;
    //=================结巴分词===============//
    MixSegment segment(dict_path, model_path);
    vector<string> words;
    //================开始循环读文件=========//
    for(int filenum = 1 ;filenum != 4 ; ++filenum )
    {     

		set<string > HighFreWordset;
		HighFreWord.clear();//每次都把高频词map置空
		words.clear();
        sprintf(file_name, "C3-Art%04d.txt", filenum);
        int fd = open(file_name, O_RDONLY); 
        if(fd == -1)
            throw runtime_error("open");
        Rio r(fd);
        char buf[1024];
        string url;
        string title;
        string content;
        string temp;
        string id;
//============开始创建网页库=============//
        r.readline(buf, 1024);

        fun1(buf);
        url = buf;
        id = ++i + '0';
        //libindex.insert(id);
        r.readline(buf,1024);
        title = buf;
        fun(title);
        tdocinfo.origin_ = lseek(fd, 0, SEEK_CUR);
        while(r.readline(buf, 1024))
        {
            temp += buf;
            fun(temp);
            segment.cut(temp, words);

            for(int ix =0; ix != words.size(); ++ix)
            {
                if(excludeWord_.count(words[ix]))
                    continue; 
                else  
                    ++ HighFreWord[words[ix]]; 
            }

        }
        content = temp;
        tdocinfo.length_ = lseek(fd, 0, SEEK_END) - tdocinfo.origin_;


        string doc = "<doc>\n    <docid>" + id + "</docid>\n" + "    <url>" + url + "</url>\n" + "    <title>" + title + "</title>\n" + "    <content>\n    " + content + "\n    </content>\n</doc>\n";

        libindex.insert(make5_pair(id, tdocinfo));
        doclib.push_back(doc);
        //cout << "=====" << endl;
        //cout << doc << endl;

		cout << "======top10=====" <<endl;
		//取top10
		//把map里的单词和词频放到结构体中，然后将结构体放入队列当中
		for(map<string, int>::iterator it = HighFreWord.begin(); it != HighFreWord.end(); ++it)
		{
			tempstu.word_ = it -> first;
			//把top10放入一个set容器中
			HighFreWordset.insert(it -> first);
			tempstu.frequence_ = it -> second;
			topten.push(tempstu);
		}
		//把当前文档的top10的集合放入vector
		allTopTenWords.insert(HighFreWordset);
		//打印队列前十
		for(int i = 0 ; i != 10 ; ++ i)
		{
			cout << topten.top().word_ << " " 
				<< topten.top().frequence_ << endl;
			topten.pop();
		}

    }
    cout << "=========" << endl;
    for(vector<string>::iterator ix = doclib.begin(); ix != doclib.end(); ++ix)
    {
        cout << *ix << endl;

    }

    cout << "====map====" << endl;//打印了记录id 号 lseek 和 offset的索引
    for(map<string, struct docinfo>::iterator it = libindex.begin(); it != libindex.end(); ++it)
    {
        cout << it -> first << " " << it->second.origin_ << " " << it->second.length_ << endl;
    }
    

    
//去重设置一个set用来进行topWord的判断，

	for (int i = 0; i != allTopTenWords.size(); ++ i)
	{
			for(int j = i + 1; j != allTopTenWords.size(); ++ j)
			{
				set<string > iset = allTopTenWords[i];
				set<string > jset = allTopTenWords[j];
				iset.insert(jset.begin(), jset.end());
				if (iset.size() < 14)
					libindex[j].first= '-1';
			}

	}
	
		
		

	




    return 0;

}
