#include <iostream>
#include <string>
#include <vector>

#include <map>
#include <set>

using namespace std;





int main(int argc, const char *argv[])
{
    string words;
    int id;
    map<string, vector<int> > foobar;
    vector<int> tmp;
    while(cin >> words >> id)
    {
        map<string, vector<int> >::iterator it = foobar.find(words);

        if(it != foobar.end())
        {
            it ->first = words;
            it ->second.push_back(id); 
        }
        tmp.clear();  
        tmp.push_back(id);
        foobar.insert(make_pair(words, tmp)); 

    }
    
   // tmp.clear();
   // tmp.push_back(5);
 
   // foobar.insert(make_pair("i", 1));
   // foobar.insert(make_pair("i", 2));
    /*tmp.clear();
        tmp.insert(3);
        tmp.insert(7);
        tmp.insert(18);
        tmp.insert(35);
    foobar.insert(make_pair("double", tmp));*/


    /*for(multimap<string, int>::iterator it = foobar.begin();
            it != foobar.end(); ++it)
    {
    
        cout << it->first << "  " << it ->second <<  endl;
    }*/




    for(map<string, vector<int> >::iterator it = foobar.begin();
            it != foobar.end(); ++it)
    {
        cout << it -> first << " " ;
        for(vector<int>::iterator i = it->second.begin() ;i != it->second.end(); ++ i)
        {
            cout  << *i << " ";
        
        }
        cout << endl;
    }



    return 0;
}
