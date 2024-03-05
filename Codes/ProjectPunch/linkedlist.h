/* 
 * I can't believe I'm implementing a linked list 10 years after college data structures.
 */

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stddef.h>
#include <Brawl/Message.h>

#include "common.h"

typedef struct listnode {
    ~listnode(){};
    void* data = 0;
    struct listnode* next = 0;
    struct listnode* prev = 0;
} listnode_t;

template <class T> class LinkedlistIterator;

template <class T>
class linkedlist {
    public:
        linkedlist();
        ~linkedlist<T>();

        T& head();
        T& tail();

        void append(T& element);
        void prepend(T& element);

        size_t length = 0;
    protected:
        listnode_t* headNode;
        listnode_t* tailNode;
    friend class LinkedlistIterator<T>;
};

template <class T>
linkedlist<T>::linkedlist() {
    this->headNode = new listnode_t();
    this->tailNode = new listnode_t();
    this->headNode->next = this->tailNode;
    this->tailNode->prev = this->headNode;
}



template<class T>
linkedlist<T>::~linkedlist<T>(){
    listnode_t* currentNode = this->headNode;
    listnode_t* nextNode;
    while (currentNode != nullptr) {
        nextNode = currentNode->next; 
        delete currentNode;
        currentNode = nextNode;
    }
}

template<class T>
T& linkedlist<T>::head() {
    if (length == 0) {
        return 0;
    }

    return *((T*)this->head->data);
}

template<class T>
T& linkedlist<T>::tail() {
    if (length == 0) {
        return 0;
    }

    return *((T*)this->tail->data);
}

template<class T>
void linkedlist<T>::prepend(T& element) {
    listnode_t* newNode = new listnode_t;
    newNode->data = &element;
    newNode->prev = this->headNode;
    newNode->next = this->headNode->next;
    newNode->next->prev = newNode;
    newNode->prev->next = newNode;
}

template<class T>
void linkedlist<T>::append(T& element) {
    listnode_t* newNode = new listnode_t;
    newNode->data = &element;
    newNode->next = this->tailNode;
    newNode->prev = this->tailNode->prev;
    newNode->next->prev = newNode;
    newNode->prev->next = newNode;
}

template <class T>
class LinkedlistIterator {
    public:
        LinkedlistIterator(linkedlist<T>& list);
        ~LinkedlistIterator<T>();
        void deleteHere();
        void insertHere(T& element);
        T* next();
    private:
        listnode_t* current;
};

template<class T>
LinkedlistIterator<T>::LinkedlistIterator(linkedlist<T>& list) {
    this->current = list.headNode;
}

template<class T>
LinkedlistIterator<T>::~LinkedlistIterator<T>() {}

template<class T>
void LinkedlistIterator<T>::deleteHere() {
    /* 
     * Note, this doesn't delete the element that used to be here.
     * delete what comes out of next() instead :) 
    */

    if (this->current == nullptr) {
        OSReport("Tried to delete null element from linked list.\n");
        return;
    }

    listnode_t* goingAway = this->current;
    if (this->current->prev == nullptr) {
        OSReport("Deleting the head node??\n");
        return;
    }

    this->current = this->current->prev;

    if (goingAway->prev != nullptr) {
        goingAway->prev->next = goingAway->next;
    }

    if (goingAway->next != nullptr) {
        goingAway->next->prev = goingAway->prev;
    }

    delete goingAway;
    return;
};

template<class T>
void LinkedlistIterator<T>::insertHere(T& element) {
    listnode_t* newNode = new listnode_t();
    newNode->data = &element;
    newNode->next = this->current->next;
    newNode->prev = this->current;
    this->current->next = newNode;
    this->current = newNode;
    if (newNode->next != nullptr) {
        newNode->next->prev = newNode;
    }
};

template<class T>
T* LinkedlistIterator<T>::next(){
    if (this->current->next == nullptr) {
        return nullptr;
    }

    this->current = this->current->next;
    return (T*)(this->current->data);
};

void testLinkedList();
#endif