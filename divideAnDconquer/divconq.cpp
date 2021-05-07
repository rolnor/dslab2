#include <iostream>
#include <vector>
#include <string>

using namespace std;


int calculateMedian(vector<int> mySubList);
int partition(vector<int>& elements, int left, int right, int pivotPoint);
void divideArray(vector<int>& elements, int pivotPosition, bool keepLeftOfPivot);
void insertionSort(vector<int>& elements);
int lomutoPartition(vector<int>& arr, int low, int high);
int kthSmallest(vector<int>& arr, int n, int k);

int main()
{
    vector<int> elements = { 2, 4, 12, 10, 17, 22, 8, 13, 7, 19,22,1 ,42,43,32,23,25,67};

    int pivotPos = 0;
    int wantedLargestNumber = 4;

    int numberOfArrays = ceil(elements.size() / 5.0);
    while (numberOfArrays > 1)
    {
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
        int pivotPoint = 0;
        for (int i = 0; i < numberOfArrays; i++)
        {
            pivotPoint += medianArray[i];
        }
        pivotPoint = pivotPoint / numberOfArrays;


        //partition the list with calculated pivot point
        pivotPoint = partition(elements, elements.back(), elements.front(), pivotPoint);

        // decide which part to keep after divide
        if (wantedLargestNumber < pivotPoint)
            divideArray(elements, pivotPoint, true);
        else
        {
            // if less than 5 remove the pivot and break
            swap(elements[pivotPoint], elements[elements.size()-1]);
            elements.pop_back();
            break;
        }
            

        delete[] medianArray;
        numberOfArrays = ceil(elements.size() / 5.0);
    }

    // [TODO] 
    // 1. sort the final array
    // need linear sorting here. insertion wrong?
    
   // kthSmallest(elements,0,5);
    insertionSort(elements);
    cout << "The " << to_string(wantedLargestNumber) << ":th smallest number is " << elements[3] << endl << endl;
    // 2. add recurive function calls
    // 3. done!!!
    
    // free up allocated memory

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
            if (elements[j] >= pivotPoint)
                j--;
        }
        else if (elements[i] > elements[j])
        {
            swap(elements[i],elements[j]);
            i++;
            j--;
        }
        else
            j--;

    }
    // swap the pivotPoint as a seperator
    swap(elements[j], elements[elements.size()-1]);

    return j;
}

int lomutoPartition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int index = low;
    for (int j = low; j <= high; j++) {
        if (arr[j] < pivot) {
            index++;
            swap(arr[index], arr[j]);
        }
    }
    swap(arr[index + 1], arr[high]);
    return index + 1;
}

int kthSmallest(vector<int>& arr, int n, int k) {

    int low = 0, high = n - 1;
    while (low <= high) {
        int pivot = lomutoPartition(arr, low, high);
        if (pivot == k - 1)
            return pivot;
        else if (pivot > k - 1)
            high = pivot - 1;
        else
            low = pivot + 1;
    }
    return -1;
}


void insertionSort(vector<int>& elements)
{
    int pivotPoint, j;
    for (int i = 0; i < elements.size(); i++) 
    {
        pivotPoint = elements[i];
        j = i;
        while (j > 0 && elements[j - 1] > pivotPoint) {
            elements[j] = elements[j - 1];
            j--;
        }
        elements[j] = pivotPoint;
    }
}