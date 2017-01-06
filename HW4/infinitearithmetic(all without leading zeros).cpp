#include<iostream>		//provides: cin, cout
#include<string>		//provides: string
#include<fstream>		//provides: ifstream
#include<sstream>		//provides: istringstream
#include<algorithm>		//provides: remove
#include<cstring>		//provides: isdigit

//setting up the environment
using namespace std;

//=======================================
//==============NODE=====================
//=======================================
template <typename T>
struct Node
{
	T data;			//value of a node
	Node *next;		//pointer to next
	Node *prev;		//pointer to previous
};

//=======================================
//==============STACK HEADER=============
//=======================================
template <class T>
class Stack
{
	private:
		Node<T> *top;				//top of stack
	
	public:
		Stack() {top = NULL;}		//stack constructor
		void push(T value);			//push value into stack
		void pop();					//delete top node
		T getTop();					//get top node data
		bool isEmpty();				//check the emptiness of stack
		void print();				//display stack
};

//=======================================
//==========STACK DEFINITIONS============
//=======================================

//push value into top of stack. Parameter: value. Return: none
template <class T>
void Stack<T>::push (T value)
{
	Node<T> *temp = new Node<T>;
	temp->data = value;
	temp->next = NULL;
	temp->prev = NULL;
	
	if (top==NULL) top = temp;
	else
	{
		temp->next = top;
		top->prev =temp;
		top =temp;
	}
}

//pop the top node of stack. Parameter: none. Return: none
template <class T>
void Stack<T>::pop()
{
	Node<T> *temp = new Node<T>;
	if (top==NULL) return;
	temp = top;
	top = top->next;
	if (top)
	top->prev = NULL;
	delete temp;
}

//get the data of top node. Parameter: none. Return: top node value
template <class T>
T Stack<T>::getTop()
{
	return top->data;
}

//check if the stack is empty. Parameter: none. Return: true/false
template <class T>
bool Stack<T>::isEmpty()
{
	return (top==NULL);
}

//display the whole stack. Parameter: none. Return: none
template <class T>
void Stack<T>::print()
{
	if (top==NULL) return;
	Node<T> *curr = top;
	while (curr!=NULL)
	{
		cout << curr->data;
		curr = curr->next;
	}
}

//=======================================
//======FUNCTION PROTOTYPES==============
//=======================================
bool isOperator(char ch);
string convertToPostfix (string str);
int priority (char ch1, char ch2);
string evaluation (string str);
string editNegative(string str);
string output(string str);

//=======================================
//==============MAIN=====================
//=======================================
int main(int argc, char* argv[])
{
	//make sure there is a command line
	if(argc==1) return -1;
	
	ifstream inFile;					//track stream text file
	string filename;					//track tokens of command line
	string argument = argv[1];			//convert command line argument to type string
	string line;						//track each line in text file
	string postfix = "";				//track the postfix of arithmetic expression
	string file;						//track the file name
	string digit;						//track the digit per node
	
	//parse command line to extract the filename
	istringstream ss(argument);			
	getline(ss,filename,'=');
	getline(ss,filename,';');
	file = filename;
	if(argument.find(';')!=string::npos)
	{
		getline(ss,filename,'=');
		getline(ss,filename,'\0');
		digit = filename;
	}
	
	string result;						//result of arithmetic expression
	string expression;					//the expresion in output screen
	
	//open text file, exit program if fail to open
	inFile.open(file.c_str());
	if (inFile.fail()) return -1;
	
	//stream in text file
	while (getline(inFile,line))
	{
		//no deal with empty line
		if (line=="") continue;
		
		//erase all white space in the line
		line.erase(remove(line.begin(),line.end(),' '), line.end());
		
		//extract the expression in output screen and save for later
		expression = output(line);
		
		//if expression contains negative numbers, edit the expression 
		line = editNegative(line);
		
		//cout << line << endl;
		
		//convert infix form to postfix form
		postfix = convertToPostfix(line);
		//cout << postfix << endl;
		
		//evaluate the postfix and calculate the result
		result=evaluation(postfix);
		
		//output to screen the expression saved earlier and result
		cout << expression << result << endl;
	}
	
	//close text file
	inFile.close();
	
	//ask user to hit any key for exit
	return 0;
}

//=======================================
//=========FUNCTION DEFINITIONS==========
//=======================================

//check if the current character is operator + - * or /
//parameter: char ch, return: true/false
bool isOperator(char ch)
{
	if (ch=='+'|| ch=='-'|| ch=='*'|| ch=='/')
		return true;
	return false;
}

