#include <iostream>
#include <vector>
#include <string>

using namespace std;


int calculateMedian(vector<int> mySubList);
void partition(vector<int>& elements, int left, int right, int pivotPoint);
void quickSelect(vector<int>& elements, int left, int right, int searchedElement);



int main()
{
    vector<int> elements = { 2, 4, 12, 6, 17, 22, 5, 13, 7, 19 };

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

  //  median(elements, elements.front(), elements.back());
    //partition the list with calculated pivot point
    partition(elements, elements.back(), elements.front(), pivotPoint);


    delete[] medianArray;
    return 0;
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


void partition(vector<int>& elements, int left, int right, int pivotPoint)
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
    swap(elements[j], elements[elements.size()-1]);

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
