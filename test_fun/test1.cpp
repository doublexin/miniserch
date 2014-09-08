#include <iostream>
#include <string>
#include <vector>
#include "Rio.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
using namespace std;

int main(int argc, const char *argv[])
{
    
    int fd = open("Art0002.txt", O_RDONLY);
    Rio R(fd);
    char buf[1024];
    string s = "<title>";
    R.readline(buf, 1024);
    s += buf;
    s += "</title>";
    cout << s << endl;
    

    return 0;
}
