//Thanh Vo_1148524

#include<iostream>
#include<sstream>
#include<fstream>
#include<algorithm>
#include<string>
#include<iomanip>
#include<algorithm>
#include<vector>

using namespace std;

//===============================================================
//store the data in the first text file which is unsorted
//===============================================================
struct input
{
	int at;			//arrival time
	int dur;		//duration
	
	//overide the operator < so that it only compare 2 arrival times 
	bool operator < (const input &a) const
	{
		return (at < a.at);
	}
	//constructor
	input(int vAt, int vDur): at(vAt), dur(vDur) {};
};

//===============================================================
//store the set of data for each process in the system
//===============================================================
struct Process
{
	int at;			//arrival time
	int dur;		//duration
	int id;			//id
	string stt;		//status
	//constructor
	Process() : at(0), dur(0), id(0), stt("") {};
};

//===============================================================
//class circular queue to operate the sets of data for processes 
//===============================================================
class CQueue
{
	private:
		Process *p;														//array-based queue
		int max;														//max size of array
		int front, rear;												//front and rear index
	public:
		CQueue(int size);												//constructor
		void enqueue (int vAt, int vDur, int vID, string stat);			//insert new element to front of queue
		void dequeue();													//delete an element at the front of queue
		int getAT();													//get end time of process at rear position
		void getDataFifo(int &vID, int &vDur);							//get front data for fifo case
		bool isEmpty();													//true if queue is empty, false otherwise
		void getDataRR(int &vID, int &vDur, int &vAt, string &stat);	//get front data for roundrobin case
		int getATFront();												//get end time of process at front position
};

//constructor, note that this array starts at 1 so all 0th elements are 0 and ""
//param:size
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

//insert at front of the array
//param: arrive time, duration, id and status
void CQueue::enqueue (int vAt, int vDur, int vID, string stat)
{
	if((front==0 && rear == max-1) || front==rear+1)
	{
		cout << "Queue is Full\n";
		return;
	}
	
	//empty array
	if (front==-1)
		front = rear = 1;
	
	//add at rear end or add at middle 
	else
	{
		if(rear==max-1) rear = 1;
		else rear = rear+1;
	}
	
	//assign value
	p[rear].at = vAt;
	p[rear].dur = vDur; 
	p[rear].id = vID;	
	p[rear].stt = stat;
}

//delete at front of queue
//param: none
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

//get end-time of rear process
//return: arrival time + duration of rear process
int CQueue::getAT()
{
	return (p[rear].at + p[rear].dur);
}

//check if queue is empty
//return: true if empty, false otherwise
bool CQueue::isEmpty()
{
	if ( front == -1 ) return true;
	else return false;
}

//get data set for fifo case
//param: reference arrival time and referrence duration
void CQueue::getDataFifo(int &vID, int &vDur)
{
	vID = p[front].id;
	vDur = p[front].dur;
}

//get data set for roundrobin case
//param: reference arrival time and referrence duration, referrence ID and referrence status
void CQueue::getDataRR(int &vID, int &vDur, int &vAt, string &stat)
{
	vID = p[front].id;
	vDur = p[front].dur;
	vAt = p[front].at;
	stat = p[front].stt;
}

//get front process's end-time
//return: front arrival time + front duration
int CQueue::getATFront()
{
	return (p[front].at + p[front].dur);
}

