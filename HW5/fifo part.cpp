//input=processlog.txt;size=5;scheduling=fifo

#include<iostream>
#include<sstream>
#include<fstream>
#include<algorithm>
#include<string>
#include<iomanip>

using namespace std;

struct Process
{
	int at;
	int dur;
	int id;
	Process() : at(0), dur(0), id(0) {};
};

class CQueue
{
	private:
		Process *p;
		int max;
		int front, rear;
	public:
		CQueue(int size);
		void enqueue (int vAt, int vDur, int vID);
		void dequeue();
		int getAT();
		void getDataFifo(int &vID, int &vDur);
		bool isEmpty();
		void getDataRR(int &vID, int &vDur, int &vAt);
		int getATFront();
};

CQueue::CQueue(int size)
{
	max = size+1;
	p = new Process [size+1];
	front = rear = -1;
	p[0].at = 0;
	p[0].dur = 0;
	p[0].id = 0;
}

void CQueue::enqueue (int vAt, int vDur, int vID)
{
	if((front==0 && rear == max-1) || front==rear+1)
	{
		cout << "Queue is Full\n";
		return;
	}
	
	if (front==-1)
		front = rear = 1;
	
	else
	{
		if(rear==max-1) rear = 1;
		else rear = rear+1;
	}

	p[rear].at = vAt;
	p[rear].dur = vDur; 
	p[rear].id = vID;	
}

void CQueue::dequeue()
{
	if (front == -1) return;
	if (front == rear) front = rear = -1;
	else
	{
		if (front == max-1) front = 1;
		else front = front + 1;
	}
}

int CQueue::getAT()
{
	return (p[rear].at + p[rear].dur);
}

bool CQueue::isEmpty()
{
	if ( front == -1 ) return true;
	else return false;
}

void CQueue::getDataFifo(int &vID, int &vDur)
{
	vID = p[front].id;
	vDur = p[front].dur;
}

void CQueue::getDataRR(int &vID, int &vDur, int &vAt)
{
	vID = p[front].id;
	vDur = p[front].dur;
	vAt = p[front].at;
}

int CQueue::getATFront()
{
	return (p[front].at + p[front].dur);
}


int main (int argc, char* argv[])
{
	if (argc==1) return -1;
	
	
	string argument = argv[1];
	string line1;							//1st parsing cmd line				
	string line2;							//2nd parsing cmd line for round robin
	string file;							//text file
	string size;							//size of queue in string
	int s;									//size of queue in int
	string schd;							//type of scheduling			
	string quantum;							//quantum in string
	int qt;									//quantum in int
	string line;							//text lines
	int arTi;								//arrival time
	int dura;								//duration
	int pid =1;								//id of process
	int end_time;							//end time of one process
	int tempAt, tempDur,tempID;
	int time;
	int end_queue_process;
	int error = 0;
	
	istringstream ss (argument);
	getline(ss,line1,'=');
	getline(ss,line1,';');
	file = line1;
	getline(ss,line1,'=');
	getline(ss,line1,';');
	size = line1;
	getline(ss,line1,'=');
	getline(ss,line1,'\0');
	
	if(line1.find(";")!=string::npos)
	{
		istringstream ss1 (line1);
		getline(ss1,line2,';');
		schd = line2;
		getline(ss1,line2,'=');
		getline(ss1,line2,'\0');
		quantum = line2;
		istringstream (quantum) >> qt;
	}
	
	else
		schd = line1;
		
	istringstream (size) >> s;
	
	ifstream fin(file.c_str());
	if (fin.fail()) return -1;
	
	ofstream fout("ps-log.txt");
	
	if (schd == "fifo")
	{
		CQueue q(s);
		cout << "time" << "\t" << "processId" << "\t" << "duration" << "\t" << "action" << endl;
		fout << "time" << "\t" << "processId" << "\t" << "duration" << "\t" << "action" << endl;
		
		while (getline(fin,line))
		{
			if (line.find("#")!=string::npos || line=="") continue;
			
			for (unsigned i = 0; i<line.size(); i++)
			{
				if (!isdigit(line.at(i)) && line.at(i)!=',' && line.at(i)!=' ') error++;
			}
			
			if (error>0) continue;
			
			line.erase(remove(line.begin(),line.end(),' '),line.end());
			istringstream ss2(line);
			getline(ss2,line1,',');
			istringstream (line1) >> arTi;
			getline(ss2,line1,'\0');
			istringstream (line1) >> dura;
			
			if (pid==1)
			{
				q.enqueue(arTi,dura,pid);
				cout << setw(4) << arTi <<  "\t" << setw(9) << pid << "\t" << setw(8) << dura << "\t" << "start" << endl;
				fout << setw(4) << arTi <<  "\t" << setw(9) << pid << "\t" << setw(8) << dura << "\t" << "start" << endl;
				pid++;
			}
			
			else
			{
				end_time= q.getAT();
				q.getDataFifo(tempID,tempDur);
				cout << setw(4) << end_time <<  "\t" << setw(9) << tempID << "\t" << setw(8) << tempDur << "\t" << "end" << endl;
				fout << setw(4) << end_time <<  "\t" << setw(9) << tempID << "\t" << setw(8) << tempDur << "\t"  << "end" << endl;
				q.dequeue();
				
				if (arTi<=end_time)
				{
					q.enqueue(end_time,dura,pid);
					cout << setw(4) << end_time <<  "\t" << setw(9) << pid << "\t" << setw(8) << dura << "\t" << "start" << endl;
					fout << setw(4) << end_time <<  "\t" << setw(9) << pid << "\t" << setw(8) << dura << "\t"  << "start" << endl;
				}
				
				else
				{
					q.enqueue(arTi,dura,pid);
					cout << setw(4) << arTi <<  "\t" << setw(9) << pid << "\t" << setw(8) << dura << "\t" << "start" << endl;
					fout << setw(4) << arTi <<  "\t" << setw(9) << pid << "\t" << setw(8) << dura << "\t" << "start" << endl;
				}
				pid++;
			}
			error = 0;
		}
		
		while (!q.isEmpty())
		{
			end_time= q.getAT();
			q.getDataFifo(tempID,tempDur);
			cout << setw(4) << end_time <<  "\t" << setw(9) << tempID << "\t" << setw(8) << tempDur << "\t" << "end" << endl;
			fout << setw(4) << end_time <<  "\t" << setw(9) << tempID << "\t" << setw(8) << tempDur << "\t"  << "end" << endl;
			q.dequeue();
		}
	}
	
	fin.close();
	fout.close();
	return 0;
}
