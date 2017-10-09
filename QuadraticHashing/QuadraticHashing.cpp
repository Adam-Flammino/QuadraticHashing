// QuadraticHashing.cpp
// Adam Flammino 8/15 SDEV345
// Modifies Gary Savard's Hash table with linear probing example to provide options for quadratic probing and double hashing

#include <iostream>
#include <vector>
#include <cstdlib>                    //for random numbers
#include <ctime>                      //for random numbers


class DataItem
{                                  //(could have more data)
public:
	int iData;                      //data item (key)
									//-----------------------------------------------------------
	DataItem(int ii) : iData(ii)       //constructor
	{  }
	//-----------------------------------------------------------
};  //end class DataItem
	/////////////////////////////////////////////////////////////
class HashTable
{
private:
	std::vector<DataItem*> hashArray;    //vctor holds hash table
	int arraySize;
	DataItem* pNonItem;             //for deleted items
public:
	//----------------------------------------------------------
	HashTable(int size) : arraySize(size)  //constructor
	{
		arraySize = size;
		hashArray.resize(arraySize);    //size the vector
		for (int j = 0; j < arraySize; j++)  //initialize elements
			hashArray[j] = NULL;
		pNonItem = new DataItem(-1);    //deleted item key is -1
	}
	//----------------------------------------------------------
	void displayTable()
	{
		std::cout << "Table: ";
		for (int j = 0; j < arraySize; j++)
		{
			if (hashArray[j] != NULL)
				std::cout << hashArray[j]->iData << " ";
			else
				std::cout << "** ";
		}
		std::cout << std::endl;
	}
	//----------------------------------------------------------
	int hashFunc(int key)
	{
		return key % arraySize;         //hash function
	}
	//----------------------------------------------------------
	int hashFunc2(int key)
	{
		return 7 - key % 7;
	}
	//----------------------------------------------------------
	void insert(DataItem* pItem, int choice)       //insert a DataItem
									   //(assumes table not full)
	{
		int qIndex = 1;
		int key = pItem->iData;         //extract key
		int hashVal = hashFunc(key);    //hash the key
		int dhStep = hashFunc2(key);	// double hash step
		switch (choice) {
			//until empty cell or -1,
		case 1:
			while (hashArray[hashVal] != NULL &&
				hashArray[hashVal]->iData != -1)
			{
				++hashVal;                   //go to next cell
				hashVal %= arraySize;        //wraparound if necessary
			}
			break;
		case 2:
			while (hashArray[hashVal] != NULL &&
				hashArray[hashVal]->iData != -1)
			{
				hashVal += qIndex * qIndex; // step by square of qIndex
				qIndex++;
				hashVal %= arraySize;        //wraparound if necessary
			}
			break;
		case 3:
			while (hashArray[hashVal] != NULL &&
				hashArray[hashVal]->iData != -1)
			{
				hashVal += dhStep; // step using second hash
				hashVal %= arraySize;        //wraparound if necessary
			}
			break;
		default:
			std::cout << "ERROR ON INSERT\n";
		}
		hashArray[hashVal] = pItem;     //insert item
	}  //end insert()
	   //----------------------------------------------------------
	DataItem* remove(int key, int choice)          //remove a DataItem
	{
		int qIndex = 1;
		int hashVal = hashFunc(key);    //hash the key
		int dhStep = hashFunc2(key);	// double hash step
			while (hashArray[hashVal] != NULL)  //until empty cell,
			{                               //found the key?
				if (hashArray[hashVal]->iData == key)
				{
					DataItem* pTemp = hashArray[hashVal]; //save item
					hashArray[hashVal] = pNonItem;        //delete item
					return pTemp;                         //return item
				}
				switch(choice)
				{
				case 1:
					++hashVal;                   //go to next cell
					hashVal %= arraySize;        //wraparound if necessary
					break;
				case 2:
					hashVal += qIndex * qIndex; // step by square of qIndex
					qIndex++;
					hashVal %= arraySize;        //wraparound if necessary
					break;
				case 3:
					hashVal += dhStep; // step using second hash
					hashVal %= arraySize;        //wraparound if necessary
					break;
				default:
					std::cout << "ERROR ON REMOVE\n";
				}
			}

				return NULL;                    //can't find item
	}  //end remove()
	   //----------------------------------------------------------
	DataItem* find(int key, int choice)            //find item with key
	{
		int qIndex = 1;
		int hashVal = hashFunc(key);    //hash the key
		int dhStep = hashFunc2(key);	// double hash step
		switch (choice)
		{
		case 1:
			while (hashArray[hashVal] != NULL)  //until empty cell,
			{                               //found the key?
				if (hashArray[hashVal]->iData == key)
					return hashArray[hashVal];   //yes, return item
				++hashVal;                   //go to next cell
				hashVal %= arraySize;        //wraparound if necessary
			}
			break;
		case 2:
			while (hashArray[hashVal] != NULL)  //until empty cell,
			{                               //found the key?
				if (hashArray[hashVal]->iData == key)
					return hashArray[hashVal];   //yes, return item
				hashVal += qIndex * qIndex; // step by square of qIndex
				qIndex++;
				hashVal %= arraySize; // wrap if needed
			}
			break;
		case 3:
			while (hashArray[hashVal] != NULL)  //until empty cell,
			{                               //found the key?
				if (hashArray[hashVal]->iData == key)
					return hashArray[hashVal];   //yes, return item
				hashVal += dhStep; // step using second hash
				hashVal %= arraySize;        //wraparound if necessaryy
			}
			break;
		default:
			std::cout << "ERROR ON FIND\n";
		}
		return NULL;                    //can't find item
	}
	//----------------------------------------------------------
};  //end class HashTable
	/////////////////////////////////////////////////////////////
