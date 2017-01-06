#include<iostream>
#include<sstream>
#include<string>

using namespace std;

int main()
{
	string str = "1234567";
	string sub;
	int i=0;
	int k=2;
	int num;
	int size;
	size = str.size();
	cout << size<< endl;
	
	//sub=str.substr(i,k);
	while(i<size)
	{
		if ((size-i)<k)
		{
			sub=str.substr(i,size-i);
			cout << sub << endl;
	
			istringstream (sub) >> num;
			cout << num << endl;
			break;
			
		}
			
		sub=str.substr(i,k);
		i=i+2;
		
		cout << sub << endl;
	
		istringstream (sub) >> num;
		cout << num << endl;
		
		
		
	}
	
	return 0;
}
