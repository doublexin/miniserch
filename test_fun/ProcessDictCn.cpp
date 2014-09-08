#include "ProcessDictCn.h"
#include "include/MixSegment.hpp"
#include <cstdlib>
#include <cstdio>
#include <stdexcept>
#include <iostream>
using namespace CppJieba;
using namespace std;


const char * const dict_path =  "dict/jieba.dict.utf8";
const char * const model_path = "dict/hmm_model.utf8";


void CnDict::readFile(const string &filename, set<string> &excludeWord, map<string, int> &WordDict)

{
    MixSegment segment(dict_path, model_path);
    vector<string> words;
    ifstream in;
    in.open(filename.c_str());
    if(!in)
        throw runtime_error("open fial");
    string line;
    CnDict::mwviter it = WordDict.begin();
    
    while(getline(in, line))
    {   
        
        words.empty();
        string word;
        segment.cut(line, words);
        for(int ix =0; ix != words.size(); ++ix)
        {
            if(excludeWord.count(words[ix]))
                continue;
            else
                ++ WordDict[words[ix]];        
        }        
}

    in.close();
}