//check if ch2 has higher or lower priority operator than ch1
//parameter: char ch1, ch2; return: 1 if higher, -1 if lower, 0 otherwise
int priority (char ch1, char ch2)
{
	if ((ch1 == '+' || ch1 == '-') && (ch2 == '*' || ch2 == '/')) return 1;
	else if ((ch1 == '*' || ch1 == '/') && (ch2 == '+' || ch2 == '-')) return -1;
	return 0;
}

//convert the expression from infix form to postfix form
//parameter: string str, return: postfix form of string str
string convertToPostfix (string str)
{
	Stack<char> s;					//track all the operators and ( )
	string postfix = "";			//track the return postfix string
	for (unsigned i=0; i< str.size(); i++)
	{
		//if number
		if (isdigit(str.at(i)))
		{
			//if character is at end of string, add to postfix
			if(i+1==str.size()) 
				postfix.insert(postfix.end(),1,str.at(i));
			//else: if next character is number, add this char to postfix; else add this char and a space to postfix
			else
			{
				if (isdigit(str.at(i+1))) 
					postfix.insert(postfix.end(),1,str.at(i));
				else 
				{
					postfix.insert(postfix.end(),1,str.at(i));
					postfix.insert(postfix.end(),1,' ');
				}
			}
		}
		
		//if character is ( push to stack
		else if (str.at(i)=='(')
		{
			s.push(str.at(i));
		}
		
		//if )
		else if (str.at(i)==')')
		{
			//if top of stack is (, simply pop it and exit this loop
			while (!s.isEmpty())
			{
				if (s.getTop()=='(')
				{
					s.pop(); break;
				}
				//add the top of stack to postfix sring and pop the stack 
				postfix.insert(postfix.end(),1,s.getTop());
				s.pop();
			}
		}
		
		//if operator + - * /
		else if (isOperator(str.at(i)))
		{
			//while stack is not empty, top of stack is not ( and this char has lower priority than top of stack
			//insert the top of stack to postfix, pop top right after
			while (!s.isEmpty() && s.getTop()!='(' && priority(s.getTop(),str.at(i))<=0)
			{
				postfix.insert(postfix.end(),1,s.getTop());
				s.pop();
			}
			//push this char to stack
			s.push(str.at(i));
		}
	}
	
	//each of remainings in stack is added to string postfix and popped right away
	while (!s.isEmpty())
	{
		postfix.insert(postfix.end(),1,s.getTop());
		s.pop();
	}
	
	//return the postfix string
	return postfix;
}

//evaluate the postfix string
//parameter: string str, return the result value of postfix str
string evaluation (string str)
{
	Stack <string> s;					//track all the numbers used in expression
	string operand="";					//operand in form of string
	signed long long numb1; 			//number after the operator in form of long long
	string num1;						//number after the operator in form of string
	signed long long numb2; 			//number before the operator in form of long long
	string num2;						//number after the operator in form of string
	signed long long res;				//result value of one arithmetic operation in form of long long
	string outcome;						//result value of one arithmetic operation in form of string
	
	for (unsigned i = 0; i<str.size(); i++)
	{
		//if space skip
		if (str.at(i)==' ') continue;
		
		//if operator
		else if (isOperator(str.at(i)))
		{
			//get each of 2 tops of the stack, pop it right after that, convert from string to signed long long these two operands 
			num1 = s.getTop(); s.pop(); istringstream (num1) >> numb1; 
			num2 = s.getTop(); s.pop(); istringstream (num2) >> numb2; 
			//set up operation for each case of +,-,*,/
			switch (str.at(i))
			{
				case '+': res = numb2 + numb1; break;
				case '-': res = numb2 - numb1; break;
				case '*': res = numb2 * numb1; break;
				case '/': res = numb2 / numb1; break;	
			}
			//convert result from signed long long to string and store in stack
			ostringstream convert;
			convert << res;
			outcome = convert.str();
			s.push(outcome);
		}
		//if number
		else
		{
			//character is at end of string, add it to string operand and push operand to stack, and reset the string
			if(i+1==str.size())
			{ 
				operand.insert(operand.end(),1,str.at(i));
				s.push(operand);
				operand = "";
			}
			
			//else, if next character is number, add char to operand; else add char to operand, push operand to stack and reset operand
			else
			{
				if (isdigit(str.at(i+1))) 
					operand.insert(operand.end(),1,str.at(i));
				else 
				{
					operand.insert(operand.end(),1,str.at(i));
					s.push(operand);
					operand = "";
				}
			}
		}
	}
	//return top of the stack which is the result of the whole expression
	return s.getTop();
}

