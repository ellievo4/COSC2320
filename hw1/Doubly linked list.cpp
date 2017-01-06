#include<iostream>
#include<string>
#include<cstring>
#include<sstream>
#include<cmath>
#include<cstdlib>

using namespace std;

struct Node
{
	int data;
	Node *prev;
	Node *next;
	Node (int value) : data(value), next(NULL), prev(NULL) {}
};


class List
{
	public:
		List();
		Node* getHead() const;
		Node* getTail() const;
		void addHead(int value);
		void addTail(int value);
		void reverse();
		int getLength();
		void display();
		void destroy();
		
	private:
		Node* head;
		Node* tail;
};

unsigned long long sum(List num1, List num2, int digit);

unsigned long long multiply(List num1, List num2, int digit);



int main()
{
	List e;
	e.addTail(12345678);
	//cout << e.getLength() << endl;
	//e.display();
	cout << endl;
	
	
	List f;
	f.addTail(89101112);
	//cout << f.getLength() << endl;
	//f.display();
	
	e.reverse();
	f.reverse();
	
	
	int digit;
	cout << "digit: ";
	cin >> digit;
	unsigned long long r1; unsigned long long r2;
	r1= sum(e,f,digit);
	r2= multiply(e,f,digit);
	cout <<r1 << endl;
	
	/*e.destroy();
	f.destroy();
	
	e.addTail(12);
	e.addTail(34);
	
	f.addTail(1);
	
	e.reverse();
	f.reverse();
	
	
	//int digit;
	cout << "digit: ";
	cin >> digit;
	//int r1,r2;
	r1= sum(e,f,digit);
	r2= multiply(e,f,digit);
	cout <<r1 << "\n" << r2 << endl;*/
	
	return 0;
}





List::List()
{
	head = NULL;
	tail = NULL;
}

//=======================================
Node* List::getHead() const
{
	return head;
}

//========================================
Node* List::getTail() const
{
	return tail;
}

//=========================================
void List::addHead(int value)
{
	Node *temp = new Node(value);
	
	if(head==NULL)
	{
		temp->prev = NULL;
		head = temp;
	}
	
	else
	{
		temp->next=head;
		head->prev=temp;
		head = temp;
		temp->prev = NULL;
	}
	
	Node* e=new Node (0);
	e=head;
	while(e->next!=NULL)
	{
		e=e->next;
	}
	tail =e;
}

//=============================================
int List::getLength()
{
	Node* len = new Node(0);
	int size = 0;
	len = head;
	while (len!=NULL)
	{
		size++;
		len = len->next;
	}
	return size;
}

//===============================================
void List::display()
{
	Node *temp = head;
	while (temp!=NULL)
	{
		cout << temp->data << " ";
		temp = temp->next;
	}
}

//=================================================
void List::destroy()
{
	Node *temp = head;
	while (temp!=NULL)
	{
		head=head->next;
		delete temp;
		temp = head;	
	}
	
	tail = head = NULL;
}

//===================================================
void List::addTail(int value)
{
	Node *s = new Node(0);
	Node *temp = new Node(0);
	temp->data = value;
	temp->next = NULL;
	
	if(head==NULL)
	{
		temp->prev = NULL;
		head = temp;
	}
	
	else 
	{
		s=head;
		while (s->next!=NULL)
			s = s->next;
		s->next = temp;
		temp->prev = s;
	}
	
	tail = temp;
}

//======================================
void List::reverse()
{
	Node* temp = NULL;
	Node* current = head;
	
	//swapping element
	while(current!=NULL)
	{
		temp = current->prev;
		current->prev = current->next;
		current->next = temp;
		current = current->prev;
	}
	
	//check if there is empty or 1-node list
	if (temp!=NULL)
	head = temp->prev;
}






