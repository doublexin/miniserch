#include "creatDoclib.h"



using namespace std;


int main(int argc, const char *argv[])
{
    
    creatStopDict StopList("stopList.txt");
    DocLib doclib(StopList, "./data");

    return 0;
}
