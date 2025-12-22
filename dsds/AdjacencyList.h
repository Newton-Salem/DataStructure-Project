#pragma once
#include <iostream>
using namespace std;

//ADJACENCY LIST NODE FOR REROUTING

class AdjNode {
private:
    int intersection;
    AdjNode* next;

public:
    AdjNode(int i) {
        intersection = i;
        next = NULL;
    }

    int getIntersection() const { return intersection; }
    AdjNode* getNext() const { return next; }
    void setNext(AdjNode* n) { next = n; }
};


//ADJACENCY LIST FOR INTERSECTION CONNECTIONS

class AdjacencyList {
private:
    AdjNode* head;

public:
    AdjacencyList() {
        head = NULL;
    }

    void addConnection(int intersection) {
        AdjNode* newNode = new AdjNode(intersection);
        newNode->setNext(head);
        head = newNode;
    }

    AdjNode* getHead() const {
        return head;
    }

    int getFirstConnection() const {
        if (head != NULL) return head->getIntersection();
        return -1;
    }
};
