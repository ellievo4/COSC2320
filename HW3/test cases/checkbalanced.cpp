//Name: Thanh Vo
//PSID: 1148524
//Program Assignment 3

#include <iostream>		//provide: cin,cout
#include <string>		//provide: string
#include <sstream>		//provide: istringstream
#include <fstream>		//provide: ifstream

//declare environment
using namespace std;

struct Node
{
	char data;								//track value of each node
	Node *next;								//track next pointer of a node
	Node () : data('\0'), next(NULL) {};	//constructor for this struct
};

class Stack
{
	private: 
		Node *top;						//track top of stack
		
	public:
		Stack();						//constructor
		void push(char value);			//add 1 element
		void pop();						//delete 1 element
		char getTop();					//return the top element
		void printStack();				//print stack to screen
		bool isStackEmpty();			//return true if stack is empty		
};

//constructor
Stack::Stack()
{
	top = NULL;
}

//add new element to top
void Stack::push(char value)
{
	Node *temp = new Node();
	temp->data = value;
	temp->next = NULL;
	
	if (top==NULL) top=temp;
	
	else
	{ 
		temp->next = top;
		top = temp;
	}
}

//delete an element at top
void Stack::pop()
{
	if(top==NULL) {
		//cout << "stack is empty!" << endl;
		return;
	}

	else
	{
		Node *temp = top;
		top = top->next;
		delete temp;
	}
}

//get the top element
char Stack::getTop()
{
	if(top==NULL) 
		return '\0';
	else return top->data;
}

//print out the stack to screen
void Stack::printStack()
{
	if(top==NULL) {
		//cout << "stack is empty!" << endl;
		return;
	}
	else 
	{
		Node *curr = top;
		while (curr!=NULL)
		{
			cout << curr->data << " ";
			curr = curr->next;
		}
	}
}

//return true if stack is empty which means top is NULL
bool Stack::isStackEmpty()
{
	return (top==NULL);
}

//--------------------------------------------------------
//main function starts here
//--------------------------------------------------------
int main (int argc, char* argv[])
{
	//make sure there is a command line
	if(argc==1) return -1;
	
	string argument = argv[1];		//convert command line to string
	char ch;						//get each character in the text file
	string filename;				//track name of input file
	int line_numb = 1;				//track number of line
	Stack s1;						//track the stack
	string open = "([{";			//open braces exclude " and '
	string close = ")]}";			//close braces except " and '
	int pos = 0;					//position of ')' or ']' or '}' in string close
	int singQuote = 1;				//position of single quote 
	int doubQuote = 1;				//position of double quote
	int error_numb = 0;				//track number of errors
	
	//get the input file name
	istringstream ss (argument);
	getline(ss,filename,'=');
	getline(ss,filename,'\0');
	
	//declare stream input variable and provide file name for streaming, if fail, exit the program
	ifstream fin(filename.c_str());
	if (fin.fail()) return -1;
	
	//stream in each character of the input file
	while (fin.get(ch))
	{
		//cout << ch;
		
		if (open.find(ch)!=string::npos)
		{
			s1.push(ch);
		}
		
		//if ch is double quote, treat double quote at odd position as open symbol and at even position as close symbol	
		else if (ch == (char)34)
		{
			//if double quote is at even position, if ch equals top of stack, pop; otherwise, error and ignore; doubQuote increases in each case
			if (doubQuote%2==0)
			{
				if(ch==s1.getTop()) 
				{
					s1.pop();
					doubQuote++;
				}
				else 
				{
					cout << "Error at line " << line_numb << ": top()=" << s1.getTop() << ";current=" << ch << endl;
					doubQuote++;
					error_numb++;
				}
			}

			//if double quote is at odd position, if stack is empty, push ch and increase doubQuote by 1 else do 2 cases
			else 
			{
				if(s1.isStackEmpty())
				{
					s1.push(ch);
					doubQuote++;
				}

				//if stack is not empty, if ch equals top of stack, pop; otherwise, push ch; doubQuote increases in each case
				else
				{
					if(ch==s1.getTop()) 
					{
						s1.pop();
						doubQuote++;
					}

					else 
					{
						s1.push(ch);
						doubQuote++;
					}

				}
			}
		}
		
		//if ch is single quote, treat single quote at odd position as open symbol and at even position as close symbol
		else if (ch == (char)39)
		{
			//if single quote is at even position, if ch equals top of stack, pop; otherwise, error and ignore; singQuote increases in each case
			if (singQuote%2==0)
			{
				if(ch==s1.getTop()) 
				{
					s1.pop();
					singQuote++;
				}
				else 
				{
					cout << "Error at line " << line_numb << ": top()=" << s1.getTop() << ";current=" << ch << endl;
					singQuote++;
					error_numb++;
				}
			}
			
			//if single quote is at odd position, if stack is empty, push ch and increase singQuote by 1 else do 2 cases
			else 
			{
				if(s1.isStackEmpty())
				{
					s1.push(ch);
					singQuote++;
				}
				
				//if stack is not empty, if ch equals top of stack, pop; otherwise, push ch; singQuote increases in each case
				else
				{
					if(ch==s1.getTop()) 
					{
						s1.pop();
						singQuote++;
					}

					else 
					{
						s1.push(ch);
						singQuote++;
					}
				}
			}
		}
		
		//if ch is close braces such as (, [, or {
		else if (close.find(ch)!=string::npos)
		{
			pos=close.find(ch);
			
			//no open braces were in stack <=> stack is empty
			if (s1.isStackEmpty())
			{
				cout << "Error at line " << line_numb << ": top()=UNDEFINED;current=" << ch << endl;
				error_numb++;
			}
			
			//stack is not empty and the top of stack equals to corresponding open brace of this close brace	
			else if (!s1.isStackEmpty() && s1.getTop()==open.at(pos))
			{
				s1.pop();
			}
			
			//stack is not empty and the top of stack does not equal to corresponding open brace of this close brace	
			else if (!s1.isStackEmpty() && s1.getTop()!=open.at(pos))
			{
				cout << "Error at line " << line_numb << ": top()=" << s1.getTop() << ";current=" << ch << endl;
				error_numb++;
			}
		}
		
		//if ch is new line character, line_numb + 1
		else if (ch=='\n')
			line_numb++;
	}
	
	//if the program end with empty stack and zero error, then program is correct	
	if (error_numb==0 && s1.isStackEmpty()) cout << "correct" << endl;
	
	//else if stack is not empty then there are lacking of close braces, print out error message with top of stack and incorrect symbol $
	if (!s1.isStackEmpty())
		cout << "Error at line" << line_numb << ": top()=" << s1.getTop() << ";current=$" << endl;
	
	//close the input file
	fin.close();
	
	//tell user to hit any key
	return 0;
}

