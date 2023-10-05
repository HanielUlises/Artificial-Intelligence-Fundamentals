#include "graph.h"

List::List(int value){
    Node *newNode = new Node (value);
    head = newNode;
    tail = newNode;
    lenght = 1;
}

void List::Append(int value){
    Node *newNode = new Node (value);
    newNode -> next = nullptr;

    tail->next = newNode;
    tail = newNode;

    lenght++;
}