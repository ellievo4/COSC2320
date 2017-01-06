#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

int main (int argc, char* argv[])
{
	if(argc==1) return -1;
	
	string line;							//track a line of command line argument + each whole line in command text file
	ifstream fin1;							//stream input for both text files
	string filename1, filename2;			//name of 2 text files
	string xyz;								//each line of input text file
	string command;							//each line in command text file
	string cmd;								//command keyword
	string numb;							//number of line of input text file in string
	int num;								//number of line of input text file in int
	string rest;							//the rest of a line in command text file after retrieving command keyword and number of line and searchword 
	string searchword;						//search word for delete and insert a word
	string insertword;						//word to insert
	
	string argument = argv[1];				//command line argument in string form
	
	//parse the command line argument to get 2 text files' names
	istringstream ss (argument);			
	getline(ss,line,'=');
	getline(ss,line,';');
	filename1 = line;
	
	getline(ss,line,'=');
	getline(ss,line,'\0');
	filename2 = line;
	
	//open input text file and add to list 1, then close this text file
	fin1.open(filename1.c_str());
	
	if (fin1.fail()) return -1;
	
	while (getline(fin1,xyz))
	{
		cout << xyz << endl;
	}
	
	fin1.close();
	
	//open command text file and do command as in each case then close command text file
	fin1.open(filename2.c_str());
	if (fin1.fail()) return -1;
	
	while (getline(fin1,command))
	{
		cout << command << endl;
		
		//make sure there is a string "=" or nothing will be operated
		if ((command.find("=") != string::npos))
		{
			//getting command keyword and the rest of line
			istringstream stst (command);
			getline(stst,line,'=');
			cmd = line;
			getline(stst,line,'\0');
			rest = line;
			
			//if the command is deleting a line, this case works with certain line number only
			if (cmd == "deleteline")
			{
				if (rest!="")
				{
					istringstream(rest) >>num;
					cout << "delete line " << num << endl;
				}	
			}
			
			//if the command is inserting a line, this case works with certain line number only
			else if (cmd == "insertline")
			{
				if (rest!="")
				{ 
					istringstream(rest) >>num;
					cout << "insert line " << num << endl;
				}
			}
			
			//if the command is delete a word
			else if (cmd == "deleteword")
			{
				istringstream sts (rest);
				
				//make sure there is ":" like the format, else nothing is operated
				if ((rest.find(":") != string::npos))
				{
					//get number of line
					getline(sts,line,':');
					numb = line; 
					
					//again we won't work with emty line number, line number is converted to int 
					if (numb!="") 
					{
						istringstream(numb) >>num;
						
						//get the delete word
						getline(sts,line,'\0');
						rest = line;
						
						//no acception for any space
						if (rest.find(" ")==string::npos) 
						{
							cout << "delete word " << rest << " at line " << num << endl;
						}
					}
				}
			}
			
			//if the command is insert a word
			else 
			{
				istringstream iss (rest);
				
				//make sure there is ":" like the format, else nothing is operated
				if ((rest.find(":") != string::npos))
				{
					//get number of line
					getline(iss,line,':');
					numb = line;
					
					//again we won't work with emty line number, line number is converted to int 
					if (numb!="") 
					{
						istringstream(numb) >>num;
						
						//get the rest of the line
						getline(iss,line,'\0');
						rest = line;
						
						//no space allowed
						if (rest.find(" ")==string::npos) 
						{
							//first case: insert a word to non-empty line, there are searchword and insertword
							if(rest.find("/")!=string::npos)
							{
								iss.clear();
								iss.str(rest);
								getline(iss,line,'/');
								searchword = line;
								getline(iss,line,'\0');
								insertword = line;
								
								cout << "Insert word " << insertword << " after searchword " << searchword << " at line " << num << endl;
							}
							
							//second case: insert a word to emty line 
							else
							{
								insertword = rest;
								cout << "Insert word " << insertword << " at line " << num << endl;
							}
						}	
					}
				}
			}
		}
		
		cout << endl;
	}
	
	fin1.close();
	
	//ask user to hit any key for exit
	return 0;
}
