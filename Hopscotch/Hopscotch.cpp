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
    void search(string& inputData, int hashLocation);
    void remove(string& inputData, int hashLocation);
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
            cout << endl << "Search: ";
            getLine(inputString);
            hopBucket.search(inputString, hopBucket.calculateHash(inputString, bucketSize));
              break;
        case '3':
            cout << endl << "Remove: ";
            getLine(inputString);
            hopBucket.remove(inputString, hopBucket.calculateHash(inputString, bucketSize));
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
    int traverserHashLocation = hashLocation;
    int bucketPosition = 0;

    while (!algCompleted)
    {
        if (bucket[traverserHashLocation] == "")
        {
            bucket[traverserHashLocation] = inputData;
            // update hop bit 
            if(traverserHashLocation != hashLocation)
                this->bitOp("OR", traverserHashLocation, -1);

            this->bitOp("OR", hashLocation, bucketPosition);
            algCompleted = true;
        }
        // stop if not unique
        else if (bucket[traverserHashLocation] == inputData)
        {
            algCompleted = true;
            cout << endl << "Op failed: Key already in bucket." << endl;
        }
        // else continue circular searching 
        else if (traverserHashLocation < bucketSize - 1)
        {
            traverserHashLocation += 1;
            bucketPosition++;
        }
        else
        {
            cout << endl << "Bucket is full" << endl;
            algCompleted = true;
        }
    }
}

void myBucket::bitOp(string oP, int bucketPosition, int dataPosition)
{
    // 1000 = 0 pos = 1
    // 0100 = 1 pos = 2 
    // 0010 = 2 pos = 4
    // 0001 = 4 pos = 8

    if (dataPosition == 0)
        dataPosition = 1;
    else if (dataPosition == 1)
        dataPosition = 2;
    else if (dataPosition == 2)
        dataPosition = 4;
    else if (dataPosition == 3)
        dataPosition = 8;
    else
        dataPosition = 0;

    if (oP == "OR")
        hopBits[bucketPosition] = hopBits[bucketPosition] | dataPosition;
    else if (oP == "AND")
        hopBits[bucketPosition] = hopBits[bucketPosition] & dataPosition;
    // SUB eq (negative bucket pos) * pos | ex pos 2 = 0100. neg pos(~) = 1011 => resets 2nd bit with AND op.
    else if (oP == "SUB")
        hopBits[bucketPosition] = hopBits[bucketPosition] & ~dataPosition;
}

void myBucket::search(string& inputData, int hashLocation)
{
    unsigned int BitIndex = hopBits[hashLocation];
    unsigned int shiftBits = 1;
    bool keyFound = false;

    if (BitIndex == 0)
    {
        cout << endl << "Key not found" << endl;
    }
    else
    {
        for (int i = 0; i < bucketMaxDist; i++)
        {
            if (inputData == bucket[hashLocation + i])
            {
                cout << endl << "Key found" << endl;
                keyFound = true;
            }

        }
        if (!keyFound)
            cout << endl << "Key not in bucket" << endl;
    }
}

void myBucket::remove(string& inputData, int hashLocation)
{
    unsigned int BitIndex = hopBits[hashLocation];
    unsigned int shiftBits = 1;
    bool keyFound = false;

    if (BitIndex == 0)
    {
        cout << endl << "Key not found" << endl;
    }
    else
    {
        for (int i = 0; i < bucketMaxDist; i++)
        {
            if (inputData == bucket[hashLocation + i])
            {
                cout << endl << "Key found and removed" << endl;
                keyFound = true;

                bucket[hashLocation + i] = "";
                bitOp("SUB", hashLocation, i);
            }
        }
        if (!keyFound)
            cout << endl << "Key not in bucket" << endl;
    }
}

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
