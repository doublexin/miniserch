//测试用Rio读文档时有空行的情况
//
#include "Rio.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdexcept>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
int main(int argc, const char *argv[])
{

    char buf[1024];
    int fd = open("test.txt",O_RDONLY); 
    if(fd == -1)
    {
        throw std::runtime_error("open");
        printf("%d",errno);
    }
    Rio R(fd);
    
    int num;
        while(1)
        {
            num = R.readline(buf, 1024);
           // if(num == 0)
           // {
                
            //  printf("%d++\n", errno);
              sleep(3);
            
            printf("%d---\n", num);
            printf("%s===\n", buf);
        }
    
    


    return 0;
}
