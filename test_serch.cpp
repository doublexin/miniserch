#include "serch.h"

#include <iostream>
#include <string>
#include <vector>


using namespace std;


int main(int argc, const char *argv[])
{
    creatStopDict StopDict("stopList.txt");
    InvertedIndex Index("lib/docoffset.dat", "lib/doclib.txt", StopDict);
    Serch serch(Index, StopDict, "lib/doclib.txt", "lib/docoffset.dat");
    cout << "111" << endl;
    string lines;
   // while(1)
   // {
        serch.dealWithCin();
        serch.startQuery();
   // }




    return 0;
}
