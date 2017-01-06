//Thanh Vo_1148524

//filelist=filename.txt;sort=slow

//library list
#include<iostream>		//provide: cin, cout
#include<string>		//provide: string
#include<sstream>		//provide: istringstream
#include<fstream>		//provide: ifstream

//set environment for program
using namespace std;

//=============================================
//a struct that stores value of i (row), j (col) and value at row i column j
//=============================================
struct Elem
{
	int row;		//represent i
	int col;		//represent j
	int value;		//represent the value from i to j
	//constructor that set i and j to 0 and their distance to 1
	Elem() :row(0), col(0), value(1) {};
};

//=============================================
//class graph that stores 2 arrays E (edge) and G* (gr), 
//and functions to compute E, G* and index to link that has most in- and out-degree
//=============================================
class Graph
{
	private: 
		Elem *edge;				//1-D binary array of i's, j's and distances between i's and corresponding j's (denoted as E's)
		Elem *gr;				//1-D binary array which is result of edge + edge^2 + edge^3 (denoted as G*)
		int eCap, gCap;			//actual capacity of 2 arrays edge and gr
		string sortType;		//sort type: slow/fast depending on command line
		
	public:
		Graph (string sType);												//constructor
		void stream (string file, string p[], int size, int index);			//take .html filename from .txt file and stream in content
		void displayEdge ();												//print out the 1-D array of edge
		void display(Elem *p, int size);									//print out the 1-D array passed as argument
		Elem* slowSortCol (Elem *x, int size);								//slow sort for column (j)
		Elem* fastSortCol (Elem *x, int start, int end);					//fast sort for column (j)
		int partitionCol (Elem *x, int start, int end);						//partition array of column in aid to fast sort
		void computeGraph();												//compute the array G*
		Elem* multiply (Elem *x, int s1, Elem *y, int s2, int &s4);			//multiply 2 matrices (1-D arrays)
		Elem* slowSortRow (Elem *x, int size);								//slow sort for row (i)
		Elem* fastSortRow (Elem *x, int start, int end);					//fast sort for row (i)
		int partitionRow (Elem *x, int start, int end);						//partition array of row in aid to fast sort
		Elem* sum (Elem *x, int s1, Elem *y, int s2, int &s3);				//sum 2 matrices (1-D arrays)
		Elem* deleteElem (Elem *x, int &s1, int index);						//delete element in 1_D passed in as argument
		void computeInDeg(int &in);											//compute the top incoming connection web page 	
		void computeOutDeg(int &out);										//compute the top outcoming connection web page
		int findMax(Elem *x, int size);										//return the index of element with max value in array x type Elem
}; 

//contructor to class Graph
//param: sType - sort type (slow/fast)
Graph::Graph(string sType)
{
	edge = new Elem [100];
	gr = new Elem[100];
	eCap=0;
	gCap=0;
	sortType = sType;
}

//display the array edge
void Graph::displayEdge()
{
	for (int i =0; i<eCap;i++)
		cout << edge[i].row << '\t' << edge[i].col << '\t' << edge[i].value << endl;
}

//display the array passed in as argument
//param: Elem array p and its size
void Graph::display(Elem *p, int size)
{
	for (int i =0; i<size;i++)
		cout << p[i].row << '\t' << p[i].col << '\t' << p[i].value << endl;
}

