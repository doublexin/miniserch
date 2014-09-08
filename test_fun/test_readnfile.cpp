#include <stdio.h>
#include <sys/types.h>   //头文件
#include <sys/stat.h>
#include <fcntl.h>


//该程序用来测试  不停地读取文件
//



int main()

{

    char file_name[15];

    int i;

    for(i=0;i<3;i++)

    {
    
           sprintf(file_name, "Beautyd%1d.txt", i);
            int i = open(file_name, O_RDONLY);
            if(!i)
                printf("error");
            else
                printf("%d",i);
           //freopen(file_name, "r", stdin); 
          // freopen(file_name, "wb", stdout);
          // printf("%s\n", stdout);
    
        }
    return 0;

}
