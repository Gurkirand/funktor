#include "load.h"
#include <fstream>

using std::vector;
using std::string;
using std::ifstream;
using std::getline;

vector<string> load_text_file(const string& fname)
{
	vector<string> lines;
	string line;
	ifstream textFile(fname);
	if (textFile.is_open())
	{
		while (getline(textFile, line))
		{
			lines.push_back(line);
		}
		textFile.close();
	}
	return lines;
}
