#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<algorithm>
#include<cstring>

using namespace std;

template <typename T>
struct Node
{
	T data;
	Node *next;
	Node *prev;
};

template <class T>
class Stack
{
	private:
		Node<T> *top;
	
	public:
		Stack() {top = NULL;}
		void push(T value);
		void pop();
		T getTop();
		bool isEmpty();
		void print();
};

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

template <class T>
T Stack<T>::getTop()
{
	return top->data;
}

template <class T>
bool Stack<T>::isEmpty()
{
	return (top==NULL);
}

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





bool isOperator(char ch);
string convertToPostfix (string str);
int priority (char ch1, char ch2);
string editNegative(string str);







int main(int argc, char* argv[])
{
	//make sure there is a command line
	if(argc==1) return -1;
	
	ifstream inFile;
	string filename;
	string argument = argv[1];
	string line;
	string postfix = "";
	istringstream ss(argument);
	getline(ss,filename,'=');
	getline(ss,filename,'\0');
	
	inFile.open(filename.c_str());
	
	while (getline(inFile,line))
	{
		if (line=="") continue;
		
		line.erase(remove(line.begin(),line.end(),' '), line.end());
		
		line = editNegative(line);
		cout << line << endl;
		postfix = convertToPostfix(line);
		cout << postfix << endl;
		
	}
	
	
	return 0;
}


bool isOperator(char ch)
{
	if (ch=='+'|| ch=='-'|| ch=='*'|| ch=='/')
		return true;
	return false;
}


int priority (char ch1, char ch2)
{
	if ((ch1 == '+' || ch1 == '-') && (ch2 == '*' || ch2 == '/')) return 1;
	else if ((ch1 == '*' || ch1 == '/') && (ch2 == '+' || ch2 == '-')) return -1;
	return 0;
}




string convertToPostfix (string str)
{
	Stack<char> s;
	string postfix = "";
	for (unsigned i=0; i< str.size(); i++)
	{
		if (isdigit(str.at(i)))
		{
			if(i+1==str.size()) 
				postfix.insert(postfix.end(),1,str.at(i));
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
		
		else if (str.at(i)=='(')
		{
			s.push(str.at(i));
		}
		
		else if (str.at(i)==')')
		{
			while (!s.isEmpty())
			{
				if (s.getTop()=='(')
				{
					s.pop(); break;
				}
				postfix.insert(postfix.end(),1,s.getTop());
				s.pop();
			}
		}
		
		else if (isOperator(str.at(i)))
		{
			while (!s.isEmpty() && s.getTop()!='(' && priority(s.getTop(),str.at(i))<=0)
			{
				postfix.insert(postfix.end(),1,s.getTop());
				s.pop();
			}
			s.push(str.at(i));
		}
	}
	
	while (!s.isEmpty())
	{
		postfix.insert(postfix.end(),1,s.getTop());
		s.pop();
	}
	
	return postfix;
}

string editNegative(string str)
{
	string edit= "";
	int neg = 0;
	for (unsigned i = 0; i< str.size(); i++)
	{
		if (str.at(i)=='(' || str.at(i)==')' || str.at(i)=='*' || str.at(i)=='/')
			edit.insert(edit.end(),1,str.at(i));
		
		else if (str.at(i)=='-')
		{
			if (i==0 || isOperator(str.at(i-1)) || str.at(i-1)=='(')
			{
				edit.insert(edit.end(),1,'(');
				edit.insert(edit.end(),1,'0');
				edit.insert(edit.end(),1,str.at(i));
				neg =1;
			}
			else 
				edit.insert(edit.end(),1,str.at(i));
		}
		
		else if (str.at(i)=='+')
		{
			if (i==0 || isOperator(str.at(i-1)) || str.at(i-1)=='(') continue;
			else
				edit.insert(edit.end(),1,str.at(i));
		}
		
		else
		{
			if(i+1==str.size())
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
			else
			{
				if (isdigit(str.at(i+1))) 
					edit.insert(edit.end(),1,str.at(i));
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
	return edit;
}

