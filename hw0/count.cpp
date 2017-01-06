/*
Course:		COSC - 2320
Assignment: #00
Date Due :  8 - 29 - 2015
Student :   Thanh Vo
Peoplesoft: 1148524
*/

#include <iostream>		//provide cin, cout
#include <fstream>		//provide ifstream, ofstream
#include <cstring>		//provide c string
#include <string>		//provide string
#include <sstream>		//provide istringstream

//setting up my environment
using namespace std;

//main program starts here
int main (int argc, char* argv[])
{
	char ch; 							// track character stream in
	char word[31];						// track a word
	char num[11];						// track a sequence of numbers
	int count1 = 0; int count2 = 0;		// count1 is word count, count2 is number count
	int w = 0;							// index for word
	int n = 0;							// index for number
	string fileName;					// track the file name
	string argument;					// track the argv[1] in string
	char sb;							// track the previous character
	
	//check if there is more than 1 argument
	if (argc <=1)
	{
		cout << "No file name entered. Exiting...";
		return -1;
	}
	
	//convert argv[1] from cstring to string then retrieve the name of text file from it
	argument = argv[1];
	istringstream ss(argument);
	getline(ss,fileName, '=');
	getline(ss,fileName, '\0');
	
	// stream input a text file
	ifstream fin(fileName.c_str());	  			
	
	// notify a failed open-file
	if(fin.fail())
	{
		cout << "file cannot be opened" << endl;
	}
	
	//get 1st character
	fin.get(ch);
	
	// get the rest character and count numbers and words
	while (!fin.eof())
	{
		//if the character is alpha, word index +1 and get another character
		if ((isalpha(ch))&&(ch!='r'))
		{
			word[w] = ch;
			sb = ch;
			w++;
			fin.get(ch);
		}
		
		
		// if the character is digit, number index +1 and get another character
		else if (isdigit(ch))
		{
			num[n] = ch;
			sb = ch;
			n++;
			fin.get(ch);
		}
		
		
		// if character is r that is following a '\' and without any alpha character after it, it is considered a carriage return, no word count for it
		else if (ch=='r')
		{
			word[w] = ch;
			w++;
			fin.get(ch);
			if (((char)sb==92) && (!isalpha(ch)))
			{
				w=0;
			}
		}
		
		
		// if character is neither alpha nor digit, if the word index > 0 or number index >0 then word count or number count +1, reset w and n to 0
		// get another character
		else
		{
			sb = ch;
			if(w>0)
				count1 ++;
			
			else if (n>0) 
				count2 ++;
			
			w=0;
			n=0;
			
			fin.get(ch);
		}
	}
	
	
	//check if there is the last word or last number in the text file and count it
	if (w>0) count1++;
	
	if (n>0) count2++;
	
	
	//output result
	cout << "words=" << count1 << " numbers=" << count2 << endl;
	
	
	// close file
	fin.close();
	
	
	//tell user to hit any key for exit
	return 0;
}
