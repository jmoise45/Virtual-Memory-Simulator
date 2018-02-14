#include <iostream>
#include <algorithm>
#include "pa2.h"

Node::Node(string name, int memorySize) {

    this->name = name;
    this->memorySize = memorySize;
    this->nextNode = NULL;
}

string Node::getName() {

    return name;
}

void Node::setName(string &name) {

    this->name = name;
}

int Node::getMemorySize() {

    return memorySize;
}

void Node::setMemorySize(int &memorySize) {

    this->memorySize = memorySize;
}

Node* Node::getNextNode() {

    return nextNode;
}

void Node::setNextNode(Node *node) {

    nextNode = node;
}

int Node::getIndexNumber() {

    return indexNumber;
}

void Node::setIndexNumber(int &indexNumber) {

    this->indexNumber = indexNumber;
}

int Node::getMarker() {

    return marker;
}

void Node::setMarker(int &marker) {

    this->marker = marker;
}

LinkedList::LinkedList() {

    //These variables are used as default values for a Node object's data fields
    string free = "free";

    int pageSize = 4;

    int freeIndexNumber = 0;

    int defaultMarker = 0;

    //the head data field is defined
    head = new Node(free, pageSize);

    head->setIndexNumber(freeIndexNumber);

    head->setMarker(defaultMarker);

    Node *copyNode = new Node(free, pageSize);

    copyNode->setIndexNumber(freeIndexNumber);

    copyNode->setMarker(defaultMarker);

    //The head data field's Node pointer points to a new Node object
    head->setNextNode(copyNode);

    //A for loop is created to set up a chain of Node objects connected by pointers (32 Node objects in total)
    for (int i = 0; i < 31; i++) {

        if(i == 30)
        {

            continue;
        }

        copyNode->setNextNode(new Node(free, pageSize));

        copyNode = copyNode->getNextNode();

        copyNode->setIndexNumber(freeIndexNumber);

        copyNode->setMarker(defaultMarker);
    }

    //The last Node object in the chain has its Node pointer pointed to nullptr
    copyNode->setNextNode(nullptr);
}

