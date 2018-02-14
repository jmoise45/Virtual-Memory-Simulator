#ifndef PA2_H_INCLUDED
#define PA2_H_INCLUDED
#include <string>
using namespace std;

class Node
{

private:

    string name;
    int memorySize;
    //Pointer to the next Node object in a LinkedList object for a given Node object
    Node *nextNode;
    //Used to count pages that are not free to count fragments in memory space
    int indexNumber;
    //Used to count pages that are free in order to allocate programs in memory space using either worst-fit or best-fit algorithm
    int marker;

public:

    Node(string name, int memorySize);
    string getName();
    void setName(string &name);
    int getMemorySize();
    void setMemorySize(int &memorySize);
    Node* getNextNode();
    void setNextNode(Node *node);
    int getIndexNumber();
    void setIndexNumber(int &indexNumber);
    int getMarker();
    void setMarker(int &marker);

};

class LinkedList
{

private:
    //The first Node object in a LinkedList object
    Node *head;

public:

    LinkedList();
    //Adds a program to memory space using the worst-fit algorithm
    void worstAddition(string &name, int &memorySize, int &fragments);
    //Adds a program to memory space using the best-fit algorithm
    void bestAddition(string &name, int &memorySize, int &fragments);
    //Kills a program in memory space
    void deletion(string &name);
    //Prints out the current state of memory space
    void print();
    //Returns the number of fragments in memory space
    int fragmentation();

};





#endif // PA2_H_INCLUDED
