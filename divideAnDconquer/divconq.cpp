#include <iostream>
#include <vector>
#include <string>

using namespace std;


int calculateMedian(vector<int> mySubList);
int partition(vector<int>& elements, int left, int right, int pivotPoint);
void divideArray(vector<int>& elements, int pivotPosition, bool keepLeftOfPivot);
void insertionSort(vector<int>& elements);

void quickSelect(vector<int>& elements, int left, int right, int searchedElement);



int main()
{
    vector<int> elements = { 2, 4, 12, 6, 17, 22, 5, 13, 7, 19 };

    int pivotPos = 0;
    int wantedLargestNumber = 4;

    int numberOfArrays = elements.size() / 5;
    // create room for segmented list. rows = elements/5 & columns = 5
    vector<vector<int>> myLists(numberOfArrays, vector<int>(5));


    // create sublists with 5 elements each
    int i_column = 0, j_row = 0, k_index = 0;
    while (k_index < elements.size())
    {
        myLists[i_column][j_row] = elements[k_index];
        j_row++;
        k_index++;

        if (j_row > 4)
        {
            j_row = 0;
            i_column++;
        }
    }
  
    // calculate median for each sublist
    int* medianArray = new int[numberOfArrays];
    for (int i = 0; i < numberOfArrays; i++)
    {       
        medianArray[i] = calculateMedian(myLists[i]);
    }

    // calculate median of medianArray
    int pivotPoint =0;
    for (int i = 0; i < numberOfArrays;i++)
    {
        pivotPoint += medianArray[i];
    }
    pivotPoint = pivotPoint / numberOfArrays;


    //partition the list with calculated pivot point
    pivotPoint = partition(elements, elements.back(), elements.front(), pivotPoint);

    // decide which part to keep after divide
    if(wantedLargestNumber < pivotPoint)
        divideArray(elements, pivotPoint, true);

    // [TODO] 
    // 1. sort the final array
    // need linear sorting here. insertion wrong?
    insertionSort(elements);
    cout << "The " << to_string(wantedLargestNumber) << ":th largest number is " << elements[3] << endl << endl;
    // 2. add recurive function calls
    // 3. done!!!
    
    // free up allocated memory
    delete[] medianArray;
    return 0;
}
void divideArray(vector<int>& elements, int pivotPosition, bool keepLeftOfPivot)
{
    vector<int> temp;
    if (keepLeftOfPivot)
    {
        for (int i = 0; i < pivotPosition; i++)
        {
            temp.push_back(elements[i]);
        }
    }
    else
    {
        pivotPosition++;
        for(pivotPosition; pivotPosition < elements.size(); pivotPosition++)
            temp.push_back(elements[pivotPosition]);
    }
    elements = temp;
}



int calculateMedian(vector<int> mySubList)
{
    int sum = 0;
    for (int number : mySubList)
    {
        sum += number;
    }
    return sum / 5;
}


int partition(vector<int>& elements, int left, int right, int pivotPoint)
{
    elements.push_back(pivotPoint);
    int i = 0, j = elements.size()-2;

    //   0  1   2  3   4   5  6  7   8   9    pivot = 10
    // { 2, 4, 12, 6, 17, 22, 5, 13, 7, 19 };
    for (i; i < j; )
    {
        if (elements[i] <= pivotPoint)
        {
            i++;
            if (elements[j] > pivotPoint)
                j--;
        }
        else if (elements[i] > elements[j])
            swap(elements[i],elements[j]);

    }
    // swap the pivotPoint as a seperator
    swap(elements[j], elements[elements.size()-1]);

    return j;
}


void insertionSort(vector<int>& elements)
{
    int key, j;
    for (int i = 0; i < elements.size(); i++) 
    {
        key = elements[i];
        j = i;
        while (j > 0 && elements[j - 1] > key) {
            elements[j] = elements[j - 1];
            j--;
        }
        elements[j] = key;   
    }
}

// median-of-median-of-five partitioning




  /*                                   [2]=12   [9]=9
int median(vector<int>& elements, int left, int right)
{
     [5] = 22 
    int center = (left + right) / 2;

                  22         12
    if (elements[center] < elements[left])
    {
        swap(elements[center], elements[left]);
    }
                9                12
    if (elements[right] < elements[left])
    {
        swap(elements[right], elements[left]);
    }
                9                  22,
    {
        swap(elements[right], elements[center]);
    }

     place pivot 
    swap(elements[right-1], elements[center]);

    return elements[right - 1];
}*/

// 