//Adds a program to memory space using the worst-fit algorithm
void LinkedList::worstAddition(string &name, int &memorySize, int &fragments)
{
    //Create a Node pointer to the head data field of a LinkedList object
    Node *node = head;

    /*Make a while loop that checks each Node object's name in the LinkedList to see if the given argument for the string parameter
    "name" exists*/
    while(node != nullptr)
    {

        if(node->getName() == name)
        {
            cout << "Error, Program " << name << " already running.\n" << endl;

            return;
        }

        else
        {
            node = node->getNextNode();
        }
    }

    //If the given size of the program is greater than 128, then an error statement is printed
    if(memorySize > 128)
    {

        cout << "Error, not enough memory for Program " << name << "\n" << endl;
    }

    else
    {

        //This variable will be used to hold the actual memory size assigned to a given program in the memory space
        int assignedMemorySize;

        //Create an array that holds all the possible page sizes in memory space (multiples of 4 up to 128)
        int sizes[32] = { };

        //Use a for loop to assign each element a multiple of 4 up to 128 in sequential order
        for(int i = 1; i <= 32; i++)
        {

            sizes[i-1] = 4*i;
        }

        //Use a for loop to determine the actual memory size assigned to a given program in the memory space
        for(int i = 0; i < 32; i ++)
        {

            if(memorySize <= sizes[i])
            {

                assignedMemorySize = sizes[i];

                break;
            }
        }

        //The assigned memory size will determine the number of pages that the program takes up in the memory space
        int pages = (assignedMemorySize/4);

        //If there are no fragments, then the worst-fit algorithm will not be needed
        if (fragments == 0)
        {

            Node *copyNode = head;

            //The while loop adds the program to the memory space based on the number of pages it contains
            while(pages > 0)
            {

                if(copyNode->getName() == "free")
                {

                    copyNode->setName(name);

                    int defaultMarker = 0;

                    copyNode->setMarker(defaultMarker);

                    --pages;

                    if(copyNode->getNextNode() == nullptr) {

                        break;
                    }

                    copyNode = copyNode->getNextNode();
                }
            }

            pages = (assignedMemorySize/4);

            cout << "Program " << name << " added successfully: " << pages << " page(s) used.\n" << endl;
        }

        //This implies that there are one or more fragments in the memory space
        else
        {

            //This variable is used to mark the free pages in the memory space
            int markCount = 1;

            int markUsed = 0;

            Node *copyNode = head;

            //Create a while loop to "mark" all the free pages
            while(copyNode->getNextNode() != nullptr)
            {

                if(copyNode->getName() == "free")
                {

                    if(markUsed == markCount)
                    {

                        ++markCount;
                    }

                    //The current Node object's marker data field will be set to the value of markCount if it's name data field is "free"
                    copyNode->setMarker(markCount);

                    /*Through each iteration, markUsed will equal markCount in order to change the value of markCount for the next
                    iteration. This is done in order to "mark" distinct contiguous pages that are "free." The first block of
                    contiguous pages will contain Node objects with a value of 1 for their marker data field. The second
                    block of contiguous pages will contain Node objects with a value of 2 for their marker data field, and
                    so on.*/
                    markUsed = markCount;

                    //Points to the next Node object in the LinkedList
                    copyNode = copyNode->getNextNode();

                    //Create a while loop that will "mark" contiguous pages with same value of markCount
                    while(copyNode->getName() == "free")
                    {

                        copyNode->setMarker(markCount);

                        if(copyNode->getNextNode() == nullptr)
                        {

                            break;
                        }

                        else {

                            copyNode = copyNode->getNextNode();
                        }
                    }
                }

                else
                {

                    copyNode = copyNode->getNextNode();
                }
            }

            //For last page

            if(copyNode->getName() == "free" && copyNode->getMarker() == 0)
            {

                ++markCount;

                copyNode->setMarker(markCount);

                markUsed = markCount;
            }


            /*Create an array with a size equal to markCount's final value after the previous code, which implies that markCount will be
            greater than or equal to 1. Each element of the markers array will contain a value that determines how many Node objects
            contain a given markCount value of (index number + 1). For example, markers[0] contains the amount of Node objects with
            a value of 1 for their marker data field.*/
            int markers[markCount] = { };

            Node *node = head;

            /*This while loop counts how many Node objects in a LinkedList object have a marker data field with a value equal to a given
            markCount value*/
            while(markCount > 0)
            {

                while(node != nullptr)
                {

                    if(node->getMarker() == markCount && node->getName() == "free")
                    {

                        markers[markCount-1] = markers[markCount-1] + 1;
                    }

                    node = node->getNextNode();

                }

                node = head;

                --markCount;
            }

            /*The variable maxSpace determines which index in the markers array has the highest value, which implies the highest number
            of Node objects that are named "free" in the memory space*/
            int maxSpace = 0;

            //This for loop determines the index in the markers array with the highest value, which is equal to maxSpace
            for(unsigned int i = 0; i < (sizeof(markers)/sizeof(*markers)); i++)
            {

                for(unsigned int j = 0; j < (sizeof(markers)/sizeof(*markers)); j++)
                {

                    if(markers[i] >= markers[j])
                    {

                        maxSpace = i;
                    }
                }
            }

            /*This variable indicates the marker data field value that is contained by the most Node objects
            with the name "free" in a LinkedList object, which determines the "biggest hole" of free space in the memory space*/
            int requiredMarker = maxSpace + 1;

            //This variable represents the maximum number of "free" pages in the memory space
            int sum = 0;

            //Add up all of the elements of the markers array in a for loop
            for(unsigned int i = 0; i < (sizeof(markers)/sizeof(*markers)); i++)
            {

                sum += markers[i];
            }

            Node *newNode = head;

            /*If the number of pages that the given program takes up is less than or equal to the the block with the greatest
            number of contiguous "free" pages in the memory space*/
            if(pages <= markers[maxSpace])
            {

                /*Make a while loop that sets the name of a "free" page with a marker data field value equal
                to the value of requiredMarker to the name of the program that is being added to the memory space. Also, the marker
                data field of a Node object that meets the conditions is given a value of 0 in order to make sure it is not affected
                when another program gets added to the memory space*/
                while(pages > 0)
                {

                    if(newNode->getMarker() == requiredMarker && newNode->getName() == "free")
                    {

                        newNode->setName(name);

                        int defaultMarker = 0;

                        newNode->setMarker(defaultMarker);

                        --pages;

                        if(newNode->getNextNode() == nullptr) {

                            break;
                        }

                        newNode = newNode->getNextNode();
                    }

                    else {

                        newNode = newNode->getNextNode();
                    }
                }

                pages = (assignedMemorySize/4);

                cout << "Program " << name << " added successfully: " << pages << " page(s) used.\n" << endl;
            }

            /*This implies that the number of pages that the given program takes up is greater than the the block
            with the greatest number of contiguous "free" pages in the memory space*/
            else {

                cout << "Error, Not enough memory for Program " << name << "\n" << endl;
            }
        }
    }
}

