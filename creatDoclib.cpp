#include "creatDoclib.h"
#include <iostream>
#include <stdexcept>
#include <dirent.h>
#include <sstream>
//#include "Rio.h"


using namespace std;

#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)



DocLib::DocLib(creatStopDict &StopDict, const string &dirname)
	:dirname_(dirname), StopDict_(StopDict)

{
	ReadPageDir(dirname);
	createdoclib();
}


void DocLib::fun(string &s)

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







void DocLib::createdoclib()
{
	//=================结巴分词===============//
    MixSegment segment(dict_path, model_path);
    vector<string> words;
    cout << "increatdoclin" << endl;
	struct docinfo tdocinfo;// 创建索引信息的结构体包含读文件时开始的位置和[偏移量
	
	int i = 0;	
	for(int filenum = 0 ;filenum != filenames_.size() ; ++filenum )
    {     


    	set<string > HighFreWordset;
		HighFreWord_.clear();//每次都把高频词map置空
		words.clear();//清空切单词的容器
        cout << "words.clear" << endl;
    	ifstream in;
    	if(!open_file(in, filenames_[filenum]))
    		ERR_EXIT("open_file");

    	
        string title;
        string content;
        string temp;
        string id;
        
        cout << " 78" << endl;

    	string line;
    	getline(in, line);
    	fun(line);
    	title = line;
    	id = ++i + '0';
        cout << "85" << endl;
    	//tdocinfo.origin_ = lseek(fd, 0, SEEK_CUR);
		while(getline(in, line))
		{

            cout << "90" << endl;
			fun(line);
            segment.cut(line, words);
			cout << line << endl;
			temp += line;
            for(int ix =0; ix != words.size(); ++ix)
            {
                if(StopDict_.getexcludeWord().count(words[ix]))
                    continue; 
                else  
                    ++ HighFreWord_[words[ix]]; 
            }
		}

		content = temp;
        //tdocinfo.length_ = lseek(fd, 0, SEEK_END) - tdocinfo.origin_;


        string doc = "<doc>\n    <docid>" + id + "</docid>\n" + 
        			 "    <title>" + title + "</title>\n" + "    <content>\n    " + 
        			 content + "\n    </content>\n</doc>\n";

        //libindex_.insert(make_pair(id, tdocinfo));
        doclib_.push_back(doc);
        getTop10(HighFreWord_);
        //=======去重========
        if(allTopTenWords_.size() <= 1)
        	cout << "11111111111111111111" << endl;
        else
        	{
        		cout << "--quchong--" << endl;
        		DuplicateRemoval();
        	
        	}



        //===================

        //=======建索引=======




        //====================

       
    }

     //=======建库=========

        buileFile();

     //====================
//cout << "===================test==================" << endl;
//===========用来测试创建的索引=========
/*for(map<std::string , struct docinfo>::iterator 
	it = libindex_.begine();
	it != libindex_.endl();
	++it)
{

	cout << it -> first << " " <<
	 it->second.origin_ << " " << 
	 it->second.length_ << endl;
}*/



//=======================================

//==========用来测试doc文本==============
	for(vector<string>::iterator ix = doclib_.begin(); ix != doclib_.end(); ++ix)
    {
        cout << *ix << endl;

    }
//========================================
}


//读取文件夹下的文件
void DocLib::ReadPageDir(const string &dirname)
{
    DIR * dir;
    struct dirent *ptr;
    char s[1000];

    if ((dir = opendir(dirname.c_str())) == NULL)
        ERR_EXIT("opendir");

    while ((ptr = readdir(dir)) != NULL)
    {
        if (strcmp(ptr->d_name, ".") == 0 
                || strcmp(ptr->d_name, "..") == 0)
            continue;
        else if (ptr->d_type == 8)  //file
        {
            string temp = dirname + "/" + ptr->d_name;
            filenames_.push_back(temp);
            cout << temp << endl;
        }
        else if (ptr->d_type == 4) //dir
        {
            memset(s, '\0', sizeof(s));
            strcpy(s, dirname.c_str());
            strcat(s, "/");
            strcat(s, ptr->d_name);
            ReadPageDir(s);
        }
        else
            continue;
    }
    closedir(dir);
}



void DocLib::getTop10(std::map<std::string, int> &HighFreWord)
{
	priority_queue<struct qtopten > topten;//优先级队列取top10
	struct qtopten tempstu;
	set<string > HighFreWordset;
	for(map<string, int>::iterator it = HighFreWord.begin(); it != HighFreWord.end(); ++it)
		{
			tempstu.word_ = it -> first;
			//把top10放入一个set容器中
			HighFreWordset.insert(it -> first);
			tempstu.frequence_ = it -> second;
			topten.push(tempstu);

		}
		allTopTenWords_.push_back(HighFreWordset);

		for(int i = 0 ; i != 10 ; ++ i)
		{
			cout << topten.top().word_ << " " 
				<< topten.top().frequence_  << endl;
			topten.pop();
		}
		cout <<  "=============" << endl;


}


//把整型转化为string类型
string itos(int i)
{
    char c[1024];
    sprintf(c, "%d", i);

    return (string)c;
}

void DocLib::buileFile()
{
	cout << "buildlib start" << endl;
    int fd_doclib = open("lib/doclib.txt", O_WRONLY | O_CREAT, 0666);
    if(fd_doclib == -1){
        throw runtime_error("fd_doclib");
    }
    int fd_offset = open("lib/docoffset.dat", O_WRONLY | O_CREAT, 0666);
    if(fd_offset == -1){
        throw runtime_error("fd_offset");
    }

    for(int i = 0; i != doclib_.size(); ++i)
    {
    	off_t begin = lseek(fd_doclib, 0 ,SEEK_CUR);
    	string s = doclib_[i];
    	write(fd_doclib, s.c_str(), s.size());
        off_t end = lseek(fd_doclib, 0, SEEK_CUR);
        string index = itos(i) + " " + itos(begin) + " " +itos(end) + "\n";
        write(fd_offset, index.c_str(), index.size());

    }

    close(fd_doclib);
    close(fd_offset);





}


void DocLib::DuplicateRemoval()
{
	int i;                                             
	for (i = 0; i != doclib_.size() - 1; ++i)                  
	{                                    
		cout << "i = " << i << endl;                 
    	//for (j = i + 1; j != doclib_.size(); ++j)          
    	//{                                                 
        	set<string> iset = allTopTenWords_[i];      
       	 //合并两个set然后查看set的大小，来辨别相似度  
       	 set<string> jset = allTopTenWords_[doclib_.size() -1 ];      
       	 iset.insert(jset.begin(), jset.end());        
       	 //合并的set大小如果小于13说明有七个以上重复   
       	 if (iset.size() < 14)  
       	 	
       	 	{
       	 		doclib_.pop_back();
       	 		cout << "====inquchong==="	<<endl;
       	 	}                      
            //doclib_[j].id_ = '-1';                       
   		//} 
   	}                                                
}                                                     




/*void del_dupdoc()
{
    vector<Page> temp(pages_);
//    cout << "before: " << pages_.size() << endl;
    pages_.clear();
    for (int i = 0; i != temp.size(); ++i)
    {
        if (temp[i].id_ != -1)
            pages_.push_back(temp[i]);
    }
//    cout << "after: " << pages_.size() << endl;
}*/




DocLib::~DocLib()
{}
