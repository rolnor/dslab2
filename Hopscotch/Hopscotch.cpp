#include <iostream>
#include <vector>
#include <string>



using namespace std;

// helper functions
void getLine(string& inputString);
void menu();

// global variables
const int bucketSize = 8;
const int bucketMaxDist = 4;

class myBucket
{
private:
    string bucket[bucketSize];
    unsigned char hopBits[bucketSize];
public:
    myBucket();
    void insert(string& inputData, int hashLocation);
    void bitOp(string oP, int bucketPosition, int dataPosition);
//    void search(string& inputData, int hashLocation);
//    void remove(string& inputData, int hashLocation);
//    void repartition(int trueHash, int newHashLocation);
    int calculateHash(const string& key, int bucketSize);
    void print();
};

int main()
{
    char choise = ' ';
    myBucket hopBucket;
    string inputString = "";
    while (choise != '0')
    {
        menu();
        cin >> choise;
        switch (choise)
        {
        case '1':
            cout << endl << "Insert: ";
            getLine(inputString);
            hopBucket.insert(inputString, hopBucket.calculateHash(inputString, bucketSize));
            break;
        case '2':
 //           cout << endl << "Search: ";
 //           getLine(inputString);
  //          linearTable.search(inputString, linearTable.calculateHash(inputString, tableSize));
              break;
        case '3':
      //      cout << endl << "Remove: ";
        //    getLine(inputString);
          //  linearTable.remove(inputString, linearTable.calculateHash(inputString, tableSize));
              break;
        case '4':
              hopBucket.print();
            break;
        case '0':
            break;
        }
    }
    return 0;
}

void menu()
{
    cout << "1. Insert" << endl;
    cout << "2. Search" << endl;
    cout << "3. Remove" << endl;
    cout << "4. Print" << endl;
    cout << "0. Quit " << endl;
}

void getLine(string& inputString)
{
    if (isspace(cin.peek()))
        cin.ignore();

    getline(cin, inputString);
}

myBucket::myBucket()
{
    for (int i = 0; i < bucketSize; i++)
    {
        hopBits[i] = 0;
    }
}

void myBucket::insert(string& inputData, int hashLocation)
{
    bool algCompleted = false;
    int i = 0;
    int traverserHashLocation = hashLocation;

    while (!algCompleted)
    {
        if (bucket[traverserHashLocation] == "")
        {
            bucket[traverserHashLocation] = inputData;
            // update hop bit 
            if(traverserHashLocation != hashLocation)
                this->bitOp("OR", traverserHashLocation, -1);

            this->bitOp("OR", hashLocation, i);
            algCompleted = true;
        }
        // stop if not unique
        else if (bucket[traverserHashLocation] == inputData)
            algCompleted = true;
        // else continue circular searching 
        else if (traverserHashLocation < bucketSize - 1)
            traverserHashLocation += 1;
        else
            traverserHashLocation = 0;

        // only allow one pass
        if (i == bucketSize - 1)
        {
            cout << endl << "Bucket is full" << endl;
            break;
        }
        else i++;
    }
}

void myBucket::bitOp(string oP, int bucketPosition, int dataPosition)
{
    // 1000 = 0 pos = 1
    // 0100 = 1 pos = 2 
    // 0010 = 2 pos = 4
    // 0001 = 4 pos = 9

    if (dataPosition == 0)
        dataPosition = 1;
    else if (dataPosition == 1)
        dataPosition = 2;
    else if (dataPosition == 2)
        dataPosition = 4;
    else if (dataPosition == 2)
        dataPosition = 8;
    else
        dataPosition = 0;

    if (oP == "OR")
        hopBits[bucketPosition] = hopBits[bucketPosition] | dataPosition;
    else if (oP == "AND")
        hopBits[bucketPosition] = hopBits[bucketPosition] & dataPosition;
}

//void myBucket::search(string& inputData, int hashLocation)
//{
//    bool algCompleted = false;
//    int i = 0;
//
//    while (!algCompleted)
//    {
//        if (data[hashLocation] == "")
//        {
//            cout << endl << "Key not found" << endl;
//            algCompleted = true;
//        }
//        else if (data[hashLocation] == inputData)
//        {
//            cout << endl << "Key found" << endl;
//            algCompleted = true;
//        }
//        else if (hashLocation < tableSize - 1)
//            hashLocation += 1;
//        else
//            hashLocation = 0;
//
//        // only allow one pass
//        if (i == tableSize - 1)
//        {
//            cout << endl << "Key not found" << endl;
//            break;
//        }
//        else i++;
//    }
//}
//
//void myBucket::remove(string& inputData, int hashLocation)
//{
//    bool algCompleted = false;
//    int traverser = hashLocation;
//
//    int i = 0;
//
//    while (!algCompleted && i < tableSize - 1)
//    {
//        if (data[traverser] == "")
//        {
//            algCompleted = true;
//        }
//        else if (data[traverser] == inputData)
//        {
//            data[traverser] = "";
//            repartition(hashLocation, traverser);
//        }
//        else if (traverser < tableSize - 1)
//            traverser += 1;
//        else
//            traverser = 0;
//    }
//}
//
//void myBucket::repartition(int trueHash, int newHashLocation)
//{
//    bool algCompleted = false;
//    int trailingHashLocation;
//    int i = 0;
//
//    while (!algCompleted)
//    {
//        if (newHashLocation + 1 < tableSize - 1)
//            newHashLocation++;
//        else newHashLocation = 0;
//
//        if (newHashLocation == 0)
//            trailingHashLocation = tableSize - 1;
//        else trailingHashLocation = newHashLocation - 1;
//
//        // move data up one slot
//        if (trueHash == calculateHash(data[newHashLocation], tableSize))
//        {
//            data[trailingHashLocation] = data[newHashLocation];
//            data[newHashLocation] = "";
//        }
//        // stop if empty slot or correct hash
//        else if (newHashLocation == calculateHash(data[newHashLocation], tableSize) || data[newHashLocation] == "")
//            algCompleted = true;
//    }
//}
//
int myBucket::calculateHash(const string& key, int tableSize)
{
    // unsigned to avoid negative number
    unsigned int hashValue = 0;

    for (char myChar : key)
    {
        hashValue = 4;//37 * hashValue + myChar;
    }
    return hashValue % tableSize;
}

void myBucket::print()
{
    int i = 0;
    cout << endl << "Table content" << endl;
    for (string key : bucket)
    {
        cout << i << " : " << key << "    " << to_string(hopBits[i]) << endl;
        i++;
    }
}
