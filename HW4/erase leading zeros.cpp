#include<iostream>
#include<string>

using namespace std;

int main()
{
	string str;
	cout << "Enter a string: ";
	getline(cin,str);
	str.erase(0, min(str.find_first_not_of('0'), str.size()-1));
	//sometimes it gives empty string in which we should reset string to 0 
	cout << str<< endl;
	return 0;
}
