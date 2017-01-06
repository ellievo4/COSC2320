#include <iostream>

using namespace std;

struct Node
{
	char data;				//track value of each node
	Node *next;				//track next pointer of a node
	Node () : data('\0'), next(NULL) {};
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
		cout << "stack is empty!" << endl;
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
	if(top==NULL) {
		cout << "stack is empty!" << endl;
	}
	else return top->data;
}

//print out the stack to screen
void Stack::printStack()
{
	if(top==NULL) {
		cout << "stack is empty!" << endl;
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

bool Stack::isStackEmpty()
{
	return (top==NULL);
}


int main()
{
	Stack s1;
	
	//add to stack A B C and D
	s1.push('A');
	s1.push('B');
	s1.push('C');
	s1.push('D');
	
	//print stack
	s1.printStack();
	cout << endl;
	
	//delete the last 2 element
	s1.pop();
	s1.pop();
	
	//print stack
	s1.printStack();
	cout << endl;
	
	//delete the last element and print top
	s1.pop();
	cout << s1.getTop() << endl;
	
	//del other 2 element
	s1.pop();
	s1.pop();
	
	//print stack
	s1.printStack();
	
	//exit
	return 0;
}

