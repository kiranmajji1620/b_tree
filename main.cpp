#include<iostream>
#include"binary_tree.h"

int main(){
    BTree<int, 5> t;
    t.insert(10);
    t.insert(11);
    t.insert(12);
    t.insert(13);
    t.insert(14);
    t.insert(15);
    t.insert(16);
    t.insert(17);
    t.insert(18);
    t.insert(19);
    t.insert(1);
    t.insert(2);
    t.insert(3);
    cout << "Traversal of B Tree : " << endl;
    t.traverse();
    cout << "Removing 15" << endl;
    t.remove(15);
    t.traverse();

    // cout << "Removing 12" << endl;
    // t.remove(12);
    // t.traverse();

    // cout << "Removing 20" << endl;
    // t.remove(20);
    // t.traverse();

    // cout << "Removing 90" << endl;
    // t.remove(90);
    // t.traverse();


    int value = 17;
    t.search(value);
    value = 18;
    t.search(value);
    cout << endl;

    return 0;
}