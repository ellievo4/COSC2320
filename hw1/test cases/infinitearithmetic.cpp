//Name: Thanh Vo
//PSID: 1148524

#include<iostream>
#include<cmath>
#include<fstream>
#include<string>
#include<cstring>
#include<sstream>
#include<cstdlib>

using namespace std;

//represent 1 node
struct Node
{
	string data;
	Node *prev;
	Node *next;
	Node (string value) : data(value), next(NULL), prev(NULL) {};
};

//contain functions and variables for a double linked list
class List
{
	public:
		List();
		Node* getHead() const;
		Node* getTail() const;
		void addHead(string value);
		void addTail(string value);
		void reverse();
		int getLength();
		void display();
		void destroy();
		~List();
		
	private:
		Node* head;
		Node* tail;
};

List sum(List num1, List num2, int digit);

List multiply(List num1, List num2, int digit);

//______________________________________________________________________
int main(int argc, char* argv[])
{
	//check for empty command line argument
	if(argc==1)
		cout << "no file enterred!" << endl;
	
	//stream in text file name
	ifstream fin;					//track stream input from text file
	string filename;				//track file name
	string argument;				//convert argument from CString to string
	string wholeThing;				//track the whole line of command line
	argument = argv[1];					
	istringstream ss(argument);		//split the command line
	getline(ss,wholeThing,'=');
	getline(ss,wholeThing,';');
	filename=wholeThing;
	
	//get number of digit per node from command line
	string digit;					//track digit per node in string form
	int digitPerNode;				//track digit per node in integer form
	getline(ss,wholeThing,'=');
	getline(ss,wholeThing,'\0');
	digit = wholeThing;
	istringstream (digit) >> digitPerNode;
	
	//open text file and make sure the text file is opened
	fin.open(filename.c_str());
	if(fin.fail())
		cout << "file cannot be open!" << endl;
	
	string str;					//track string input from text file
	char s1[100];				//track num 1 in C string
	char s2[100];				//track num 2 in c string
	char bef;					//track character that is before that character for operation
	int j , k;					//track index of c string num 1 and num 2
	j = k = 0;
	string str1; 				//track string num 1 
	string sstr1;				//track 1st string that have enough digit to divide by digit per node with remainder 0
	string sub1;				//use for spliting the 1st string into substring of size digit per node
	string str2;				//track string num 2
	string sstr2;				//track 2nd string that have enough digit to divide by digit per node with remainder 0
	string sub2;				//use for spliting the 2nd string into substring of size digit per node
	char ch;					//track operator
	int i = 0;					//track all index from line 143 to 206
	List num1;					//linked list stores num 1
	List num2;					// linked list store num 2
	List result1;				//result for add operation
	List result2;				//result for multiply operation

	while(!fin.eof())
	{
		//get a line of text and set bef equal to fist character of that line
		getline(fin,str); 
		if(str!="")
		{
			bef = str.at(0);
		
			//parse number 1 and number 2 from the line
			for(unsigned l = 0; l < str.length(); l++)
			{
				//if character sits after character and index of C string num 2 equal to 0 (load in num 1) or not equal to 0 (load in num 2)
				if (isdigit(str.at(l)) && isdigit(bef))
				{
					if(k==0)
					{
						s1[j] = bef = str.at(l);
						j++;
					}
				
					else
					{
						s2[k] = bef = str.at(l);
						k++;
					}
				}
			
				//first character of c string num 2
				else if (isdigit(str.at(l)) && !isdigit(bef))
				{
					s2[k] = bef = str.at(l);
					k++;
				}
			
				//load in operator
				else if (str.at(l) == '+' || str.at(l) == '*')
				{
					bef = ch = str.at(l);
				}
			
				//anything else, space for example
				else
					bef = str.at(l);
			}
		
			//terminate 2 c strings for num 1 and num 2 with null then convert them to string
			s1[j] = s2[k] = '\0';
			str1 = s1;
			str2 = s2;
		
			//edit string 1 by adding 0 at head so that it can fully fill node with max digit per node
			int size1 = str1.size();		//track size of number 1
			sstr1 = str1;
			if(size1 % digitPerNode != 0)
			{
				i=0;
				int rem = size1 % digitPerNode;
				int num = digitPerNode - rem;
				while(i<num)
				{
					sstr1 = "0" + sstr1;
					i++;
				}
			}
			
			//reset i
			i=0;
			
			//edit string 2 by adding 0 at head so that it can fully fill node with max digit per node
			int size2 = str2.size();		//track size of number 2
			sstr2 = str2;
			if(size2 % digitPerNode != 0)
			{
				int rem = size2 % digitPerNode;
				int num = digitPerNode - rem;
				while(i<num)
				{
					sstr2 = "0" + sstr2;
					i++;
				}
			}
			
			//reset i
			i = 0;
			int ssize1 = sstr1.size();
			
			//divide the string 1 into string containing digit per node numbers and store in list
			while (i<ssize1)
			{
				sub1 = sstr1.substr(i,digitPerNode);
				num1.addTail(sub1);
				i = i+digitPerNode;
			}
			
			//reset i
			i=0;
			int ssize2 = sstr2.size();
			
			//divide the string 2 into string containing digit per node numbers and store in list
			while (i<ssize2)
			{
				sub2 = sstr2.substr(i,digitPerNode);
				num2.addTail(sub2);
				i = i+digitPerNode;
			}
			
			//if operator is +, reverse 2 list, do operation and display result
			if(ch=='+')
			{
				num1.reverse();
				num2.reverse();
				result1 = sum(num1,num2,digitPerNode);
				cout << '\n' << str1 << " + " << str2 << " = ";
				result1.display();
				cout << endl;
			}
			
			//if operator is *, reverse two lists, do operation and display result
			if(ch=='*')
			{
				num1.reverse();
				num2.reverse();
				result2 = multiply(num1,num2,digitPerNode);
				cout << '\n' << str1 << " * " << str2 << " = ";
				result2.display();
				cout << endl;
			}
			
			//reset 2 lists and lists of results of add and multiply operation
			num1.destroy();
			num2.destroy();
			result1.destroy();
			result2.destroy();
			
			//reset j and k
			j = k = 0;
		}
	}
	
	//close file
	fin.close();
	
	//ask user to hit any key for exit
	return 0;
}


