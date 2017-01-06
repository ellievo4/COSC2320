#include<iostream>

using namespace std;

int* quicksort(int *a, int start, int end);
int partition (int *a, int start, int end);
int* deleteElem(int *a, int &size, int index);

int main()
{
	int *a = new int [8];
	int size = 8;
	//a = {3,2,1,6,8,5,7,4};
	a[0]=3; a[4]=8; 
	a[1]=2; a[5]=5;
	a[2]=1; a[6]=7;
	a[3]=6; a[7]=4;
	a = quicksort(a,0,7);
	for (int i = 0; i<size; i++)
		cout << a[i] << " ";
	a = deleteElem(a,size,5);
	
	cout << endl;
	
	for (int i = 0; i<size; i++)
		cout << a[i] << " ";
	return 0;
}

int* quicksort(int *a, int start, int end)
{
	if (start<end)
	{
		int pIndex = partition(a,start,end);
		a = quicksort(a,start,pIndex-1);
		a = quicksort(a,pIndex+1,end);
	}
	return a;
}

int partition (int *a, int start, int end)
{
	int pivot = a[end];
	int pIndex = start;
	int temp;
	for (int i=start; i<end; i++)
	{
		if (a[i]<pivot)
		{
			temp = a[i];
			a[i] = a[pIndex];
			a[pIndex] = temp;
			pIndex++;
		}
	}
	temp = a[pIndex];
	a[pIndex] = a[end];
	a[end] = temp;
	return pIndex;
}

int* deleteElem(int *a, int &size, int index)
{
	for (int i = index; i < size-1; i++)
	{
		a[i] = a[i+1];
	}
	size--;
	return a;
}
