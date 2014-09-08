#include "creatInvertedIndex.h"

#include <iostream>

using namespace std;



int main(int argc, const char *argv[])
{
   
    creatStopDict StopDict("stopList.txt");
    InvertedIndex Index("lib/docoffset.dat", "lib/doclib.txt", StopDict);

    return 0;
}