void LinkedList::bestAddition(string &name, int &memorySize, int &fragments) {

    //Create a Node pointer to the head data field of a LinkedList object
    Node *node = head;

    /*Make a while loop that checks each Node object's name in the LinkedList to see if the given argument for the string parameter
    "name" exists*/
    while(node != nullptr)
    {

        if(node->getName() == name)
        {
            cout << "Error, Program " << name << " already running.\n" << endl;

            return;
        }

        else
        {
            node = node->getNextNode();
        }
    }

    //If the given size of the program is greater than 128, then an error statement is printed
    if(memorySize > 128)
    {

        cout << "Error, not enough memory for Program " << name << "\n" << endl;
    }

    else
    {

        //This variable will be used to hold the actual memory size assigned to a given program in the memory space
        int assignedMemorySize;

        //Create an array that holds all the possible page sizes in memory space (multiples of 4 up to 128)
        int sizes[32] = { };

        //Use a for loop to assign each element a multiple of 4 up to 128 in sequential order
        for(int i = 1; i <= 32; i++)
        {

            sizes[i-1] = 4*i;
        }

        //Use a for loop to determine the actual memory size assigned to a given program in the memory space
        for(int i = 0; i < 32; i ++)
        {

            if(memorySize <= sizes[i])
            {

                assignedMemorySize = sizes[i];

                break;
            }
        }

        //The assigned memory size will determine the number of pages that the program takes up in the memory space
        int pages = (assignedMemorySize/4);

        //If there are no fragments, then the worst-fit algorithm will not be needed
        if (fragments == 0)
        {
            Node *copyNode = head;

            //The while loop adds the program to the memory space based on the number of pages it contains
            while(pages > 0)
            {

                if(copyNode->getName() == "free")
                {

                    copyNode->setName(name);

                    int defaultMarker = 0;

                    copyNode->setMarker(defaultMarker);

                    --pages;

                    if(copyNode->getNextNode() == nullptr) {

                        break;
                    }

                    copyNode = copyNode->getNextNode();
                }
            }

            pages = (assignedMemorySize/4);

            cout << "Program " << name << " added successfully: " << pages << " page(s) used.\n" << endl;
        }

        //This implies that there are one or more fragments in the memory space
        else
        {
            //This variable is used to mark the free pages in the memory space
            int markCount = 1;

            int markUsed = 0;

            Node *copyNode = head;

            //Create a while loop to "mark" all the free pages
            while(copyNode->getNextNode() != nullptr)
            {

                if(copyNode->getName() == "free")
                {

                    if(markUsed == markCount)
                    {

                        ++markCount;
                    }

                    //The current Node object's marker data field will be set to the value of markCount if it's name data field is "free"
                    copyNode->setMarker(markCount);

                    /*Through each iteration, markUsed will equal markCount in order to change the value of markCount for the next
                    iteration. This is done in order to "mark" distinct contiguous pages that are "free." The first block of
                    contiguous pages will contain Node objects with a value of 1 for their marker data field. The second
                    block of contiguous pages will contain Node objects with a value of 2 for their marker data field, and
                    so on.*/
                    markUsed = markCount;

                    //Points to the next Node object in the LinkedList
                    copyNode = copyNode->getNextNode();

                    //Create a while loop that will "mark" contiguous pages with same value of markCount
                    while(copyNode->getName() == "free")
                    {

                        copyNode->setMarker(markCount);

                        if(copyNode->getNextNode() == nullptr)
                        {

                            break;
                        }

                        else {

                            copyNode = copyNode->getNextNode();
                        }
                    }
                }

                else
                {

                    copyNode = copyNode->getNextNode();
                }
            }

            //For last page

            if(copyNode->getName() == "free" && copyNode->getMarker() == 0)
            {

                ++markCount;

                copyNode->setMarker(markCount);

                markUsed = markCount;
            }

            /*Create an array with a size equal to markCount's final value after the previous code, which implies that markCount will be
            greater than or equal to 1. Each element of the markers array will contain a value that determines how many Node objects
            contain a given markCount value of (index number + 1). For example, markers[0] contains the amount of Node objects with
            a value of 1 for their marker data field.*/
            int markers[markCount] = { };

            Node *node = head;

            /*This while loop counts how many Node objects in a LinkedList object have a marker data field with a value equal to a given
            markCount value*/
            while(markCount > 0)
            {

                while(node != nullptr)
                {

                    if(node->getMarker() == markCount && node->getName() == "free")
                    {

                        markers[markCount-1] = markers[markCount-1] + 1;
                    }

                    node = node->getNextNode();

                }

                node = head;

                --markCount;
            }

            /*The variable maxSpace determines which index in the markers array has the lowest value, which implies the lowest number
            of Node objects that are named "free" in the memory space*/
            int maxSpace = 0;

            //This for loop determines the index in the markers array with the lowest value, which is equal to maxSpace
            for(unsigned int i = 0; i < (sizeof(markers)/sizeof(*markers)); i++)
            {

                for(unsigned int j = 0; j < (sizeof(markers)/sizeof(*markers)); j++)
                {

                    if(markers[i] < markers[j])
                    {

                        maxSpace = i;
                    }
                }
            }

            /*This variable indicates the marker data field value that is contained by the least Node objects
            with the name "free" in a LinkedList object, which determines the "smallest hole" of free space in the memory space*/
            int requiredMarker = maxSpace + 1;

            //This variable represents the maximum number of "free" pages in the memory space
            int sum = 0;

            for(unsigned int i = 0; i < (sizeof(markers)/sizeof(*markers)); i++)
            {

                sum += markers[i];
            }

            Node *newNode = head;

            if(pages <= markers[maxSpace])
            {

                while(pages > 0)
                {

                    if(newNode->getMarker() == requiredMarker && newNode->getName() == "free")
                    {

                        newNode->setName(name);

                        int defaultMarker = 0;

                        newNode->setMarker(defaultMarker);

                        --pages;

                        if(newNode->getNextNode() == nullptr) {

                            break;
                        }

                        newNode = newNode->getNextNode();
                    }

                    else {

                        newNode = newNode->getNextNode();
                    }
                }

                pages = (assignedMemorySize/4);

                cout << "Program " << name << " added successfully: " << pages << " page(s) used.\n" << endl;
            }

            else if(pages <= sum && pages > markers[maxSpace])
            {

                Node *anotherNode = head;

                unsigned int sizeOfMarkers = (sizeof(markers)/sizeof(*markers));

                bool condition = true;

                while(condition == true)
                {

                    int oldMax = markers[maxSpace];

                    for(unsigned int i = 0; i < sizeOfMarkers; i++)
                    {
                        if(markers[i] > oldMax)
                        {

                            for(unsigned int j = 0; j < sizeOfMarkers; j++)
                            {

                                if(markers[j] == oldMax) {

                                    continue;
                                }

                                if(markers[i] <= markers[j])
                                {

                                    maxSpace = i;

                                    break;
                                }
                            }
                        }
                    }

                    requiredMarker = maxSpace + 1;

                    if(pages <= markers[maxSpace])
                    {

                        Node *otherNode = head;

                        while(pages > 0)
                        {

                            if(otherNode->getMarker() == requiredMarker && otherNode->getName() == "free")
                            {

                                otherNode->setName(name);

                                int defaultMarker = 0;

                                otherNode->setMarker(defaultMarker);

                                --pages;

                                if(otherNode->getNextNode() == nullptr)
                                {

                                    break;
                                }

                                otherNode = otherNode->getNextNode();
                            }

                            else
                            {

                                otherNode = otherNode->getNextNode();
                            }
                        }

                        pages = (assignedMemorySize/4);

                        cout << "Program " << name << " added successfully: " << pages << " page(s) used.\n" << endl;

                        condition = false;

                    }

                    else
                    {
                        continue;
                    }

                }
            }

            else {

                cout << "Error, Not enough memory for Program " << name << "\n" << endl;
            }
        }
    }
}

