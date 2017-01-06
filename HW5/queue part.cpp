#include <iostream>
#include <fstream>

using namespace std;

struct Process
{
	int id;
	int at;
	int dur;
	Process (): id(0), at(0), dur(0) {};
};

class CQueue
{
	private:
		Process *p;
		int front, rear;
		int max;
	public:
		CQueue(int size)
		{
			max = size;
			p = new Process [size];
			rear = front = -1;
		}
		void enqueue(int id, int at, int dur);
		void dequeue();
		int getAT();
		int getFrontAT();
		bool isEmpty();
		void displayRear();
		void displayFront1();
};

void CQueue::enqueue(int id, int at, int dur)
{
	if ((front == 0 && rear == max-1) || (front == rear+1))
	{
		cout << "\nQueue is Full\n";
		return;
	}
	else
	{
		if(front==-1)
		{
			front = 0;
			rear = 0;
		}
		else
		{
			if (rear == max-1)
			{
				rear = 0;
			}
			else
			{
				rear = rear + 1;
			}
		}
	}
	p[rear].id = id;
	p[rear].at = at;
	p[rear].dur = dur;
}

void CQueue::dequeue()
{
	if (front == -1) return;
	if (front == rear)
	{
		front = rear = -1;
	}
	else
	{
		if (front == max-1) front = 0;
		else front = front + 1;
	}
}

int CQueue::getAT()
{
	return (p[rear].at+p[rear].dur);
}

int CQueue::getFrontAT()
{
	return (p[front].at+p[rear].dur);
}

bool CQueue::isEmpty()
{
	if (front==-1 && rear==-1) return true;
	else return false;
}

void CQueue::displayFront1()
{
	if (front == -1) return;
	cout << p[front].id << "\t\t   " << p[front].dur << "\t\t"; 
}

void CQueue::displayRear()
{
	if (front == -1) return;
	cout << p[rear].at << "\t\t   " << p[rear].id << "\t\t   " << p[rear].dur << "\t\t"; 
}

int main()
{
	CQueue q(5);
	int numb = 1;
	int end_time;
	
	cout << "time" << "\t\t" << "process id" << "\t" << "duration" << "\t" << "action" << endl; 
	
	q.enqueue(numb,0,8);
	q.displayRear();
	cout << "start" << endl;
	numb++;
	
	end_time = q.getAT();
	q.enqueue(numb,end_time,2);
	q.displayRear();
	cout << "start" << endl;
	
	cout << end_time << "\t\t   "; 
	q.displayFront1();
	cout << "end" << endl;
	q.dequeue();
	
	end_time = q.getAT();
	cout << end_time << "\t\t   "; 
	q.displayFront1();
	cout << "end" << endl;
	q.dequeue();
	
	system ("Pause");
	return 0;
}
