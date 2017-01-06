//Thanh Vo
//ID: 1148524
//Program Assignment 2

#include <iostream>			//provides cin, cout
#include <string>			//provides string
#include <fstream>			//provides stream input, output
#include <sstream>			//provides istringstream

//setting up environment
using namespace std;

//-----------------------------------------
//class List that have variables for a list and functions to operate a list
//-----------------------------------------
class List
{
	private: 
		string data;			//track data of a node
		List *next, *prev;		//track pointers of a node that point to previous or next node
		
	public:
		//constructor of class list
		List(string value)
		{
			data = value;
			next = NULL;
			prev = NULL;
		}
		
		List* findHead(List *c);												//find head function
		List* findEnd(List *c);													//find end function
		void addTail(List *c, string value);									//add after tail function
		void addHead(List *c, string value);									//add before head function
		void display(List *c);													//display nodes function
		void destroy(List *c);													//destroy list function
		List* addMiddle(List *c, string value, int pos);						//add node at nth position function 
		int count(List *c);														//count nodes function
		int search(List *c,string value, int i);								//search a node function
		List* deleteNode(List *c, int pos);										//delete node at nth position function	
		List* searchPos(List *c, int pos,int i);								//get node at nth position function
		string getStr(List *c);													//get combined string from node's data function
		List* deleteWord(List *c, string value, int pos);						//delete a word in a node data function		
		List* insertWord(List *c, string sWord, string iWord, int pos);			//insert a word to a node data function
		~List() {};																//destructor of class list
};

//-----------------------------------------
//recursive search-node function that takes in a list and returns the head node
//-----------------------------------------
List* List::findHead(List *c)
{
	if(c->prev==NULL)  
	{
		return c;
	}
	
	else
	{
		c = findHead(c->prev);
		return c;
	}
}

//-----------------------------------------
//recursive search-node function that takes in a list and returns the end node
//-----------------------------------------
List* List::findEnd(List *c)
{
	if(c->next==NULL)  
	{
		return c;
	}
	
	else
	{
		c = findEnd(c->next);
		return c;
	}
}

//-----------------------------------------
//insert-end-node function that takes in a list and insert value; find end of the list and insert after it
//-----------------------------------------
void List::addTail(List *c, string value)
{
	List *end = findEnd(c);
	List* temp = new List(value);
	end->next = temp;
	temp->prev = end;
	end = temp;
}

//-----------------------------------------
//insert-head-node function that takes in a list and insert value; find head of the list and insert before it
//-----------------------------------------
void List::addHead(List *c, string value)
{
	List *head = findHead(c);
	List *temp = new List(value);
	temp->next=head;
	head->prev=temp;
	head = temp;
	temp->prev = NULL;
}

//-----------------------------------------
//recursive search-node function that takes in a list and displays the list with nodes separated by new line character
//-----------------------------------------
void List::display(List *c)
{
	if(c==NULL) return;
	else
	{
		cout << c->data << endl;
		display (c->next);
	}
}

//-----------------------------------------
//recursive delete-list function that takes in a list and returns an empty list
//-----------------------------------------
void List::destroy(List *c)
{
	if(c==NULL) return;
	else 
	{
		destroy(c->next);
		delete c;
		c= NULL;
	}
}

//-----------------------------------------
//insert-a-node function that takes in a list, insert value, and position to insert and returns a node-inserted list
//-----------------------------------------
List* List::addMiddle(List *c, string value, int pos)
{
	List *head = c;
	List *curr = head;
	List *temp = new List(value); 
	int length = count(c);
	
	//if postion to add at head
	if(pos==1)
	{
		//if position 1 is already a blank string
		if(curr->data=="")
		{
			curr->data = value;
			return curr;
		}
		
		else
		{
			//if there is only 1 node in the list
			if(curr->next==NULL) 
			{
				addTail(curr,value); 
				return curr;
			}
			
			//else there are many nodes in the list
			else
			{
				temp->next = curr->next;
				curr->next->prev = temp;
				temp->prev = curr;
				curr->next=temp;
				return curr; 
			}
		}
	}
	
	//if position is at end
	else if (pos==length)
	{
		addTail(c,value);
		return c;
	}
	
	//if position is between head and end, get curr at pos and insert at pos+1	
	curr = searchPos(curr,pos,1); 
	temp->next = curr->next; 
	curr->next->prev=temp;
	curr->next = temp;
	temp->prev = curr; 
	return head;	
}

//-----------------------------------------
//recursive count function that takes in a list and returns number of nodes in list
//-----------------------------------------
int List::count(List *c)
{
	if (c==NULL) return 0;
	else 
	{
		return (1+count(c->next));
	}
}

//-----------------------------------------
//recursive search-node function that takes in a list, a search value, and index of start position and returns index of that word
//-----------------------------------------
int List::search(List *c, string value, int i)
{
	if (c==NULL) return 0;
	else
	{
		if (c->data == value) return i;
		else 
		{
			i++;
			return search(c->next,value,i);
		}
	}
}