//take the .html filename from the .txt file and stream in contents
//param: file - name of .html file, string array p containing all of .html filenames, its size, index of string file in this array 
void Graph::stream(string file, string p[], int size, int index)
{	
	string q[100];			//string array size 100
	int size1=0;			//actual size of array q, initialized with 0
	string line;			//each line of .html file
	string token;			//parse line in the file to get desired token
	string temp;			//be assigned with a string value for compare 
	file += ".html";		//add .html tail in order to open file
	
	//stream .html file, return when fail
	ifstream fin (file.c_str());
	if (fin.fail()) return;
	
	//stream in each line
	while(getline(fin,line))
	{
		//not an anchor tag with link to another page? definitely comment or text => ignore
		if (line.find("<a href")==string::npos && line.find("</a>")==string::npos) 
			continue;
		
		//parse the line to get the page name from link, put in array q, actual size increments
		istringstream ss(line);
		getline(ss,token,'\"');
		getline(ss,token,'.');
		q[size1] = token;
		size1++;
	}
	 
	//traverse 2 arrays, q outer and p inner to compare if the page in .html file is already introduced in .txt file 
	//if yes, add i, j (row,col) and value (default 1) to array edge, actual capacity increments
	for (int k = 0; k<size1; k++)
	{
		temp = q[k];
		for (int l = 0; l < size; l++)
		{
			if (p[l] == temp)
			{
				edge[eCap].col = l+1;
				edge[eCap].row = index + 1;
				eCap++;
			}
		}
	}  
	
	//close .html file	
	fin.close();
}

//multiply 2 matrices (1-D arrays) passed in and return as reference the result matrix
//param: matrices x, y, their sizes and reference size for result matrix
//return: the result matrix
Elem* Graph::multiply (Elem *x, int s1, Elem *y, int s2, int &s4)
{
	//1 temporary matrix and another 1 for return, they are initialized with 100
	//s3 and s4 are their actual capacities
	Elem *c = new Elem[100];
	int s3 = 0;
	Elem *d = new Elem[100];
	s4 = 0;
	
	//sort following column (j) with slow or fast sort depending on sortType
	if (sortType=="slow") y = slowSortCol(y,s2);	
	else if (sortType=="fast") y = fastSortCol(y,0,s2-1);
	
	//2 for loop, outer traverses matrix x and inner traverse matrix y.
	//if col of x = row of y, take the value of x time value of y and store in c, increment actual size c
	for (int indexX = 0; indexX<s1; indexX++)
	{
		for (int indexY = 0; indexY<s2; indexY++)
		{
			if (x[indexX].col == y[indexY].row)
			{
				c[s3].row = x[indexX].row;
				c[s3].col = y[indexY].col;
				c[s3].value = x[indexX].value * y[indexY].value;
				s3++;
			}
		}
	}
	
	int done;		//check if we already list element of c to result array d or not
	
	//traverse matrix c, set done = 0, if it is the first element of c, add it to array d immediately, incrementing actual size of d
	for (int indexC = 0; indexC < s3; indexC++)
	{
		done = 0;
		if (indexC==0)
		{
			d[s4] = c[indexC];
			s4++;
		}
		//else if it is not the 1st element of c, traverse array d to figure out if a element in d with same pair (i,j) to the one in c is already there
		//yes: no need add new element to d, take this d element.value + c element.value, done = 1
		//no (done = 0): add this c element to d, increment d's actual size
		else
		{
			for (int indexD=0; indexD<s4; indexD++)
			{
				if (d[indexD].row == c[indexC].row && d[indexD].col == c[indexC].col)
				{
					d[indexD].value += c[indexC].value;
					done = 1;
					break;
				}
			}
			
			if (done==1) continue;
			
			d[s4] = c[indexC];
			s4++;
		}
	}
	
	//delete elements in diagonal of matrix
	for (int indexD = 0; indexD < s4; indexD++)
		if (d[indexD].row == d[indexD].col)
			d = deleteElem(d,s4,indexD);
	
	//return Elem array d
	return d;
}

