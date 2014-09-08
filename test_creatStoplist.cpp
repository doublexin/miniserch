#include "creatStopDict.h"

#include <iostream>
using namespace std;




int main(int argc, char const *argv[])
{
	
	creatStopDict stopDict("stopList.txt");

	for(set<string >::iterator 
		it = stopDict.getexcludeWord().begin(); 
		it != stopDict.getexcludeWord().end(); 
		++ it)
	{
		cout << *it << endl;

	}

	return 0;
}

