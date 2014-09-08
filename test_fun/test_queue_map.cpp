#include <iostream>
#include <string>
#include <vector>

#include <queue>
#include <map>



using namespace std;

int main(int argc, const char *argv[])
{
    
    map<string, int> aaa;
    aaa.insert(make_pair("china", 5));
    aaa.insert(make_pair("shenzhen", 10));
    aaa.insert(make_pair("usa", 3));

    priority_queue<map<string, int> > q;
    q.push(aaa);

    map<string, int>::iterator it = aaa.begin();
    while(q.size() != 0)
    {
      cout <<  q.top()->first  << "  " << q.top()->second << endl
          q.pop();
    
    }





    return 0;
}
