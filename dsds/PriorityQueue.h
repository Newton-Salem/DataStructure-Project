
#pragma once
#include <iostream>
#include "Vehicle.h"
using namespace std;
class PriorityQueue {
private:
    Vehicle* head;

public:
    PriorityQueue() {
        head = NULL;
    }

    void enqueue(Vehicle* v) {
        double priority = v->calculatePriority();

        // If empty or new vehicle has higher priority than head
        if (head == NULL || priority > head->calculatePriority()) {
            v->setNext(head);
            head = v;
            return;
        }

        // Find correct position
        Vehicle* curr = head;
        while (curr->getNext() != NULL && curr->getNext()->calculatePriority() >= priority) {
            curr = curr->getNext();
        }

        v->setNext(curr->getNext());
        curr->setNext(v);
    }

    Vehicle* dequeue() {
        if (head == NULL) return NULL;
        Vehicle* temp = head;
        head = head->getNext();
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
            return true;
        }

        Vehicle* curr = head;
        while (curr->getNext() != NULL) {
            if (curr->getNext()->getID() == id) {
                curr->setNext(curr->getNext()->getNext());
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