void LinkedList::deletion(string &name) {

    Node *node = head;

    string free = "free";

    int defaultMarker = 0;

    int defaultSize = 4;

    int freeIndexNumber = 0;

    int pages = 0;

    while(node != nullptr) {

        if(node->getName() == name) {

            node->setName(free);

            node->setMarker(defaultMarker);

            node->setMemorySize(defaultSize);

            node->setIndexNumber(freeIndexNumber);

            ++pages;

            node = node->getNextNode();
        }

        else {

            node = node->getNextNode();
        }
    }

    if (pages > 0) {

        cout << "Program " << name << " successfully killed, " << pages << " page(s) reclaimed.\n" << endl;
    }

    else {

        cout << "Program " << name << " does not exist.\n" << endl;
    }
}

void LinkedList::print() {

    cout << head->getName() << " ";

    Node *copyNode = head;

    for(int i = 0; i < 7; i++) {

        copyNode = copyNode->getNextNode();

        cout << copyNode->getName() << " ";
    }

    cout << endl;

    for(int i = 0; i < 8; i++) {

        copyNode = copyNode->getNextNode();

        cout << copyNode->getName() << " ";
    }

    cout << endl;

    for(int i = 0; i < 8; i++) {

        copyNode = copyNode->getNextNode();

        cout << copyNode->getName() << " ";
    }

    cout << endl;

    for(int i = 0; i < 8; i++) {

        if(copyNode->getNextNode() != nullptr)
        {
            copyNode = copyNode->getNextNode();
        }

        cout << copyNode->getName() << " ";
    }

    cout << endl;
}