//compute the G* matrix
void Graph::computeGraph()
{
	//Elem array e1 for edge*edge result, actual size = 0
	Elem *e1 = new Elem[100];
	int rSize1 = 0;
	
	//do multiplication edge*edge, get size of result matrix e1
	e1 = multiply(edge,eCap,edge,eCap,rSize1);
	
	//sort row (i) of e1
	if(sortType=="slow") slowSortRow(e1,rSize1);
	else if (sortType=="fast") fastSortRow(e1,0,rSize1-1);
	
	
	/*cout << "edge array (edge*edge): " << endl;
	display(e1,rSize1); 
	cout << endl;*/
	
	//Elem array e2 for edge*edge*edge result, actual size = 0
	Elem *e2 = new Elem[100];
	int rSize2 = 0;
	
	//do multiplication edge*edge*edge, get size of result matrix e2
	e2 = multiply(e1,rSize1,edge,eCap,rSize2);
	
	//sort row (i) of e2
	if(sortType=="slow") slowSortRow(e2,rSize2);
	else if (sortType=="fast") fastSortRow(e2,0,rSize2-1);
	
	
	/*cout << "edge array (edge*edge*edge): " << endl;
	display(e2,rSize2); 
	cout << endl;*/
	
	//sort row (i) of edge
	if(sortType=="slow") slowSortRow(edge,eCap);
	else if (sortType=="fast") fastSortRow(edge,0,eCap-1);
	
	//Elem array e3 for edge+edge*edge result, actual size = 0
	Elem *e3 = new Elem[100];
	int rSize3 = 0;
	
	//do sum edge+edge*edge, get size of result matrix e3
	e3 = sum(edge,eCap,e1,rSize1,rSize3);
	
	
	/*cout << "sum of edge and edge^2: " << endl;
	display(e3,rSize3); 
	cout << endl;*/
	
	//do sum G* = edge+edge*edge+edge*edge*edge, get size of result matrix gr
	gr = sum(e3,rSize3,e2,rSize2,gCap);
	
	
	/*cout << "sum of edge, edge^2 and edge^3: " << endl;
	display(gr,gCap); 
	cout << endl;*/
}

//slow sort following the column (j): selection sort
//param: Elem array x and its size
//return: the sorted array x
Elem* Graph::slowSortCol (Elem *x, int size)
{
	Elem temp;
	for (int k=0; k<(size-1); k++)
	{
		for (int l = (k+1); l< size; l++)
		{
			if (x[k].col > x[l].col)
			{
				temp = x[k];
				x[k] = x[l];
				x[l] = temp;
			}
		}
	}
	return x;
}

//fast sort following the column (j): quick sort
//param: Elem array x, start index and end index
//return sorted array x
Elem* Graph::fastSortCol (Elem *x, int start, int end)
{
	int pIndex;
	if (start<end)
	{
		pIndex = partitionCol(x,start,end);
		x = fastSortCol(x,start,pIndex-1);
		x = fastSortCol(x,pIndex+1,end);
	}
	return x;
}

//partition the array passed in into 2 parts bordered by a pivot j, left of pivot are numbers less than pivot and right of pivot are numbers larger than pivot
//param: Elem array x, start and end index
//return: the pivot index
int Graph::partitionCol (Elem *x, int start, int end)
{
	int pivot  = x[end].col;
	int pIndex = start;
	Elem temp;
	for (int k=start; k<end; k++)
	{
		if (x[k].col <= pivot)
		{
			temp = x[k];
			x[k] = x[pIndex];
			x[pIndex] = temp;
			pIndex++;
		}
	}
	
	temp = x[pIndex];
	x[pIndex] = x[end];
	x[end] = temp;
	
	return pIndex;
}

//slow sort following the row (i): selection sort
//param: Elem array x and its size
//return: the sorted array x
Elem* Graph::slowSortRow (Elem *x, int size)
{
	Elem temp;
	for (int k=0; k<(size-1); k++)
	{
		for (int l = (k+1); l< size; l++)
		{
			if (x[k].row > x[l].row)
			{
				temp = x[k];
				x[k] = x[l];
				x[l] = temp;
			}
		}
	}
	return x;
}

//fast sort following the row (i): quick sort
//param: Elem array x, start index and end index
//return sorted array x
Elem* Graph::fastSortRow (Elem *x, int start, int end)
{
	int pIndex;
	if (start<end)
	{
		pIndex = partitionRow(x,start,end);
		x = fastSortRow(x,start,pIndex-1);
		x = fastSortRow(x,pIndex+1,end);
	}
	return x;
}

