#include <iostream>
using namespace std;

template <typename T, int ORDER> // to keep the b tree generic. Order is the max no of children in a node.
class BTreeNode {
public:
    T keys[ORDER - 1];
    BTreeNode* children[ORDER];
    int n; //current no of keys
    bool leaf;

    BTreeNode(bool isLeaf = true) : n(0), leaf(isLeaf) {
        for(int i = 0; i < ORDER; i++){
            children[i] = nullptr;
        }
    }
};

template <typename T, int ORDER>
class BTree {
private:
    BTreeNode<T, ORDER>* root;
    void splitChild(BTreeNode<T, ORDER>* x, int i){
        BTreeNode<T, ORDER>* y = x -> children[i];
        BTreeNode<T, ORDER>* z = new BTreeNode<T, ORDER>(y -> leaf);
        z -> n = ORDER/2 - 1;// the no of keys in z.
        for(int j = 0; j < ORDER/2 - 1; j++){ // In case of odd no of keys, we start at the middle element. right half in case of even elements.
            z -> keys[j] = y -> keys[j + ORDER/2];
        }
        if(!y -> leaf){ // In case y is an internal node, transfer the children of y to z.
            for(int j = 0; j < ORDER/2; j++){
                z -> children[j] = y -> children[j + ORDER/2];
            }
        }
        y -> n = ORDER/2 - 1; // just change the no of keys instead of removing them.
        for(int j = x -> n; j >= i + 1; j--){ // Here, we are shifting the childrens by 1 towards right side.
            x -> children[j + 1] = x -> children[j];
        }
        x -> children[i + 1] = z; // y is at i and z will be at i + 1.
        for(int j = x -> n - 1; j >= i; j--){
            x -> keys[j + 1] = x -> keys[j];
        }
        x -> keys[i] = y -> keys[ORDER/2 - 1];
        x -> n = x -> n + 1;
    }

    void insertNonFull(BTreeNode<T, ORDER>* x, T k){
        int i = x -> n - 1;
        if(x -> leaf){
            while(i >= 0 && k < x -> keys[i]){
                x -> keys[i + 1] = x -> keys[i];
                i--;
            }
            x -> keys[i + 1] = k;
            x -> n = x -> n + 1;
        }
        else {
            // Iterate to the correct child and recursively insert the key k.
            while(i >= 0 && k < x -> keys[i]){
                i--;
            }
            i++; // This key should be put in x->children[i].
            if(x -> children[i] -> n == ORDER - 1){
                splitChild(x, i);
                if(k > x -> keys[i]){
                    i++;
                }
            }
            insertNonFull(x -> children[i], k);
        }
    }
    void traverse(BTreeNode<T, ORDER>* x){ // InOrder traversal of b tree
        // cout << "no of keys in x : " << x -> n << endl;
        int i;
        for(i = 0; i < x -> n; i++){
            if(!x -> leaf){
                cout << "Traversing to child " << x -> keys[i] << endl;
                traverse(x -> children[i]);
            }
            cout << x -> keys[i] << " ";
        }
        if(!x -> leaf){ // Traversing the last sub tree.
            traverse(x -> children[i]);
        }
        cout << "completed one level" << endl;
    }
public:
    BTree() {
        root = new BTreeNode<T, ORDER> (true);
    }
    void insert(T k){
        cout << "current children in root :  " << root -> keys[0] << endl;
        if(root -> n == ORDER - 1){
            BTreeNode<T, ORDER>* s = new BTreeNode<T, ORDER>(false);
            s -> children[0] = root;
            root = s;
            splitChild(s, 0);
            insertNonFull(s, k);
        }
        else {
            insertNonFull(root, k);
        }
    }
    void traverse(){
        // cout << "root " << root -> keys[0] << endl;
        if(root){
            traverse(root);
        }
    }
};