int main()
{
	DataItem* pDataItem;
	int aKey, size, n, keysPerCell;
	time_t aTime;
	char choice = 'b';
	int insertType = 0;
	//get sizes
	std::cout << "Enter size of hash table: ";
	std::cin >> size;
	std::cout << "Enter initial number of items: ";
	std::cin >> n;
	while (insertType != 1 && insertType != 2 && insertType != 3)
	{
		std::cout << "Enter 1 for linear probing, 2 for quadratic probing, or 3 for double hashing:\t";
		std::cin >> insertType;
	}
	keysPerCell = 10;
	//make table
	HashTable theHashTable(size);
	srand(static_cast<unsigned>(time(&aTime)));
	for (int j = 0; j < n; j++)        //insert data
	{
		aKey = rand() % (keysPerCell*size);

		pDataItem = new DataItem(aKey);
		theHashTable.insert(pDataItem, insertType);
	}

	while (choice != 'x')          //interact with user
	{
		std::cout << "Enter first letter of "
			<< "show, insert, delete, or find: ";
		char choice;
		std::cin >> choice;
		switch (choice)
		{
		case 's':
			theHashTable.displayTable();
			break;
		case 'i':
			std::cout << "Enter key value to insert: ";
			std::cin >> aKey;
			pDataItem = new DataItem(aKey);
			theHashTable.insert(pDataItem, insertType);
			break;
		case 'd':
			std::cout << "Enter key value to delete: ";
			std::cin >> aKey;
			pDataItem = theHashTable.remove(aKey, insertType);
			if (pDataItem != NULL)
				std::cout << "Deleted " << aKey << std::endl;
			else
				std::cout << "Could not find " << aKey << std::endl;
			break;
		case 'f':
			std::cout << "Enter key value to find: ";
			std::cin >> aKey;
			pDataItem = theHashTable.find(aKey, insertType);
			if (pDataItem != NULL)
				std::cout << "Found " << aKey << std::endl;
			else
				std::cout << "Could not find " << aKey << std::endl;
			break;
		default:
			std::cout << "Invalid entry\n";
		}  //end switch
	}  //end while
	return 0;
}  //end main()

