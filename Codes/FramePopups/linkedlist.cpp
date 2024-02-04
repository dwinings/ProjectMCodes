#include "./linkedlist.h"


void testLinkedList() {
    linkedlist<int>* ll = new linkedlist<int>();

    int ints[4] = {1, 2, 3, 0}; 
    int* intPtr = ints;

    ll->append(*intPtr++);
    ll->append(*intPtr++);
    ll->append(*intPtr++);
    ll->prepend(*intPtr++);

    auto itr = ll->iterator();

    OSReport("Linked list test 1:\n");
    int* p;
    while((p = itr->next()) != nullptr){
        OSReport(" - %d\n", *p);
    }

    delete itr;

    itr = ll->iterator();
    itr->next(); // 0
    itr->next(); // 1
    itr->deleteHere(); // goodbye 1

    int* newInt = new int();
    *newInt = 99;
    itr->insertHere(*newInt); // hello 99
    p = itr->next();

    delete itr;

    itr = ll->iterator();

    while((p = itr->next()) != nullptr){
        OSReport(" - %d\n", *p);
    }


    delete itr;
    delete ll;
    delete newInt;
}