//constructor to initialize head and tail
List::List()
{
	head = NULL;
	tail = NULL;
}

//return the head of the node
Node* List::getHead() const
{
	return head;
}

//return the tail of the node
Node* List::getTail() const
{
	return tail;
}

//add node at head of list
void List::addHead(string value)
{
	Node *temp = new Node(value);
	
	//if empty list
	if(head==NULL)
	{
		temp->prev = NULL;
		head = temp;
	}
	
	//if list has head
	else
	{
		temp->next=head;
		head->prev=temp;
		head = temp;
		temp->prev = NULL;
	}
	
	//set tail
	Node* e=new Node ("");
	e=head;
	while(e->next!=NULL)
	{
		e=e->next;
	}
	tail =e;
}

//get length of the list
int List::getLength()
{
	Node* len = new Node("");
	int size = 0;
	len = head;
	while (len!=NULL)
	{
		size++;
		len = len->next;
	}
	return size;
}

//display the list
void List::display()
{
	Node *temp = head;			//copy of head
	Node *tempp = head;			//another copy of head
	int numb;					//track data of node in int
	int n=0;					//track count of numbers in nodes that are not 0 
	
	//check for list of all 0's
	while (temp!=NULL)
	{
		istringstream (temp->data) >> numb;
		if (numb != 0)
			n++;	
		temp = temp->next;
	}
	
	//if there is a number different than zero in the list
	if (n>0)
	{
		while (tempp!=NULL)
		{
			cout << tempp->data;
			tempp = tempp->next;
		}
	}
	
	// ekse the list becomes simple 0
	else
	{
		cout << "0";
	}
}