//-----------------------------------------
//delete-a-node function that takes in a list, position to delete and returns a node-deleted list
//-----------------------------------------
List* List::deleteNode(List *c, int pos)
{
	List *head = findHead(c);	
	List *curr = new List("");	
	List *temp = new List("");
	curr = head;
	int len = count(c);
	
	//if position to delete is head
	if (pos==1)
	{
		if(curr->next!=NULL)
		{
			curr=curr->next;
			curr->prev = NULL;
			return curr;
		}
		
		else 
		{
			curr->data="";
			return curr;
		}
	}
	
	//else position is end, add at end an empty node, get curr at position pos-1 and delete at pos
	else if (pos== len)
	{
		addTail(c, " ");
		head = findHead(c);
		curr = searchPos(curr,pos-1,1); 
		temp = curr->next;
		curr->next = temp->next;
		temp->next->prev = curr;
		delete temp;
		return head;
	}
	
	//else position is between head and end, get curr at pos-1 and delete at pos
	else
	{
		curr = searchPos(curr,pos-1,1); 
		temp = curr->next;
		curr->next = temp->next;
		temp->next->prev = curr;
		delete temp;
		return head;	
	}
}

//-----------------------------------------
//recursive search-position function that takes in a list, a position, and index of start position and returns a pointer at desired position
//-----------------------------------------
List* List::searchPos(List *c, int pos, int i)
{
	if (c==NULL) return c;
	else
	{
		if(pos==i) return c;
		else
		{
			i++;
			c = searchPos(c->next,pos,i);
			return c;
		}
	}
}

//-----------------------------------------
//recursive getting-the-line function that takes in a list and returns a combination string of string from each node
//-----------------------------------------
string List::getStr(List *c)
{
	if (c==NULL) return "\0";
	else 
		return (c->data + " " + getStr(c->next));
}


//-----------------------------------------
//delete a word that takes in a list of lines, delete word and position of line number, return a word-deleted list
//-----------------------------------------
List* List::deleteWord(List *c, string value, int pos)
{
	//get head and set curr equal to head
	List* head = c; 
	List* curr = head;
	
	//get curr at position pos
	curr = searchPos(curr,pos,1);
	
	//retrieve the string from this position, parse words with no symbol with them and store them in list L2	
	string str = curr->data;
	string buffer;
	char str1[100];
	int end=0;
	stringstream buf (str);
	buf >> buffer; 
	List *L2 = new List(buffer);
	while(buf >> buffer) 
	{
		for(unsigned i = 0; i<buffer.size(); i++)
		{
			if (isalpha(buffer.at(i)))
			{
				str1[end]=buffer.at(i);
				end++;
			}
		}

		str1[end] = '\0';
		buffer = str1;
		addTail(L2, buffer);
		buffer.clear();
		end=0;
	}
	
	//search for index of delete word in L2 and use index to delete word
	L2 = findHead(L2);
	int foundIndex = search(L2, value, 1);
	if (foundIndex==0) return c;
	else L2 = deleteNode(L2, foundIndex);
	
	//store the edited string back to L1
	if (count(L2) > 1) L2 = findHead(L2);
	curr->data = getStr(L2);
	
	//delete list L2
	destroy(L2);
	
	//return head of L1
	return head;
}

//-----------------------------------------
//insert a word that takes in a list of lines, search word, insert word and position of line number, return a word-inserted list
//-----------------------------------------
List* List::insertWord(List *c, string sWord, string iWord, int pos)
{
	//get head if list c is one-node list or multi node list and set curr equal to head
	List* head = c;
	List* curr = head; 
	
	//get curr at position pos
	curr = searchPos(curr,pos,1);
	
	//retrieve the string from this position
	string str = curr->data; 
	string buffer;
	
	//add word to empty line, change the line from "" to the insert word
	if (curr->data=="")
	{
		curr->data = iWord;
		return c;
	}
	
	//split this line into words and store them in list L2 in the case of adding word to non-empty line	
	else 
	{
		stringstream buf (str);
		buf >> buffer; 
		List *L2 = new List(buffer);
		while(buf >> buffer) 
		{
			addTail(L2, buffer);
		}
		
		//search for index of searchword word in L2 and use index to insert word
		L2 = findHead(L2);
		
		if (sWord=="")
		{
			addHead(L2,iWord);
		}
		
		else
		{
			int foundIndex = search(L2, sWord, 1);
			if (foundIndex==0) return c;
			else L2 = addMiddle(L2, iWord, foundIndex);
		}
	
		//store the edited string back to L1
		if (count(L2) > 1) L2 = findHead(L2);
		curr->data = getStr(L2);
		
		//delete list L2
		destroy(L2);
		
		//return head of L1
		return head;
	}
}