int LinkedList::fragmentation() {

    int indexCount = 1;

    Node *copyNode = head;

    while(copyNode->getNextNode() != nullptr) {

        if(copyNode->getName() != "free") {

            copyNode->setIndexNumber(indexCount);

            copyNode = copyNode->getNextNode();

            while(copyNode->getName() != "free") {

                ++indexCount;

                copyNode->setIndexNumber(indexCount);

                if(copyNode->getNextNode() == nullptr)
                {

                    break;
                }

                else {

                    copyNode = copyNode->getNextNode();
                }
            }

            indexCount = 1;
        }

        else {

            copyNode = copyNode->getNextNode();
        }
    }

    //For the last page

    if(copyNode->getName() != "free") {

        copyNode->setIndexNumber(indexCount);
    }

    int fragments = 0;

    copyNode = head;

    while(copyNode->getNextNode() != nullptr) {

        if(copyNode->getIndexNumber() == 1) {

            ++fragments;
        }

        copyNode = copyNode->getNextNode();
    }

    //For last page

    if(copyNode->getIndexNumber() == 1) {

        ++fragments;
    }

    return fragments;
}

int main()
{
    int c = 0;

    string choice = "";

    LinkedList *lList = new LinkedList();

    while (c == 0)
    {
        cout << "Please choose an algorithm (type either worst-fit or best-fit): ";

        cin >> choice;

        cout << endl;

        if (choice != "worst-fit" && choice != "best-fit")
        {

            cout << "That is not valid. Try again." << endl;
        }

        else
        {

            c++;
        }
    }

    if (choice == "worst-fit")
    {

        cout << "Using worst fit algorithm\n" << endl;

        cout << "1. Add Program"
        "\n2. Kill Program"
        "\n3. Fragmentation"
        "\n4. Print memory"
        "\n5. Exit\n" << endl;

        bool condition = true;

        do {

            cout << "choice - ";

            string option;

            cin >> option;

            cout << endl;

            if (option == "1")
            {

                cout << "Program name - ";

                string programName = "";

                cin >> programName;

                cout << endl;

                cout << "Program size (KB) - ";

                int programSize;

                cin >> programSize;

                cout << endl;

                int fragments = lList->fragmentation();

                lList->worstAddition(programName, programSize, fragments);
            }

            if (option == "2") {

                cout << "Program name - ";

                string programName = "";

                cin >> programName;

                cout << endl;

                lList->deletion(programName);
            }

            if (option == "3") {

                cout << "There are " << lList->fragmentation() << " fragment(s).\n" << endl;
            }

            if (option == "4") {

                lList->print();

                cout << endl;
            }

            if (option == "5") {

                condition = false;
            }

            if (option != "1" && option != "2" && option != "3" && option != "4" && option != "5") {

                cout << "Invalid input.\n" << endl;
            }


        }while(condition == true);
    }

    else if(choice == "best-fit")
    {

        cout << "Using best fit algorithm\n" << endl;

        cout << "1. Add Program"
        "\n2. Kill Program"
        "\n3. Fragmentation"
        "\n4. Print memory"
        "\n5. Exit\n" << endl;

        bool condition = true;

        do
        {
            cout << "choice - ";

            string option;

            cin >> option;

            cout << endl;

            if (option == "1")
            {

                cout << "Program name - ";

                string programName = "";

                cin >> programName;

                cout << endl;

                cout << "Program size (KB) - ";

                int programSize;

                cin >> programSize;

                cout << endl;

                int fragments = lList->fragmentation();

                lList->bestAddition(programName, programSize, fragments);
            }

            if (option == "2")
            {

                cout << "Program name - ";

                string programName = "";

                cin >> programName;

                cout << endl;

                lList->deletion(programName);
            }

            if (option == "3")
            {

                cout << "There are " << lList->fragmentation() << " fragment(s).\n" << endl;
            }

            if (option == "4")
            {

                lList->print();

                cout << endl;
            }

            if (option == "5")
            {

                condition = false;
            }

            if (option != "1" && option != "2" && option != "3" && option != "4" && option != "5")
            {

                cout << "Invalid input.\n" << endl;
            }

        }
        while(condition == true);

    }

    return 0;
}
