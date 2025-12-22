#pragma once
#include <iostream>
#include "Vehicle.h"
using namespace std;
// ============================================================================
// REGULAR QUEUE (FCFS) FOR PT, NC, FV
// ============================================================================
class Queue {
private:
    Vehicle* head;
    Vehicle* tail;

public:
    Queue() {
        head = tail = NULL;
    }

    void enqueue(Vehicle* v) {
        v->setNext(NULL);
        if (tail == NULL) {
            head = tail = v;
        }
        else {
            tail->setNext(v);
            tail = v;
        }
    }

    Vehicle* dequeue() {
        if (head == NULL) return NULL;
        Vehicle* temp = head;
        head = head->getNext();
        if (head == NULL) tail = NULL;
        temp->setNext(NULL);
        return temp;
    }

    Vehicle* peek() const {
        return head;
    }

    bool isEmpty() const {
        return head == NULL;
    }

    bool remove(int id) {
        if (head == NULL) return false;

        if (head->getID() == id) {
            head = head->getNext();
            if (head == NULL) tail = NULL;
            return true;
        }

        Vehicle* curr = head;
        while (curr->getNext() != NULL) {
            if (curr->getNext()->getID() == id) {
                Vehicle* toRemove = curr->getNext();
                curr->setNext(toRemove->getNext());
                if (toRemove == tail) tail = curr;
                return true;
            }
            curr = curr->getNext();
        }
        return false;
    }


    void printIDs() const {
        Vehicle* curr = head;
        while (curr != NULL) {
            cout << curr->getID() << " ";
            curr = curr->getNext();
        }
    }
};