//=====================================
//main function, return 1 for completion, -1 for exit immediately, param: array of command line and size of this array
//=====================================
int main (int argc, char* argv[])
{
	//exit if there is no cmd line
	if (argc==1) return -1;
	
	string argument = argv[1];				//convert cmd into string
	string line1;							//1st parsing cmd line and parsing each line of text file				
	string line2;							//2nd parsing cmd line for round robin
	string file;							//text file name
	string size;							//size of queue in string
	int s;									//size of queue in int
	string schd;							//type of scheduling			
	string quantum;							//quantum in string
	int qt;									//quantum in int
	string line;							//text lines
	int arTi;								//arrival time
	int dura;								//duration
	int pid =1;								//id of process start at 1
	int end_time;							//end time of one process
	int tempAt, tempDur,tempID;				//temporary arrival time, duration and ID
	string tpStt;							//temporary status		
	int time;								//time after executing 1 process, use to track whether a process comes in during 1 execution of another in RR
	int end_queue_process;					//in RR, end-time of rear process
	int error = 0;							//number of error in a text line
	int duration[100];						//duration of each process, since we don't output the remaining seconds but a fixed duration 
	int finish[100];						//track if right after start, the finish will either executing or end
	int offset=1;							//same function as id, act as index for array duration and finish
	vector <input> a;						//array a use to store a set of input for sorting
	int si = 0;								//size of a
	
	//parse command line to get input file name, size of queue, type of scheduling, 
	//fifo has no quantum, RR has quatum, normally every quantum is 1 
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
	
	//stream in input text file and stream out ps-log.txt which has arrival time in order 	
	ifstream fin1(file.c_str());
	
	ofstream fout1("ps-log.txt");
	
	//exit if fail to open input file
	if (fin1.fail()) return -1;
	
	//stream in text lines, line with # is comment so be streamed out immediately, ignore empty lines, erase spaces in line
	while (getline(fin1,line))
	{
		if (line.find("#")!=string::npos)
		{
			fout1 << line << endl;
			continue;
		}
		
		if (line =="") continue;
		
		line.erase(remove(line.begin(),line.end(),' '),line.end());
		
		//get a set of arrival time and duration and place this in array vector a, size of a ++ 
		istringstream ss2(line);
		getline(ss2,line1,',');
		istringstream (line1) >> arTi;
		getline(ss2,line1,'\0');
		istringstream (line1) >> dura;
		
		a.push_back(input(arTi,dura));
		si++;
		
	}
	
	//sort the array a from begin til end then stream out the ordered list
	sort(a.begin(),a.end());
	for (int i=0;i<si; i++)
	{
		fout1 << a[i].at << ',' << a[i].dur << endl;
	}
	
	//close 2 text files
	fin1.close();
	fout1.close();
	
	//convert queue size from string to int	
	istringstream (size) >> s;
	
	//open text file called ps-log.txt and output file ps.txt
	ifstream fin("ps-log.txt");
	if (fin.fail()) return -1;
	
	ofstream fout("ps.txt");
	
	//if the scheduling is fifo- first in first out
	if (schd == "fifo")
	{
		CQueue q(s);	//queue of process
		//output and stream out names of columns
		cout << "time" << "\t" << "processId" << "\t" << "duration" << "\t" << "action" << endl;
		fout << "time" << "\t" << "processId" << "\t" << "duration" << "\t" << "action" << endl;
		
		//get text lines
		while (getline(fin,line))
		{
			//ignore comment
			if (line.find("#")!=string::npos || line=="") continue;
			
			//check error
			for (unsigned i = 0; i<line.size(); i++)
			{
				if (!isdigit(line.at(i)) && line.at(i)!=',' && line.at(i)!=' ') error++;
			}
			
			if (error>0) continue;
			
			//stream in arrival time and duration
			line.erase(remove(line.begin(),line.end(),' '),line.end());
			istringstream ss2(line);
			getline(ss2,line1,',');
			istringstream (line1) >> arTi;
			getline(ss2,line1,'\0');
			istringstream (line1) >> dura;
			
			//if this is first process, put in queue, output as well as stream out start report and executing reports for the next few 1's, id++
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
			
			//if this is not first process
			else
			{
				//get end-time of rear process, cout and fout end report and dequeue
				end_time= q.getAT();
				q.getDataFifo(tempID,tempDur);
				cout << setw(4) << end_time <<  "\t" << setw(9) << tempID << "\t" << setw(8) << tempDur << "\t" << "end" << endl;
				fout << setw(4) << end_time <<  "\t" << setw(9) << tempID << "\t" << setw(8) << tempDur << "\t"  << "end" << endl;
				q.dequeue();
				
				//if the next process comes during execution of the previous process, put in queue with start at end_time
				//output as well as stream out start report and executing reports for the next few 1's, id++
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
				
				//if the next process comes after execution of the previous process, put in queue with start at its own arrival time
				//output as well as stream out start report and executing reports for the next few 1's, id++
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
		
		//pop the remaining in the queue, report end for this remaining and dequeue
		while (!q.isEmpty())
		{
			end_time= q.getAT();
			q.getDataFifo(tempID,tempDur);
			cout << setw(4) << end_time <<  "\t" << setw(9) << tempID << "\t" << setw(8) << tempDur << "\t" << "end" << endl;
			fout << setw(4) << end_time <<  "\t" << setw(9) << tempID << "\t" << setw(8) << tempDur << "\t"  << "end" << endl;
			q.dequeue();
		}
	}
	
	//if scheduling is fifo
	if (schd=="roundrobin")
	{
		CQueue q1(s), q2(100);				//2 queues: 1 holds processes for execution, other 1 holds push-back process with remaining duration 
		cout << "time" << "\t" << "processId" << "\t" << "duration" << "\t" << "action" << endl;
		fout << "time" << "\t" << "processId" << "\t" << "duration" << "\t" << "action" << endl;
		
		//stream in text line
		while (getline(fin,line))
		{
			//ignore comment and check error
			if (line.find("#")!=string::npos || line=="") continue;
			
			for (unsigned i = 0; i<line.size(); i++)
			{
				if (!isdigit(line.at(i)) && line.at(i)!=',' && line.at(i)!=' ') error++;
			}
			
			if (error>0) continue;
			
			//get arrive time and duration out of text line
			line.erase(remove(line.begin(),line.end(),' '),line.end());
			istringstream ss2(line);
			getline(ss2,line1,',');
			istringstream (line1) >> arTi;
			getline(ss2,line1,'\0');
			istringstream (line1) >> dura;
			
			//duration for this id is a fixed duration every report
			duration[offset] = dura;
			
			//id = 1, if duration larger than quantum, queue this process to q1 with duration quantum, and q2 with duration dura-qt, finish = 0
			// if duration <= quantum, queue this process to q1 with duration, finish = 1. 
			//id++
			if (pid==1)
			{
				if(dura<=qt) 
				{
					q1.enqueue(arTi,dura,pid,"start");
					time = dura;
					finish[offset]=1;
				}
				else
				{
					q1.enqueue(arTi,qt,pid,"start");
					q2.enqueue(0,dura-qt,pid,"executing");
					time = qt;
					finish[offset]=0;
				}
				pid++;
			}
			
			//other id's > 1
			else
			{
				//if a process is coming during another process's execution
				//if duration larger than quantum, queue this process to q1 with duration quantum and arrival time at rear process's end-time 
				//									and q2 with duration dura-qt, finish = 0
				//if duration <= quantum, queue this process to q1 with duration and arrival time at rear process's end-time, finish = 1
				//id++
				if (arTi < time)
				{
					if(dura<=qt) 
					{
						q1.enqueue(q1.getAT(),dura,pid,"start");
						finish[offset]=1;
					}
					else
					{
						q1.enqueue(q1.getAT(),qt,pid,"start");
						q2.enqueue(0,dura-qt,pid,"executing");
						finish[offset]=0;
					}	
					pid++;
				}
				
				//if a process is coming after another process's execution
				else if (arTi>=time)
				{
					//while the arrival time is larger than or equal the front process's end-time, and q2 is not empty
					//get front data of q1 (ID, arrival time, duration and status), get end_time of front process and end-time (end_queue_process) 
					//			of rear process
					//cout and fout the process based on status
					while(arTi >= q1.getATFront() && !q2.isEmpty())
					{
						q1.getDataRR(tempID, tempDur, tempAt,tpStt);
						end_time = q1.getATFront();
						end_queue_process = q1.getAT();
						
						//status is start, cout and fout start process plus executing or end report basing on finish = 0(executing) and 1(end)
						if (tpStt == "start")
						{
							cout << setw(4) << tempAt <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << tpStt << endl;
							fout << setw(4) << tempAt <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << tpStt << endl;
							
							if (finish[tempID]==0)
							{
								cout << setw(4) << end_time <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << "executing" << endl;
								fout << setw(4) << end_time <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << "executing" << endl;
							}
							
							else
							{
								cout << setw(4) << end_time <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << "end" << endl;
								fout << setw(4) << end_time <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << "end" << endl;
							}
						}
						
						//status is executing, cout and fout executing report at the end of the process (if process starts at 3s,
						//then this report is writing "4 ID duration executing")
						else if (tpStt == "executing")
						{
							cout << setw(4) << end_time <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << tpStt << endl;
							fout << setw(4) << end_time <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << tpStt << endl;
						}
						
						//status is end, cout and fout end report at the end of the process (if process starts at 3s,
						//then this report is writing "4 ID duration end")
						else if (tpStt == "end")
						{
							cout << setw(4) << end_time <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << tpStt << endl;
							fout << setw(4) << end_time <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << tpStt << endl;
						}
						
						//dequeue q1	
						q1.dequeue();
						
						//take data set from q2 place at end of q1, the remaining of this set after this placing is pushed back to q2
						//if duration of this data set <= qt, status is "end", else status is "executing"
						//dequeue this set from q2
						q2.getDataFifo(tempID,tempDur);
						if (tempDur<=qt)
							q1.enqueue(end_queue_process,tempDur,tempID,"end");
						else
						{
							q1.enqueue(end_queue_process,qt,tempID,"executing");
							q2.enqueue(0,tempDur-qt,tempID,"executing");
						}
						q2.dequeue();
					}
					
					//if in case of the loop ending by empty q2, arrival time of this process is still larger than end-time of front process
					//if duration larger than quantum, queue this process to q1 with duration quantum and its own arrival time
					//									and q2 with duration dura-qt, finish = 0
					//if duration <= quantum, queue this process to q1 with duration and its own arrival time, finish = 1
					if (arTi > q1.getATFront())
					{
						if(dura<=qt) 
						{
							q1.enqueue(arTi,dura,pid,"start");
							finish[offset]=1;
						}
						else
						{
							q1.enqueue(arTi,qt,pid,"start");
							q2.enqueue(0,dura-qt,pid,"executing");
							finish[offset]=0;
						}	
					}
					
					//otherwise, arrival time of this streamed in process is less than or equal end-time of front process
					//if duration larger than quantum, queue this process to q1 with duration quantum and arrival time at rear process's end-time 
					//									and q2 with duration dura-qt, finish = 0
					//if duration <= quantum, queue this process to q1 with duration and arrival time at rear process's end-time, finish = 1
					else
					{
						if(dura<=qt) 
						{
							q1.enqueue(q1.getAT(),dura,pid,"start");
							finish[offset]=1;
						}
						else
						{
							q1.enqueue(q1.getAT(),qt,pid,"start");
							q2.enqueue(0,dura-qt,pid,"executing");
							finish[offset]=0;
						}	
					}
					
					//id++ and time is now the end-time of front process
					pid++;	
					time = q1.getATFront();
				}
			}
			//reset error and offset++
			error = 0;
			offset++;
		}
		
		//pop-front every leftover of the remaining q2 until q2 is empty
		//1/ get front of q1, cout and fout based on status, dequeue q1
		//2/ take data set from q2 place at end of q1, the remaining of this set after this placing is pushed back to q2
		//		if duration of this data set <= qt, status is "end", else status is "executing"
		//		dequeue this set from q2
		while (!q2.isEmpty())
		{
			q1.getDataRR(tempID, tempDur, tempAt,tpStt);
			end_time = q1.getATFront();
			end_queue_process = q1.getAT();
			if (tpStt == "start")
			{
				cout << setw(4) << tempAt <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << tpStt << endl;
				fout << setw(4) << tempAt <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << tpStt << endl;
				
				if (finish[tempID]==0)
				{			
					cout << setw(4) << end_time <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << "executing" << endl;
					fout << setw(4) << end_time <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << "executing" << endl;
				}
				else 
				{
					cout << setw(4) << end_time <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << "end" << endl;
					fout << setw(4) << end_time <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << "end" << endl;
				}
			}
						
			else if (tpStt == "executing")
			{
				cout << setw(4) << end_time <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << tpStt << endl;
				fout << setw(4) << end_time <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << tpStt << endl;
			}
						
			else if (tpStt == "end")
			{
				cout << setw(4) << end_time <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << tpStt << endl;
				fout << setw(4) << end_time <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << tpStt << endl;
			}
							
			q1.dequeue();
			
			q2.getDataFifo(tempID,tempDur);
			if (tempDur<=qt)
				q1.enqueue(end_queue_process,tempDur,tempID,"end");
			else
			{
				q1.enqueue(end_queue_process,qt,tempID,"executing");
				q2.enqueue(0,tempDur-qt,tempID,"executing");
			}
			q2.dequeue();
		}	
		
		//pop-front every leftover of the remaining q2 until q2 is empty
		//get front of q1, cout and fout based on status, dequeue q1
		while (!q1.isEmpty())
		{
			q1.getDataRR(tempID, tempDur, tempAt,tpStt);
			end_time = q1.getATFront();
			if (tpStt == "start")
			{
				cout << setw(4) << tempAt <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << tpStt << endl;
				fout << setw(4) << tempAt <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << tpStt << endl;
				
				if (finish[tempID]==0)
				{			
					cout << setw(4) << end_time <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << "executing" << endl;
					fout << setw(4) << end_time <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << "executing" << endl;
				}
				else 
				{
					cout << setw(4) << end_time <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << "executing" << endl;
					fout << setw(4) << end_time <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << "executing" << endl;
				}
			}
						
			else if (tpStt == "executing")
			{
				cout << setw(4) << end_time <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << tpStt << endl;
				fout << setw(4) << end_time <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << tpStt << endl;
			}
						
			else if (tpStt == "end")
			{
				cout << setw(4) << end_time <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << tpStt << endl;
				fout << setw(4) << end_time <<  "\t" << setw(9) << tempID << "\t" << setw(8) << duration[tempID] << "\t" << tpStt << endl;
			}
							
			q1.dequeue();
		}		
	}
	
	//close 2 text files
	fin.close();
	fout.close();
	
	//hit any keys for completion exit
	return 0;
}