unsigned long long sum(List num1, List num2, int digit)
{
	int size1, size2;
	unsigned long long r, re;
	int carry = 0;
	List result;
	
	Node* add1 = num1.getHead();
	Node* add2 = num2.getHead();
	
	//get 2 length	
	size1 = num1.getLength();
	size2 = num2.getLength();
	
	//2 lengths are different, insert 0 to the list with less nodes and insert to head
	if(size1!=size2)
	{
		int diff = abs(size1-size2);
		int k = 0;
		
		while (k<diff)
		{
			//addition 1 has less node
			if(size1<size2)
			{
				num1.addTail(0); k++;
			}
				
			//addition 2 has less node	
			else if (size1>size2)
			{
				num2.addTail(0); k++;
			}
		}
	}
		
	//do add, start with leftmost node 	
	while(add1!=NULL)
	{
		//add 2 nodes with carry
		r= (unsigned long long)add1->data + (unsigned long long)add2->data + carry; 
			
		//if sum > 100, take the last 2 number and fisrt 1 becomes carry
		if (r > pow(10.0,digit))
		{
			re = r - pow(10.0,digit);
			carry = (r - re)/pow(10.0,digit);
			result.addHead(re);
		}
			
		//if sum = 100, number becomes 0 and carry 1	
		else if (r==pow(10.0,digit))
		{
				carry = 1;
				result.addHead(0);
		}
		
		//if the two cases above are not matched then do this rest	
		else
		{
			result.addHead(r);
			carry = 0;
		}
		
		//shift pointer to previous node	
		add1 = add1->next;
		add2 = add2->next;
	}
	
	if (carry!=0) 
		result.addHead(carry);
	
	//display the list of result
	//result.display(); cout << endl;
	
	//get the tail of the list
	Node* res = result.getTail();
	//cout << res->data << endl;
	
	//calculate the result by multiply nodes with 1, 100, 1000, ...
	unsigned long long num=0;
	int i = 0;
	while(res!=NULL)
	{
		num = num + res->data * pow(10.0,i);
		res = res->prev;
		i=i+digit;
		//cout << num << endl;
	}
	
	return num;
}

//========================================================
unsigned long long multiply(List num1, List num2, int digit)
{
	unsigned long long re =0;		//for multiplication of 2 nodes result
	int div=0; 		//for the result after division
	unsigned long long  rem;		//for remainder of division
	unsigned long long r =0;		//for return result
	int k= 0;		//for indexes of list 2
	int i = 0;		//for index to get number after multiply list2 with 1 node of list 1
	unsigned long long num = 0;	//for number after multiply list2 with 1 node of list 1
	List result;	//list of result
		
	
	//get head of 2 list
	Node* add1 = num1.getHead();
	Node* add2 = num2.getHead();
	
	while(add2!=NULL)
	{
		//traverse through list 1 and multiply leftmost node of list 2 with all list 1
		while (add1!=NULL)
		{
			re = (unsigned long long)add2->data * (unsigned long long)add1->data + div;
			cout << re << endl;
			
			//if the result of multiplication has more digit than digit per node
			if(re >= pow(10.0,digit))
			{
				div = re/pow(10.0,digit);
				rem = re% (unsigned long long) pow(10.0,digit);
				result.addHead(rem);
			}
			
			//else less than digit per node
			else
			{
				result.addHead(re);
				div = 0;
			}
			
			//shift pointer to previous node of list 1
			add1=add1->next;	
		}
		
		//if div is not 0, add div as node at head and set div equal to 0
		if(div!=0) 
		{
			result.addHead(div);
			div = 0;
		}
		//get the tail of the list 1
		Node* res = result.getTail();
		//cout << res->data << endl;
		
		//calculate the result by multiply nodes with 1, 100, 1000, ...
		while(res!=NULL)
		{
			num = num + res->data * pow(10.0,i);
			res = res->prev;
			i=i+digit;
			//cout << num << endl;
		}
		
		//calculate overall multiplication result, destroy the result list and shift pointer of list 2 to previous node 
		r= r+num*pow(10.0,(digit*k));
		result.destroy();
		add2=add2->next;
		
		//get head of num1 again, reset num, i
		add1 = num1.getHead();
		num = 0;
		i=0;
		k++;
			
	}
	return r;	
}
