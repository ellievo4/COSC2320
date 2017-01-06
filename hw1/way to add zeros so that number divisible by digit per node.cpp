#include<iostream>
#include<string>

using namespace std;

int main()
{
	string str1 = "20000"; string str2;
	int n = str1.size();
	int digit;
	cout << "digit: ";
	cin >> digit;
	if (n%digit!=0)
	{
		int rem = n%digit;
		int n = digit - rem;
		int i=0;
		while(i< n)
		{
			str1 = "0" + str1;
			i++;
		}
		str2 =str1;
		cout << str2 << endl;
	}
	
	else
		cout << str1 << endl;
		
	
	
	return 0;
}