//-----------------------------------------
//main function takes in command line
//-----------------------------------------
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
	int k;									//index for string line number, this is to prevent letter form of line number
	
	
	string argument = argv[1];				//command line argument in string form
	
	//parse the command line argument to get 2 text files' names
	istringstream ss (argument);			
	getline(ss,line,'=');
	getline(ss,line,';');
	filename1 = line;
	
	getline(ss,line,'=');
	getline(ss,line,'\0');
	filename2 = line;
	
	//open input text file, make sure no failed open, and add lines to list L1, then close this text file
	fin1.open(filename1.c_str());
	
	if (fin1.fail()) return -1;
	
	getline(fin1,xyz);
	List *L1 = new List(xyz); 
	
	while (getline(fin1,xyz))
	{
		L1->addTail(L1,xyz); 
	}
	
	//close input text file
	fin1.close();
	
	//open command text file and do command as in each case then close command text file
	fin1.open(filename2.c_str());
	if (fin1.fail()) return -1;
	
	while (getline(fin1,command))
	{
		//cout << command << endl;
		
		//make sure there is a string "=" or nothing will be operated
		if ((command.find("=") != string::npos))
		{
			//getting command keyword and the rest of line
			istringstream stst (command);
			getline(stst,line,'=');
			cmd = line;
			getline(stst,line,'\0');
			rest = line;
			
			//if the command is DELETING A LINE, this case works with certain line number only
			if (cmd == "deleteline")
			{
				k=0;
				
				//make sure no invalid line number in form of letter string
				for (unsigned i=0; i< rest.size(); i++) 
					if (!isdigit(rest.at(i))) k++;
				
				if (rest!="" && k==0)
				{
					istringstream(rest) >>num;
					
					//make sure line delete is exist
					int size = L1->count(L1);
					
					if (num<=size)
					{
						//cout << "delete line " << num << endl;
						L1 = L1->deleteNode(L1,num);
						//L1->display(L1);
					}
				}	
			}
			
			//if the command is INSERTING AN EMPTY LINE, this case works with non-empty number-form line number only
			else if (cmd == "insertline")
			{
				k=0;
				
				//make sure no invalid line number in form of letter string
				for (unsigned i=0; i< rest.size(); i++) 
					if (!isdigit(rest.at(i))) k++;
				
				if (rest!="" && k==0)
				{ 
					istringstream(rest) >>num;
					
					//make sure line insert is valid
					int size = L1->count(L1);
					
					if (num<=(size+1))
					{
						//cout << "insert line " << num << endl;
						L1 = L1->addMiddle(L1,"",num-1);
						//L1->display(L1); 
					}
				}
			}
			
			//if the command is DELETING A WORD
			else if (cmd == "deleteword")
			{
				istringstream sts (rest);
				
				//make sure there is ":" like the format, else nothing is operated
				if ((rest.find(":") != string::npos))
				{
					//get number of line
					getline(sts,line,':');
					numb = line; 
					
					k=0;
					
					//make sure no invalid line number in form of letter string
					for (unsigned i=0; i< numb.size(); i++) 
						if (!isdigit(numb.at(i))) k++;
					
					//again we won't work with emty letter-form line number, line number is converted to int 
					if (numb!="" && k==0) 
					{
						istringstream(numb) >>num;
						
						//get the delete word
						getline(sts,line,'\0');
						rest = line;
						
						//no acception for any space in delete word
						if (rest.find(" ")==string::npos) 
						{
							//cout << "delete word " << rest << " at line " << num << endl;
							L1 = L1->deleteWord(L1,rest,num);
							//L1->display(L1); 
							
						}
					}
				}
			}
			
			//if the command is INSERTING A WORD
			else 
			{
				istringstream iss (rest);
				
				//make sure there is ":" like the format, else nothing is operated
				if ((rest.find(":") != string::npos))
				{
					//get number of line
					getline(iss,line,':');
					numb = line;
					
					k=0;
					
					//make sure no invalid line number in form of letter string
					for (unsigned i=0; i< numb.size(); i++) 
						if (!isdigit(numb.at(i))) k++;
					
					//again we won't work with emty letter-form line number, line number is converted to int 
					if (numb!="" && k==0) 
					{
						istringstream(numb) >>num;
						
						//get the rest of the line
						getline(iss,line,'\0');
						rest = line;
						
						//no space allowed in rest
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
								
								//skip empty searchword and add insertword only with certain searchword
								if (searchword!="")
								{
									//cout << "Insert word " << insertword << " after searchword " << searchword << " at line " << num << endl;
									L1 = L1->insertWord(L1, searchword, insertword, num);
									//L1->display(L1);
								}
							
							
								//second case: insert a word to emty line 
								else
								{
									//cout << "Insert word " << insertword << " at line " << num << endl;
									L1 = L1->insertWord(L1, "\0", insertword, num);
									//L1->display(L1);
								}
							}
						}	
					}
				}
			}
		}
		//cout << endl;
	}
	
	//display list L1
	L1->display(L1); 
	
	//close command text file 
	fin1.close();
	
	//destroy list L1
	L1->destroy(L1);
	
	//ask user to hit any key for exit
	return 0;
}
