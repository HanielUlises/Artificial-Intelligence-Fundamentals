#include "iostream"

// Graph's adjacency list is an array of simple linked lists
class Node{
    int value;
    Node *next;

    Node (int value){
        this->value = value;
    }
};

class List{
public:
    List (int value);
    void Append(int value);
private:
    Node *head;
    Node *tail;
    int length = 0;
};

class Graph{

};