//partition the array passed in into 2 parts bordered by a pivot i, left of pivot are numbers less than pivot and right of pivot are numbers larger than pivot
//param: Elem array x, start and end index
//return: the pivot index
int Graph::partitionRow (Elem *x, int start, int end)
{
	int pivot  = x[end].row;
	int pIndex = start;
	Elem temp;
	for (int k=start; k<end; k++)
	{
		if (x[k].row <= pivot)
		{
			temp = x[k];
			x[k] = x[pIndex];
			x[pIndex] = temp;
			pIndex++;
		}
	}
	
	temp = x[pIndex];
	x[pIndex] = x[end];
	x[end] = temp;
	
	return pIndex;
}

//take sum of 2 matrices (1-D arrays)
//param: 2 arrays x and y, and their sizes, reference size of result matrix
//return: result matrix
Elem* Graph::sum (Elem *x, int s1, Elem *y, int s2, int &s3)
{
	Elem *temp = new Elem[100];		//temporary matrix size 100 actual size s3 = 0
	int done1;						//check if we already take sum of value of elements of x and y and add to result matrix or not
	
	//outer: traverse array x, inner: traverse array y
	//done1 = 0 <=> not done. check if element of x.i/j == element of y.i/j: yes: sum 2 elements.value, add i, j, value to result matrix,  
	//delete the current element in y. <note that i is row and j is col.> done1==0: add element of x to result matrix
	for (int indexX=0; indexX<s1; indexX++)
	{
		done1 = 0;
		for (int indexY=0; indexY<s2; indexY++)
		{
			if (x[indexX].row == y[indexY].row && x[indexX].col == y[indexY].col)
			{
				temp[s3].value = x[indexX].value + y[indexY].value;
				temp[s3].row = x[indexX].row;
				temp[s3].col = x[indexX].col;
				s3++;
				done1 = 1;
				y = deleteElem(y,s2,indexY);
				break;
			}
		}
		
		if (done1==1) continue;
		
		temp[s3] = x[indexX];
		s3++;
	}
	
	//the leftover in array y is added to result matrix
	for (int indexY=0; indexY<s2; indexY++)
	{
		temp[s3] = y[indexY];
		s3++;
	}
	
	//sort result matrix following row (i)
	if(sortType=="slow") slowSortRow(temp,s3);
	else if (sortType=="fast") fastSortRow(temp,0,s3-1);
	
	//return result matrix
	return temp;
}

//delete an element in array passed in at index
//param: Elem array x, reference its size, index
//return: new array after deletion
Elem* Graph::deleteElem (Elem *x, int &s1, int index)
{
	for (int i=index; i<s1-1; i++)
		x[i] = x[i+1];
	s1--;
	return x;	
}

//compute the most incoming web page 
//param: reference the index of this page in array of .html filenames
void Graph::computeInDeg(int &in)
{
	Elem *c = new Elem[100];		//store only col numbers and values after sum all element values on corresponding column
	int size = 0;					//actual size = 0
	int done;						//check if we already list element of gr to result array c or not
	
	//sort gr array following column (j)
	if (sortType=="slow") gr = slowSortCol(gr,gCap);	
	else if (sortType=="fast") gr = fastSortCol(gr,0,gCap-1);
	
	//loop through gr and c
	//1st element of gr is immediately inserted to c
	//other elements of gr must be compared to check if it has a same col to any element in c
	for (int indexG = 0; indexG < gCap; indexG++)
	{
		done  = 0;
		if (indexG == 0)
		{
			c[size] = gr[indexG];
			c[size].row = 0;
			size++;
		}
		
		else
		{
			for (int indexC=0; indexC<size; indexC++)
			{
				//yes: sum the value, done = 1
				if(c[indexC].col==gr[indexG].col)
				{
					c[indexC].value += gr[indexG].value;
					done = 1;
					break;
				}
			}
			
			if (done==1) continue;
			
			//no: insert this element to c
			c[size] = gr[indexG];
			c[size].row = 0;
			size++;
		}	
	}
	
	//find element in c that has max value pull out the col and minus 1 to get index 
	int num = findMax(c,size);
	in = c[num].col-1;
}