//clear the list
void List::destroy()
{
	Node *current = head;
	Node *temp = NULL;

	while (current!=NULL)
	{
		temp = current;
		current = current->next;
		temp = NULL;	
	}
	
	tail = head = NULL;
}

//add node at the tail of the list
void List::addTail(string value)
{
	Node *s = new Node("");
	Node *temp = new Node("");
	temp->data = value;
	temp->next = NULL;
	
	//if list is empty
	if(head==NULL)
	{
		temp->prev = NULL;
		head = temp;
	}
	
	//if list has head
	else 
	{
		s=head;
		while (s->next!=NULL)
			s = s->next;
		s->next = temp;
		temp->prev = s;
	}
	
	//set tail
	tail = temp;
}

//reverse the list, head becomes tail and tail becomes head
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

//destructor for list
List::~List()
{
	destroy();
}



//addition operator that takes in 2 lists and return their sum in form of a list
List sum(List num1, List num2, int digit)
{
	int size1, size2;			//track 2 sizes of lists
	int r;						//track result after + 2 nodes
	int re;						//track result after + 2 nodes if result contains more digit than number per nodes
	int carry = 0;				//track carry of addition
	List result;				//store result nodes
	string med;					//use for string data to store in result node
	
	
	//retrieve heads of the list
	Node* add1 = num1.getHead();
	Node* add2 = num2.getHead();
	
	//get 2 lengths of 2 lists
	size1 = num1.getLength();
	size2 = num2.getLength();
	
	//2 lengths are different, insert 0 to the list with less nodes and insert to tail
	if(size1!=size2)
	{
		int diff = abs(size1-size2);
		int k = 0;
		
		while (k<diff)
		{
			//addition 1 has less node
			if(size1<size2)
			{
				num1.addTail("0"); k++;
			}
				
			//addition 2 has less node	
			else if (size1>size2)
			{
				num2.addTail("0"); k++;
			}
		}
	}
	
	//get length of list num1 again
	size1 = num1.getLength();
	
	int numb1, numb2;		//track two add data values
	int numb3 = 0;			//track the checking of reaching head of result or not
	
	//do add, start with leftmost node 	
	while(add1!=NULL)
	{
		//convert node data to int and add 2 nodes with carry
		istringstream(add1->data) >> numb1;
		istringstream(add2->data) >> numb2;
		r= numb1 + numb2 + carry; 
			
		//if sum > 100, take the last digit-digit number and fisrt 1 becomes carry, convert data from int to string and store to result list 
		if (r > pow(10.0,digit))
		{
			re = r - (int)pow(10.0,digit);
			carry = (r - re)/(int)pow(10.0,digit);
			ostringstream convert;
			convert << re;
			med = convert.str();
			
			//if the number has less digit than digits per node then add 0 in front of it
			if (med.size()<digit)
			{
				for (int i = 0; i< (digit-med.size()); i++)
					med = "0" + med;
			}
			result.addHead(med);
		}
			
		//if sum = 100, number becomes 0 and carry 1, create string for 0 and add 0 to reach digit digits per node then store to result list
		else if (r==pow(10.0,digit))
		{
				int j = 1;
				carry = 1;
				med = "0";
				while (j<digit)
				{
					med = "0" + med;
					j++;
				}
				result.addHead(med);
		}
		
		//if the two cases above are not matched then do this rest which is convert data from int to string and store to result list	
		else
		{
			ostringstream con;
			con << r;
			med = con.str();
			
			//if the number has less digit than digits per node and it is not the head node then add 0 in front of it
			if (med.size()<digit && numb3<(size1-1))
			{
				for (int i = 0; i< (digit-med.size()); i++)
					med = "0" + med;
			}
			result.addHead(med);
			carry = 0;
		}
		
		//shift pointer to previous node and increase numb3	
		add1 = add1->next;
		add2 = add2->next;
		numb3++;
	}
	
	//convert carry to string and add to head of result list
	if (carry!=0)
	{ 
		ostringstream co;
		co << carry;
		med = co.str();
		result.addHead(med);
	}
	
	//return the result
	return result;
}