//edit the string so that any negative number becomes (0-<number>)
//parameter: string str, return: eddited string
string editNegative(string str)
{
	string edit= "";		//track return string
	int neg = 0;			//track the appearance of negative sign
	for (unsigned i = 0; i< str.size(); i++)
	{
		//if character at index i is ( or ) or * or / add it to edit
		if (str.at(i)=='(' || str.at(i)==')' || str.at(i)=='*' || str.at(i)=='/')
			edit.insert(edit.end(),1,str.at(i));
		
		//if character at index i is -
		else if (str.at(i)=='-')
		{
			//if character represent sign, add (0<char> to edit, neg becomes 1
			if (i==0 || isOperator(str.at(i-1)) || str.at(i-1)=='(')
			{
				edit.insert(edit.end(),1,'(');
				edit.insert(edit.end(),1,'0');
				edit.insert(edit.end(),1,str.at(i));
				neg =1;
			}
			//else just add char to string edit
			else 
				edit.insert(edit.end(),1,str.at(i));
		}
		
		//if character at index i is + 
		else if (str.at(i)=='+')
		{
			//if character represents sign, ignore it else add it to edit
			if (i==0 || isOperator(str.at(i-1)) || str.at(i-1)=='(') continue;
			else
				edit.insert(edit.end(),1,str.at(i));
		}
		
		//if character at index i is number
		else
		{
			//if at end of string
			if(i+1==str.size())
			{
				//if number is negative, add char and ) to string edit
			 	if (neg==1)
			 	{
			 		edit.insert(edit.end(),1,str.at(i));
			 		edit.insert(edit.end(),1,')');
			 		neg = 0;
				}
				//else not negative, add char to string edit
				else
					edit.insert(edit.end(),1,str.at(i));
			}
			
			//if not at end of string
			else
			{
				//if next character is digit, add this character to string edit 
				if (isdigit(str.at(i+1))) 
					edit.insert(edit.end(),1,str.at(i));
					
				//else, if number is negative add char and ) to edit, else add char to string edit
				else 
				{
					if (neg==1)
			 		{
			 			edit.insert(edit.end(),1,str.at(i));
			 			edit.insert(edit.end(),1,')');
			 			neg = 0;
					}
					else
						edit.insert(edit.end(),1,str.at(i));
				}
			}
		}
	}
	//return the editted string
	return edit;
}

//edit the expression output to screen which is the part before '='
//parameter: string str. return: string after edit
string output(string str)
{
	string out = "";			//track the return string
	string operand = "";		//track the operands in expression
	
	for(unsigned i= 0; i<str.size(); i++)
	{
		//character at index i is digit
		if (isdigit(str.at(i)))
		{
			//at the last index, add the char to operand, erase leading zeros in operand,
			//set operand = 0 if it is empty and then add operand and = to out
			if (i+1==str.size())
			{
				operand.insert(operand.end(),1,str.at(i));
				operand.erase(0,min(operand.find_first_not_of('0'),str.size()-1));
				if (operand == "") operand = "0";
				out += operand;
				out += " = ";
				operand = "";
			}
			else
			{
				//if next char is not digit
				if(!isdigit(str.at(i+1)))
				{
					//add number to operand, erase leading zeros in operand
					operand.insert(operand.end(),1,str.at(i));
					operand.erase(0,min(operand.find_first_not_of('0'),str.size()-1));
					//if operand is empty <=> 0, add operand to out
					if (operand == "") operand = "0";
					out += operand;
					//if next char is operator, add space
					if (isOperator(str.at(i+1)))
						out += " ";
					//reset operand
					operand = "";
				}
				//else just add char to operand
				else
					operand.insert(operand.end(),1,str.at(i));
			}
		}
		
		//if character at index i is )
		else if (str.at(i)==')')
		{
			//if at end of string, add char and = to out
			if (i+1==str.size())
			{
				out.insert(out.end(),1,str.at(i));
				out += " = ";
			}
			//else add char and a space to out
			else
			{
				out.insert(out.end(),1,str.at(i));
				out.insert(out.end(),1,' ');
			}
		}
		
		//if character at index i = ( just add char to out
		else if (str.at(i)=='(')
			out.insert(out.end(),1,str.at(i));
		
		//
		else if (str.at(i)=='-')
		{
			if (i==0 || isOperator(str.at(i-1)) || str.at(i-1)=='(')
				out.insert(out.end(),1,str.at(i));
			else
			{
				out.insert(out.end(),1,str.at(i));
				out.insert(out.end(),1,' ');
			}
		}
		
		//if character at index i is + 
		else if (str.at(i)=='+')
		{
			//if it represents sign then ignore it
			if (i==0 || isOperator(str.at(i-1)) || str.at(i-1)=='(')
				continue;
			//else it is operator, add it and a space to out
			else
			{
				out.insert(out.end(),1,str.at(i));
				out.insert(out.end(),1,' ');
			}
		}
		
		//if character at index i is * or / add it and a space to out
		else
		{
			out.insert(out.end(),1,str.at(i));
			out.insert(out.end(),1,' ');
		}
	}
	
	//return the editted string
	return out;
}