//compute the most outcoming web page 
//param: reference the index of this page in array of .html filenames
void Graph::computeOutDeg(int &out)
{
	Elem *c = new Elem[100];		//store only row numbers and values after sum all element values on corresponding row	
	int size = 0;					//actual size = 0
	int done;						//check if we already list element of gr to result array c or not
	
	//sort gr array following row (i)
	if (sortType=="slow") gr = slowSortRow(gr,gCap);	
	else if (sortType=="fast") gr = fastSortRow(gr,0,gCap-1);
	
	//loop through gr and c
	//1st element of gr is immediately inserted to c
	//other elements of gr must be compared to check if it has a same row to any element in c
	for (int indexG = 0; indexG < gCap; indexG++)
	{
		done = 0;
		if (indexG == 0)
		{
			c[size].row = gr[indexG].row;
			c[size].col = 0;
			c[size].value = gr[indexG].value;
			size++;
		}
		
		else
		{
			for (int indexC=0; indexC<size; indexC++)
			{
				//yes: sum the value, done = 1
				if(c[indexC].row==gr[indexG].row)
				{
					c[indexC].value += gr[indexG].value;
					done = 1;
					break;
				}
			}
			
			if (done==1) continue;
			
			//no: insert this element to c
			c[size].row = gr[indexG].row;
			c[size].col = 0;
			c[size].value = gr[indexG].value;
			size++;
		}	
	}
	
	//find element in c that has max value, pull out the row and minus 1 to get index 
	int num = findMax(c,size);
	out = c[num].row-1;
}

//find the value in array that has the max value
//param: Elem array and its size
//return: index of element with max value
int Graph::findMax(Elem *x, int size)
{
	int mx = x[0].value;
	int indexMax = 0;
	for (int index = 1; index < size; index++)
	{
		if (x[index].value>mx)
		{
			indexMax = index;
			mx = x[index].value;
		}
	}
	return indexMax;
}


//=============================================
//Main function that takes in array of command lines and its size
//=============================================
int main (int argc, char *argv[])
{
	//exit if no command line
	if (argc==1) return -1;
	
	string argument = argv[1];				//convert command line to string
	string token;							//track the parsing command line to get filename
	
	//parse command line to get .txt filename and sort type	
	istringstream ss (argument);			
	getline(ss,token,'=');
	getline(ss,token,';');
	string filename = token;
	getline(ss,token,'=');
	getline(ss,token,'\0');
	string sortType = token;
	
	//open the .txt file, exit when fail
	ifstream fin (filename.c_str());
	if (fin.fail()) return -1;
	
	string line;				//line of text file
	string page[100];			//array of .html link name
	int size = 0;				//actual size of above array
	int in;						//index of top incoming
	int out;					//index of outgoing
	
	//stream the content on text file and store to array
	while (getline(fin,line))
	{
		if (line=="") continue;
		istringstream ss1 (line);
		getline(ss1,token,'.');
		page[size] = token;
		size++;
	}
	
	//close text file
	fin.close();
	
	//call object graph, pass in sort type
	Graph g(sortType);
	
	//stream in all html file and their contents
	for (int i = 0; i<size; i++)
	{
		g.stream(page[i],page,size,i);
	}
	
	/*cout << "Adjacent Edge Array: " << endl;
	g.displayEdge();
	cout << endl;*/
	
	//compute G*
	g.computeGraph();
	
	//compute top incoming and outgoing web page
	g.computeOutDeg(out);
	g.computeInDeg(in);
	
	//output them to screen
	cout << "top outgoing connections web page=" << page[out] << ".html" << endl;
	cout << "top incoming connections web page=" << page[in] << ".html" <<endl;
	
	//ask user to hit any key for exit
	return 0;
}
