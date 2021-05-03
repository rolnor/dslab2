#include <iostream>
#include <vector>
#include <string>



using namespace std;

// helper functions
void getLine(string& inputString);
void menu();

// global variables
const int bucketSize = 8;
// hardcoded.... don't change max dist! WILL destroy bit calculation.
const int bucketMaxDist = 4;

class myBucket
{
private:
    string bucket[bucketSize];
    unsigned char hopBits[bucketSize];
public:
    myBucket();
    // inserts data into bucket
    void insert(string& inputData, int hashLocation);
    // update bit operations. supports ADD, OR and SUB
    void bitOp(string oP, int bucketPosition, int dataPosition);
    // calculates relative index postition list from bits
    vector<int> calculateBitLocation(int bitString);
    // search for input key in the specified hashlocation
    void search(string& inputData, int hashLocation);
    // remove key and update bit information
    void remove(string& inputData, int hashLocation);
    // try to move items to adjacent buckets and updates bitstring. returns true if op successful.
    bool repartition(int trueHash);
    // calculates hash index of specified key
    int calculateHash(const string& key, int bucketSize);
    void insertData();
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
        case '5':
            hopBucket.insertData();
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
    cout << "5. Insert data" << endl;
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
        else if (traverserHashLocation < bucketMaxDist)
        {
            traverserHashLocation += 1;
            bucketPosition++;
        }
        else
        {
            cout << endl << "Bucket is full. Trying to restructure" << endl;

            if (repartition(hashLocation))
            {
                cout << endl << "Restructure successful. Adding key." << endl;
                traverserHashLocation = hashLocation;
                bucketPosition = 0;
            }
            else
            {
                cout << endl << "Restructure failed. Key not added." << endl;
                algCompleted = true;
            }
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

vector<int> myBucket::calculateBitLocation(int bitString)
{
    vector<int> indexes;
    if (~bitString & 1)
        indexes.push_back(0);
    if (~bitString & 2)
        indexes.push_back(1);
    if (~bitString & 4)
        indexes.push_back(2);
    if (~bitString & 8)
        indexes.push_back(3);

    return indexes;
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

                // empty bucket
                bucket[hashLocation + i] = "";
                // set represented buckets bit to 0 in main bucket.
                bitOp("SUB", hashLocation, i);
            }
        }
        if (!keyFound)
            cout << endl << "Key not in bucket" << endl;
    }
}

// inputs are trueHash: location that has bitinfo. & currentLocation is bucket pos
bool myBucket::repartition(int trueHash)
{ 
    // find first buckets bit sequence. are there any 0:os ie less than 15?
    bool succededMove = false;
    int currentIndex, moveToIndex, itemHomeHash;

    if (hopBits[trueHash] < 15)
    {
        vector<int> searchIndex = calculateBitLocation(hopBits[trueHash]); 
        while (!searchIndex.empty() && !succededMove)
        {
            currentIndex = searchIndex.back();
            searchIndex.pop_back();
            // check hash of that entry
            if(trueHash + currentIndex < bucketSize)
                itemHomeHash = calculateHash(bucket[trueHash + currentIndex], bucketSize);
            else 
                itemHomeHash = calculateHash(bucket[(trueHash + currentIndex)-bucketSize], bucketSize);
            // check bits of that hash index. any free slots?
            if (hopBits[itemHomeHash] < 15)
            {
                // if so move to free slot and update bits.
                searchIndex.clear();
                searchIndex = calculateBitLocation(hopBits[itemHomeHash]);
                moveToIndex = itemHomeHash + searchIndex.back();
  
                // move item in other free bucket
                bucket[moveToIndex] = bucket[trueHash + currentIndex];
                bitOp("SUB", itemHomeHash, moveToIndex-currentIndex);
                // empty bucket and update index bits
                bucket[trueHash + currentIndex] = "";
                bitOp("OR", itemHomeHash, currentIndex);
                succededMove = true;
            }
        }
    }
    return succededMove;
}

int myBucket::calculateHash(const string& key, int tableSize)
{
    // unsigned to avoid negative number
    unsigned int hashValue = 0;

    for (char myChar : key)
    {
        hashValue = myChar;//4;//37 * hashValue + myChar;
    }
    return hashValue % tableSize;
}

void myBucket::insertData()
{
    bucket[1] = "a";
    hopBits[1] = 1;
    bucket[2] = "b";
    hopBits[2] = 7;
    bucket[3] = "bb";
    hopBits[3] = 0;
    bucket[4] = "bbb";
    hopBits[4] = 0;
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
