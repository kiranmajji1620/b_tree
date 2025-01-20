#include<iostream>
#include"binary_tree.h"

int main(){
    BTree<int, 5> t;
    t.insert(10);
    t.insert(20);
    t.insert(5);
    t.insert(6);
    t.insert(12);
    t.insert(30);
    t.insert(7);
    t.insert(17);
    cout << "Traversal of B Tree : " << endl;
    t.traverse();
    cout << endl;
    return 0;
}