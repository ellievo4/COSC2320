#include<iostream>
#include<string>
#include<fstream>
#include<sstream>

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

int main()
{
	Stack<string> s;
	
	s.push("a");
	s.push("b");
	s.push("c");
	s.push("d");
	s.print();
	cout << '\n';
	s.pop();
	s.pop();
	s.pop();
	s.pop();
	s.print();
	
	return 0;
}

