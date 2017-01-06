//Thanh Vo _ 1148524

//input=processlog.txt;size=5;scheduling=fifo

#include<iostream>
#include<sstream>
#include<fstream>
#include<algorithm>
#include<string>
#include<iomanip>
#include<vector>
#include<algorithm>

using namespace std;

struct input
{
	int at;
	int dur;
	bool operator < (const input &a) const
	{
		return (at < a.at);
	}
	input(int vAt, int vDur): at(vAt), dur(vDur) {};
};

struct Process
{
	int at;
	int dur;
	int id;
	string stt;
	Process() : at(0), dur(0), id(0), stt("") {};
};

class CQueue
{
	private:
		Process *p;
		int max;
		int front, rear;
	public:
		CQueue(int size);
		void enqueue (int vAt, int vDur, int vID, string stat);
		void dequeue();
		int getAT();
		void getDataFifo(int &vID, int &vDur);
		bool isEmpty();
		void getDataRR(int &vID, int &vDur, int &vAt, string &stat);
		int getATFront();
		int getATRear();
};

CQueue::CQueue(int size)
{
	max = size+1;
	p = new Process [size+1];
	front = rear = -1;
	p[0].at = 0;
	p[0].dur = 0;
	p[0].id = 0;
	p[0].stt = "";
}

void CQueue::enqueue (int vAt, int vDur, int vID, string stat)
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
	p[rear].stt = stat;
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

void CQueue::getDataRR(int &vID, int &vDur, int &vAt, string &stat)
{
	vID = p[front].id;
	vDur = p[front].dur;
	vAt = p[front].at;
	stat = p[front].stt;
}

int CQueue::getATFront()
{
	return p[front].at;
}

