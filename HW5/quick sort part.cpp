#include<vector>
#include<algorithm>
#include<iostream>
#include<fstream>
#include<string>
#include <sstream>

using namespace std;

struct input
{
	int at;
	int dur;
	bool operator < (const input &a) const
	{
		return (at < a.at);
	}
	input(int vAt, int vDur): at(vAt), dur(vDur) {};
};

int main()
{
	vector <input> a;
	int size = 0;
	string line;
	string line1;
	int arTi;
	int dura;
	
	ifstream fin1("processlog.txt");
	
	ofstream fout1("ps-log.txt");
	
	if (fin1.fail()) return -1;
	
	while (getline(fin1,line))
	{
		if (line.find("#")!=string::npos)
		{
			fout1 << line << endl;
			continue;
		}
		
		if (line =="") continue;
		
		line.erase(remove(line.begin(),line.end(),' '),line.end());
		
		istringstream ss2(line);
		getline(ss2,line1,',');
		istringstream (line1) >> arTi;
		getline(ss2,line1,'\0');
		istringstream (line1) >> dura;
		
		a.push_back(input(arTi,dura));
		size++;
		
	}
	
	sort(a.begin(),a.end());
	for (int i=0;i<size; i++)
	{
		fout1 << a[i].at << ',' << a[i].dur << endl;
	}
	
	fin1.close();
	fout1.close();
	
	return 0;
}
