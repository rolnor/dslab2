#include <iostream>
#include <vector>
#include <string>



using namespace std;

// helper functions
void getLine(string& inputString);
void menu();

// global variables
const int tableSize = 10;

class myTable
{
private:
    string data[tableSize];
    int collisions;
public:
    myTable() { collisions = 0;};
    void insert(string& inputData,int hashLocation);
    void search(string& inputData, int hashLocation);
    void remove(string& inputData, int hashLocation);
    void repartition(int trueHash, int newHashLocation);
    int calculateHash(const string& key, int tableSize);
    void incCollisions();
    int getCollisions() { return collisions; };
    void print();
};

int main()
{
    char choise = ' ';
    myTable linearTable;
    string inputString = "";
    while (choise != '0')
    {
        cout << endl << endl << "Collisions: " << to_string(linearTable.getCollisions()) << endl << endl;
        menu();
        cin >> choise;
        switch (choise)
        {
            case '1':
                cout << endl << "Insert: ";
                getLine(inputString);
                linearTable.insert(inputString, linearTable.calculateHash(inputString, tableSize));
                break;
            case '2':
                cout << endl << "Search: ";
                getLine(inputString);
                linearTable.search(inputString, linearTable.calculateHash(inputString, tableSize));
                break;
            case '3':
                cout << endl << "Remove: ";
                getLine(inputString);
                linearTable.remove(inputString, linearTable.calculateHash(inputString, tableSize));
                break;
            case '4':
                linearTable.print();
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

void myTable::insert(string& inputData, int hashLocation)
{
    bool algCompleted = false;
    int i = 0;

    while (!algCompleted)
    {
        if (data[hashLocation] == "")
        {
            data[hashLocation] = inputData;
            algCompleted = true;
        }
        // stop if not unique
        else if (data[hashLocation] == inputData)
            algCompleted = true;
        // else continue circular searching 
        else if (hashLocation < tableSize - 1)
        {
            hashLocation += 1;
            incCollisions();
        }
        else
        {
            hashLocation = 0;
            incCollisions();
        }


        // only allow one pass
        if (i == tableSize - 1)
        {
            cout << endl << "Key not found" << endl;
            break;
        }
        else i++;
    }
}

void myTable::search(string& inputData, int hashLocation)
{
    bool algCompleted = false;
    int i = 0;

    while (!algCompleted)
    {
        if (data[hashLocation] == "")
        {
            cout << endl << "Key not found" << endl;
            algCompleted = true;
        }
        else if (data[hashLocation] == inputData)
        {
            cout << endl << "Key found" << endl;
            algCompleted = true;
        }
        else if (hashLocation < tableSize - 1)
            hashLocation += 1;
        else
            hashLocation = 0;

        // only allow one pass
        if (i == tableSize - 1)
        {
            cout << endl << "Key not found" << endl;
            break;
        }else i++;
    }
}

void myTable::remove(string& inputData, int hashLocation)
{
    bool algCompleted = false;
    int traverser = hashLocation;

    int i = 0;

    while (!algCompleted && i < tableSize-1)
    {
        if (data[traverser] == "")
        {
            algCompleted = true;
        }
        else if (data[traverser] == inputData)
        {
            data[traverser] = "";
            repartition(hashLocation, traverser);
        }
        else if (traverser < tableSize - 1)
            traverser += 1;
        else
            traverser = 0;
    }
}

// new hashlocation is empty
void myTable::repartition(int trueHash, int newHashLocation)
{
    bool algCompleted = false;
  //  int trailingHashLocation;
    int openSlot = newHashLocation;
    int i = 0,y;

    while(!algCompleted)
    {
        if (newHashLocation + 1 < tableSize)
            newHashLocation++;
        else newHashLocation = 0;

        if (trueHash == calculateHash(data[newHashLocation], tableSize))
        {
            data[openSlot] = data[newHashLocation];
            data[newHashLocation] = "";
            openSlot = newHashLocation;
        }
        else if((data[newHashLocation] != "") && openSlot >= calculateHash(data[newHashLocation], tableSize))
        {
            data[openSlot] = data[newHashLocation];
            data[newHashLocation] = "";
            openSlot = newHashLocation;
        }
        // stop if empty slot or correct hash
        else if(data[newHashLocation] == "")
            algCompleted = true;
    }
}

int myTable::calculateHash(const string& key, int tableSize)
{
    // unsigned to avoid negative number
    unsigned int hashValue = 0;

    for (char myChar : key)
    {
        hashValue = myChar;//9;//37 * hashValue + myChar;
    }
    return hashValue % tableSize;
}

void myTable::incCollisions()
{
    collisions++;
}

void myTable::print()
{
    int i = 0;
    cout << endl << "Table content" << endl;
    for (string key : data)
    {
        cout << i << " : " << key << endl;
        i++;
    }
}
