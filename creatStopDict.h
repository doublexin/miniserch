#ifndef CREATSTOPDICT_H
#define CREATSTOPDICT_H 


#include <fstream>
#include <sstream>
#include <string>

#include <set>


class creatStopDict
{
public:
	creatStopDict(const std::string &filename);
	~creatStopDict();
	void readStopList(const std::string &filename);
	std::set<std::string> &getexcludeWord();

private:
	std::set<std::string> excludeWord_;

	/* data */
};





#endif