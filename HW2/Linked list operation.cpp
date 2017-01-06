#include <iostream>
#include <string>

using namespace std;

class List
{
	private: 
		string data;
		List *next, *prev;
		
	public:
		List(string value)
		{
			data = value;
			next = NULL;
			prev = NULL;
		}
		List* findHead(List *c);
		List* findEnd(List *c);
		void addTail(List *c, string value);
		void addHead(List *c, string value);
		void display(List *c);
		void destroy(List *c);
		List* addMiddle(List *c, string value, int pos);
		int count(List *c);
		int search(List *c,string value, int i);
		List* deleteNode(List *c, int pos);
		
		string getStr(List *c);
		
		List* searchPos(List *c, int pos,int i);
};

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

void List::addTail(List *c, string value)
{
	List *end = findEnd(c);
	List* temp = new List(value);
	end->next = temp;
	temp->prev = end;
	end = temp;
}

void List::addHead(List *c, string value)
{
	List *head = findHead(c);
	List *temp = new List(value);
	temp->next=head;
	head->prev=temp;
	head = temp;
	temp->prev = NULL;
}

void List::display(List *c)
{
	if(c==NULL) return;
	else
	{
		cout << c->data << " ";
		display (c->next);
	}
}

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

List* List::addMiddle(List *c, string value, int pos)
{
	List *curr = new List("");
	List *head = findHead(c);
	List *temp = new List(value); 
	
	List*p = findHead(c);
	int length = count(p);
	
	curr = head;
	
	if(pos==1)
	{
		if(curr->data=="")
		{
			curr->data = value;
			return head;
		}
		
		else
		{
			if(curr->next==NULL) 
			{
				addTail(curr,value);
				return curr;
			}
			
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
	
	else if (pos==length)
	{
		addTail(c,value);
		p = findHead(c);
		return p;
	}
		
	curr = searchPos(curr,pos,1); //traverse from 1 to pos, get curr at pos using searchPos function
		
	temp->next = curr->next; 
	curr->next->prev=temp;
	curr->next = temp;
	temp->prev = curr;
	return head;	
}

int List::count(List *c)
{
	if (c==NULL) return 0;
	else 
	{
		return (1+count(c->next));
	}
}

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

List* List::deleteNode(List *c, int pos)
{
	int i=0;
	List *head = findHead(c);
	List *curr = new List("");
	List *temp = new List("");
	curr = head;
	int len = count(c);
	
	if (pos==1)
	{
		curr=curr->next;
		//delete head;
		curr->prev = NULL;
		return curr;	
	}
	
	else if (pos== len)
	{
		addTail(c, " ");
		head = findHead(c);
		curr = searchPos(curr,pos-1,1); //traverse from 1 to pos-1, get curr at pos-1 using searchPos function
		temp = curr->next;
		curr->next = temp->next;
		temp->next->prev = curr;
		delete temp;
		return head;
	}
	
	else
	{
		curr = searchPos(curr,pos-1,1); //traverse from 1 to pos-1, get curr at pos-1 using searchPos function
		temp = curr->next;
		curr->next = temp->next;
		temp->next->prev = curr;
		delete temp;
		return head;	
	}
}

//fixed
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
		}
	}
}

string List::getStr(List *c)
{
	if (c==NULL) return "\0";
	else 
		return (c->data + " " + getStr(c->next));
}


//===========================================================
int main()
{
	//1st list
	List *str = new List("I");
	/*str->addTail(str, "want");
	str->addTail(str, "cookies");
	str->display(str);
	cout << endl;
	
	//search for word want
	cout << str->search(str,"want",1) << endl;
	
	//destroy
	str->destroy(str);*/
	
	//2nd list
	str = new List("cookies");
	str->addHead(str, "want");
	str->addHead(str, "really");
	str->addHead(str, "I");
	List* c = str->findHead(str);
	str->display(c);
	cout << endl;
	
	/*//search for word want
	cout << str->search(c,"hello",1) << endl;
	int k ; 
	k= str->search(c,"hello",1);
	cout << str->search(c,"cookies",1) << endl;
	k=str->search(c,"cookies",1);
	int i = str->search(c,"want",1);
	 
	//add node A after word want in 2nd list 
	str = str->addMiddle(str,"A",i);
	str->display(str);
	cout << endl;
	
	//delete word want
	str = str->deleteNode(str,1);
	str->display(str);
	cout << endl;
	
	//get the whole sentence
	string line = str->getStr(str);
	cout << line << endl;*/
	
	/*List *str = new List("end");
	str = str->addMiddle(str,"bye",1);
	str = str->deleteNode(str,1);
	str = str->findEnd(str);
	str->addTail(str,"Teddy");
	str->display(str);*/
	
	/*List *str = new List("one");
	str->addTail(str,"two");
	str->addTail(str,"three");
	str->addTail(str,"four");
	str->addTail(str,"five");
	str->addTail(str,"six");
	str->addTail(str,"seven");
	str->addTail(str,"eight");
	str->addTail(str,"nine");
	str->addTail(str,"ten");
	str->addTail(str,"eleven");
	str->addTail(str,"twelve");
	str->display(str);
	cout << endl;
	
	str = str->deleteNode(str,1);
	str = str->deleteNode(str,11);
	str = str->deleteNode(str,1);
	str = str->deleteNode(str,9);
	str = str->deleteNode(str,1);
	
	str->display(str);*/
	
	return 0;
}