int CQueue::getATRear()
{
	return p[rear].at;
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
	string tpStt;
	int time;
	int end_queue_process;
	int error = 0;
	int duration[100];
	int finish[100];
	int offset=1;
	vector <input> a;
	int si = 0;
	
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
	
	
	ifstream fin1(file.c_str());
	
	ofstream fout1("ps-log.txt");
	
	if (fin1.fail()) return -1;
	
	while (getline(fin1,line))
	{
		if (line.find("#")!=string::npos)
		{
			fout1 << line << endl;
			continue;
		}
		
		if (line =="") continue;
		
		line.erase(remove(line.begin(),line.end(),' '),line.end());
		
		istringstream ss2(line);
		getline(ss2,line1,',');
		istringstream (line1) >> arTi;
		getline(ss2,line1,'\0');
		istringstream (line1) >> dura;
		
		a.push_back(input(arTi,dura));
		si++;
		
	}
	
	sort(a.begin(),a.end());
	for (int i=0;i<si; i++)
	{
		fout1 << a[i].at << ',' << a[i].dur << endl;
	}
	
	fin1.close();
	fout1.close();
	
		
	istringstream (size) >> s;
	
	ifstream fin("ps-log.txt");
	if (fin.fail()) return -1;
	
	ofstream fout("ps.txt");
	
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
			
			//start p1, the next 1s periods are exe
			if (pid==1)
			{
				q.enqueue(arTi,dura,pid,"start");
				cout << setw(4) << arTi <<  "\t" << setw(9) << pid << "\t" << setw(8) << dura << "\t" << "start" << endl;
				fout << setw(4) << arTi <<  "\t" << setw(9) << pid << "\t" << setw(8) << dura << "\t" << "start" << endl;
				
				for (int i = arTi+1; i < dura; i++)
				{
					cout << setw(4) << i <<  "\t" << setw(9) << pid << "\t" << setw(8) << dura << "\t" << "executing" << endl;
					fout << setw(4) << i <<  "\t" << setw(9) << pid << "\t" << setw(8) << dura << "\t" << "executing" << endl;
				}
				
				pid++;
			}
			
			//end the last process, start new process and exe stt for next 1s periods
			else
			{
				end_time= q.getAT();
				q.getDataFifo(tempID,tempDur);
				cout << setw(4) << end_time <<  "\t" << setw(9) << tempID << "\t" << setw(8) << tempDur << "\t" << "end" << endl;
				fout << setw(4) << end_time <<  "\t" << setw(9) << tempID << "\t" << setw(8) << tempDur << "\t"  << "end" << endl;
				q.dequeue();
				
				if (arTi<=end_time)
				{
					q.enqueue(end_time,dura,pid,"start");
					cout << setw(4) << end_time <<  "\t" << setw(9) << pid << "\t" << setw(8) << dura << "\t" << "start" << endl;
					fout << setw(4) << end_time <<  "\t" << setw(9) << pid << "\t" << setw(8) << dura << "\t"  << "start" << endl;
					
					for (int i = end_time+1; i < end_time + dura; i++)
					{
						cout << setw(4) << i <<  "\t" << setw(9) << pid << "\t" << setw(8) << dura << "\t" << "executing" << endl;
						fout << setw(4) << i <<  "\t" << setw(9) << pid << "\t" << setw(8) << dura << "\t" << "executing" << endl;
					}
				}
				
				else
				{
					q.enqueue(arTi,dura,pid,"start");
					cout << setw(4) << arTi <<  "\t" << setw(9) << pid << "\t" << setw(8) << dura << "\t" << "start" << endl;
					fout << setw(4) << arTi <<  "\t" << setw(9) << pid << "\t" << setw(8) << dura << "\t" << "start" << endl;
					
					for (int i = end_time+1; i < end_time + dura; i++)
					{
						cout << setw(4) << i <<  "\t" << setw(9) << pid << "\t" << setw(8) << dura << "\t" << "executing" << endl;
						fout << setw(4) << i <<  "\t" << setw(9) << pid << "\t" << setw(8) << dura << "\t" << "executing" << endl;
					}
				}
				pid++;
			}
			error = 0;
		}
		
		//pop the leftover process
		while (!q.isEmpty())
		{
			end_time= q.getAT();
			q.getDataFifo(tempID,tempDur);
			cout << setw(4) << end_time <<  "\t" << setw(9) << tempID << "\t" << setw(8) << tempDur << "\t" << "end" << endl;
			fout << setw(4) << end_time <<  "\t" << setw(9) << tempID << "\t" << setw(8) << tempDur << "\t"  << "end" << endl;
			q.dequeue();
		}
	}
	
	
	if (schd=="roundrobin")
	{
		CQueue q1(s);
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
			
			duration[offset] = dura;
			
			//id 1, start it and: end if duration < quantum, push back with exe stt if duration > quantum
			if (pid==1)
			{
				if(dura<=qt) 
				{
					q1.enqueue(arTi,dura,pid,"start");
					q1.enqueue(arTi+qt,0,pid,"end");
				}
				else
				{
					q1.enqueue(arTi,dura,pid,"start");
					q1.enqueue(arTi+qt,dura-qt,pid,"executing");
				}
				time=1;
				pid++;
			}
			
			//id not 1
			else
			{
				//while arrival time is larger than end time of current exe process, do the process at front of queue (4-element struct data)
				//push back leftover (if have) and dequeue this process
				while(arTi >= time && !q1.isEmpty())
				{
					q1.getDataRR(tempID, tempDur, tempAt, tpStt);
					if (tpStt == "start")
					{
						cout << setw(4) << tempAt <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << tpStt << endl;
						fout << setw(4) << tempAt <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << tpStt << endl;
							
						q1.dequeue();
						
						q1.getDataRR(tempID, tempDur, tempAt, tpStt);
						cout << setw(4) << tempAt <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << tpStt << endl;
						fout << setw(4) << tempAt <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << tpStt << endl;
						
						end_time = q1.getATRear()+qt;
					}
						
					else if (tpStt == "executing")
					{
						//duration = 0 means no leftover so end
						if (tempDur==0)
						{
							cout << setw(4) << tempAt <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << "end" << endl;
							fout << setw(4) << tempAt <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << "end" << endl;
						}
						
						else
						{
							cout << setw(4) << tempAt <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << tpStt << endl;
							fout << setw(4) << tempAt <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << tpStt << endl;
							end_time = q1.getATRear()+qt;
						}
					}
					
					//push back: 1/leftover, 2/end process-process with 0 duration
					if (tempDur>qt)
					{
						q1.enqueue(end_time,tempDur-qt,tempID,"executing");
					}
					
					else if (tempDur>0 && tempDur <= qt)
					{
						q1.enqueue(end_time,0,tempID,"executing");
					}
							
					q1.dequeue();
					time += qt;
						
				}
				
				//process that arrives far far that queue is empty before its arrival	
				if (arTi > time)
				{
					if(dura<=qt) 
					{
						q1.enqueue(arTi,dura,pid,"start");
						q1.enqueue(arTi+qt,0,pid,"end");
					}
					else
					{
						q1.enqueue(arTi,dura,pid,"start");
						q1.enqueue(arTi+qt,dura-qt,pid,"executing");
					}
				}
				
				//any process that comes soon before queue is empty	
				else
				{
					if(dura<=qt) 
					{
						q1.enqueue(end_time,dura,pid,"start");
						q1.enqueue(end_time+qt,0,pid,"end");
					}
					else
					{
						q1.enqueue(end_time,dura,pid,"start");
						q1.enqueue(end_time+qt,dura-qt,pid,"executing");
					}	
				}
				pid++;	
			}
			error = 0;
			offset++;
		}
		
		//pop the leftover process in queue while pushing back the leftover after an exe process	
		while (!q1.isEmpty())
		{
			q1.getDataRR(tempID, tempDur, tempAt, tpStt);
			if (tpStt == "start")
			{
				cout << setw(4) << tempAt <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << tpStt << endl;
				fout << setw(4) << tempAt <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << tpStt << endl;
							
				q1.dequeue();
						
				q1.getDataRR(tempID, tempDur, tempAt, tpStt);
				cout << setw(4) << tempAt <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << tpStt << endl;
				fout << setw(4) << tempAt <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << tpStt << endl;
						
				end_time = q1.getATRear()+qt;
			}
						
			else if (tpStt == "executing")
			{
				if (tempDur==0)
				{
					cout << setw(4) << tempAt <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << "end" << endl;
					fout << setw(4) << tempAt <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << "end" << endl;
				}
						
				else
				{
					cout << setw(4) << tempAt <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << tpStt << endl;
					fout << setw(4) << tempAt <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << tpStt << endl;
					end_time = q1.getATRear()+qt;
				}
			}
					
			if (tempDur>qt)
			{
				q1.enqueue(end_time,tempDur-qt,tempID,"executing");
			}
			else if (tempDur>0 && tempDur <= qt)
			{
				q1.enqueue(end_time,0,tempID,"executing");
			}
							
			q1.dequeue();	
		}
	}
	
	fin.close();
	fout.close();
	return 0;
}
