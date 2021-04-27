#include <iostream>
#include <vector>
#include <string>



using namespace std;
void menu();

// helper functions
void getLine(string& inputString);

// global variables
const int tableSize = 10;

class myTable
{
private:
    string data[tableSize];
public:
    void insert(string& inputData,int hashLocation);
    void search(string& inputData, int hashLocation);
    int calculateHash(const string& key, int tableSize);
    void print();
};

int main()
{
    char choise = ' ';
    myTable linearTable;
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
                linearTable.insert(inputString, linearTable.calculateHash(inputString, tableSize));
                break;
            case '2':
                cout << endl << "Search: ";
                getLine(inputString);
                linearTable.search(inputString, linearTable.calculateHash(inputString, tableSize));
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
            hashLocation += 1;
        else
            hashLocation = 0;

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

int myTable::calculateHash(const string& key, int tableSize)
{
    // unsigned to avoid negative number
    unsigned int hashValue = 0;

    for (char myChar : key)
    {
        hashValue = 37 * hashValue + myChar;
    }
    return hashValue % tableSize;
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