//multiply operator that takes in 2 lists and return their product in form of a list
List multiply(List num1, List num2, int digit)
{
	unsigned long long re =0;		//for multiplication of 2 nodes result
	int div=0; 						//for the result after division
	unsigned long long  rem;		//for remainder of division
	unsigned long long r =0;		//for return result
	int k= 0;						//for indexes of list 2
	unsigned long long num = 0;		//for number after multiply list2 with 1 node of list 1
	List result;					//list of result
	List medium;					//list of products of second, third,..., nth rightmost node in num2 with num1
	List tres;						//list of result after summing products of second, third,..., nth rightmost node in num2 with num1
	int numb1, numb2;				//store data of 2 nodes multiplied in form of int
	string med;						//use for string data to store in result and medium nodes
		
	tres.addHead("0");				//initial tres list with a node of 0
	
	//get head of 2 lists
	Node* add1 = num1.getHead();
	Node* add2 = num2.getHead();
	
		
	while(add2!=NULL)
	{
		//traverse through list 1 and multiply leftmost node of list 2 with all list 1
		while (add1!=NULL)
		{
			//convert string data of 2 nodes to int and multiply them then plus div
			istringstream(add1->data) >> numb1;
			istringstream(add2->data) >> numb2;
			re = (unsigned long long)numb2 * (unsigned long long)numb1 + div;
			
			//if the result of multiplication has more digit than digit per node
			if(re >= pow(10.0,digit))
			{
				div = re/pow(10.0,digit);
				rem = re% (unsigned long long) pow(10.0,digit);
	
				//convert data from int to string
				ostringstream conve;
				conve << rem;
				med = conve.str();
				
				//if the number has less digit than digits per node then add 0 in front of it
				if (med.size()<digit)
				{
					for (int i = 0; i< (digit-med.size()); i++)
						med = "0" + med;
				}
				
				//if it is the product of rightmost node of num2 with num1, store in list result
				if (k==0) result.addHead(med);
				
				//else store in list medium 
				else medium.addHead(med);
			}
			
			//else less than digit per node
			else
			{
				//convert data from int to string
				ostringstream conver;
				conver << re;
				med = conver.str();
				
				//if the number has less digit than digits per node then add 0 in front of it
				if (med.size()<digit)
				{
					for (int i = 0; i< (digit-med.size()); i++)
						med = "0" + med;
				}
				
				//if it is the product of rightmost node of num2 with num1, store in list result
				if(k==0) result.addHead(med);
				
				//else store in list medium 
				else medium.addHead(med);
				
				//reset div
				div = 0;
			}
			
			//shift pointer to previous node of list 1
			add1=add1->next;	
		}
		
		//if div is not 0, add div as node at head and set div equal to 0
		if(div!=0) 
		{
			//convert div from int to string and store in med
			ostringstream cv;
			cv << div;
			med = cv.str();
			
			//if it is the product of rightmost node of num2 with num1, store in list result
			if(k==0) result.addHead(med);
			
			//else store in list medium 
			else medium.addHead(med);
			
			div = 0;
		}
		
		//if this is the products of second, third,..., nth rightmost node in num2 with num1, add 0 at tail depending of number of line it is on
		if(k>0)
		{
			for (int i= 0; i<k; i++)
			{
				medium.addTail("0");
			}
		
			//reverse 2 lists for addition and take addition
			tres.reverse(); 
			medium.reverse(); 
			tres = sum(tres,medium,digit);
		}
		
		//shift pointer of num2 to next node
		add2=add2->next;
		
		//get head of num1 again, destroy list medium and increment of k
		add1 = num1.getHead();
		medium.destroy();
		k++;
			
	}
	
	//reserve 2 lists for addition and take addition
	result.reverse();
	tres.reverse();
	result = sum(result,tres,digit);
		
	//return the multiplication result
	return result;	
}
