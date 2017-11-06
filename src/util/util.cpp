#include "util.h"
#include <sstream>

using std::vector;
using std::string;
using std::stringstream;
using std::getline;

namespace gud
{

vector<string> split(const string& s, char delim)
{
	vector<string> elems;
	stringstream ss(s);
	string elem;
	while (getline(ss, elem, delim))
	{
		if (!elem.empty())
		{
			elems.push_back(elem);
		}
	}
	return elems;
}

}
