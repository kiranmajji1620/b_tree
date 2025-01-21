#include <iostream>
using namespace std;
#include <bits/stdc++.h>
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
        // In case of odd order like 5, the no of keys will be 4. {10, 20, 30, 40} will split into {10} and {30, 40}.
        // In case of even order like 6, the no of keys will be 5. {10, 20, 30, 40, 50} will split into {10, 20} and {40, 50}.
        if(ORDER%2){
            z -> n = ORDER/2;
        }
        else {
            z -> n = ORDER/2 - 1;
        }
        y -> n = ORDER/2 - 1; // just change the no of keys instead of removing them.
        for(int j = 0; j < z -> n; j++){ // In case of odd no of keys, we start at the middle element. right half in case of even elements.
            z -> keys[j] = y -> keys[j + ORDER/2];
        }
        if(!y -> leaf){ // In case y is an internal node, transfer the children of y to z.
            for(int j = 0; j < z -> n + 1; j++){
                z -> children[j] = y -> children[j + ORDER/2];
            }
        }
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
        if(x -> leaf){ // If we are at leaf, just insert the key.
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
            if(x -> children[i] -> n == ORDER - 1){ // In case of when the subtree root node that contains the child is full, split it and push to the right node.We do this to ensure not going back upwards when the split happens at the leaf.
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
                // cout << "Traversing to child " << x -> keys[i] << endl;
                traverse(x -> children[i]);
            }
            cout << x -> keys[i] << " ";
        }
        if(!x -> leaf){ // Traversing the last sub tree.
            traverse(x -> children[i]);
        }
        // cout << endl;
        // cout << "completed one level" << endl;
    }
    BTreeNode<T, ORDER>* search(BTreeNode<T, ORDER>* x, T k){
        int i = 0;
        while(i < x -> n && k > x -> keys[i]){
            i++;
        }
        // we are now at a point where keys[i] might be equal to or greater than k.
        // if equal, return current node. else if current node is leaf, return null else search in the possible node.
        if(i < x -> n && k == x -> keys[i]){
            return x;
        }
        if(x -> leaf){
            return nullptr;
        }
        return search(x -> children[i], k);
    }

    T getPredecessor(BTreeNode<T, ORDER>* node, int ind){
        BTreeNode<T, ORDER>* currentChild = node -> children[ind]; // left child of the key present at ind.
        while(!currentChild -> leaf){
            currentChild = currentChild->children[currentChild -> n]; // go to the last element of the current child
        }
        return currentChild -> keys[currentChild -> n - 1];
    }

    T getSuccesor(BTreeNode<T, ORDER>* node, int ind){
        BTreeNode<T, ORDER>* currentChild = node -> children[ind + 1];
        while(!currentChild -> leaf){
            currentChild = currentChild -> children[0];
        }
        return currentChild -> keys[0];
    }

    // Siblings have an extra key => borrow one key from them.
    // Siblings don't have any extra key => merge with a sibling 
    void fill(BTreeNode<T, ORDER>* node, int ind){
        if(ind != 0 && (node -> children[ind - 1]->n) >= ORDER/2){
            borrowFromPrev(node, ind);
        }
        else if(ind != node -> n && (node -> children[ind + 1]->n) >= ORDER/2){
            borrowFromNext(node, ind);
        }
        else {// Merging always happens with the next node.
            if(ind != node -> n){
                merge(node, ind);
            }
            else {
                merge(node, ind - 1);
            }
        }
    }

    void borrowFromPrev(BTreeNode<T, ORDER>* node, int ind){
        BTreeNode<T, ORDER>* child = node -> children[ind];
        BTreeNode<T, ORDER>* sibling = node -> children[ind - 1];
        for(int i = child -> n - 1; i >= 0; i--){
            child -> keys[i + 1] = child -> keys[i];
        }
        if(!child -> leaf){
            for(int i = child -> n; i >= 0; i--){
                child -> children[i + 1] = child -> children[i];
            }
        }
        child -> keys[0] = node -> keys[ind - 1];
        if(!child -> leaf){
            child -> children[0] = sibling -> children[sibling -> n];
        }
        node -> keys[ind - 1] = sibling -> keys[sibling -> n - 1];
        sibling -> n -= 1;
        child -> n += 1;
    }

    void borrowFromNext(BTreeNode<T, ORDER>* node, int ind){
        BTreeNode<T, ORDER>* child = node -> children[ind];
        BTreeNode<T, ORDER>* sibling = node -> children[ind + 1];
        child -> keys[child -> n] = node -> keys[ind];
        if(!child -> leaf){
            child -> children[child -> n + 1] = sibling -> children[0];
        }
        node -> keys[ind] = sibling -> keys[0];
        for(int i = 1; i < sibling -> n; i++){
            sibling -> keys[i - 1] = sibling -> keys[i];
        }
        if(!sibling -> leaf){
            for(int i = 1; i <= sibling -> n; i++){
                sibling -> children[i - 1] = sibling -> children[i];
            }
        }
        sibling -> n -= 1;
        child -> n += 1;
    }
    // Merge the current child with it's next sibling
    // First transfer the parent's key to the current child and then Transfer the siblings keys and children to the current child
    void merge(BTreeNode<T, ORDER>* node, int ind){
        BTreeNode<T, ORDER>* child = node -> children[ind];
        BTreeNode<T, ORDER>* sibling = node -> children[ind + 1];

        int n = child -> n;
        child -> keys[n] = node -> keys[ind];
        for(int i = 0; i < sibling -> n; i++){
            child -> keys[n + i + 1] = sibling -> keys[i];
        }
        if(!child -> leaf){
            for(int i = 0; i <= sibling -> n; i++){
                child -> children[n + i + 1] = sibling -> children[i];
            }
        }
        for(int i = ind + 1; i < node -> n; i++){
            node -> keys[i - 1] = node -> keys[i];
        }
        for(int i = ind + 2; i <= node -> n; i++){
            node -> children[i - 1] = node -> children[i];
        }
        child -> n += sibling -> n + 1; // because we merged both children and we also pushed the parent key.
        node -> n--;
        delete sibling;
    }

    void removeFromNonLeaf(BTreeNode<T, ORDER>* node, int ind){
        T k = node -> keys[ind];
        if(node -> children[ind]->n >= ORDER/2){
            T pred = getPredecessor(node, ind);
            node -> keys[ind] = pred;
            remove(node -> children[ind], pred);
        }
        else if(node -> children[ind + 1] -> n >= ORDER/2){
            T succ = getSuccesor(node, ind);
            node -> keys[ind] = succ;
            remove(node -> children[ind + 1], succ);
        }
        else {
            merge(node, ind);
            remove(node -> children[ind], k);
        }
    }
    //Removing a key from a node.
    void removeFromLeaf(BTreeNode<T, ORDER>* node, int ind){
        for(int i = ind + 1; i < node -> n; i++){
            node -> keys[i - 1] = node -> keys[i];
        }
        node -> n--;
    }

    void remove(BTreeNode<T, ORDER>* node, int k){
        int ind = 0;
        while(ind < node -> n && k > node -> keys[ind]){
            ind++;
        }
        if(ind < node -> n && node -> keys[ind] == k){
            if(node -> leaf){
                removeFromLeaf(node, ind);
            }
            else {
                removeFromNonLeaf(node, ind);
            }
        }
        else {
            if(node -> leaf){
                cout << "The key hasn't been found" << endl;
                return;
            }
            bool flag = false;
            if(node -> n == ind){
                flag = true;
            }
            if(node -> children[ind] -> n < ORDER/2){
                fill(node, ind);
            }
            if(flag && ind > node -> n){
                remove(node -> children[ind - 1], k);
            }
            else {
                remove(node -> children[ind], k);
            }
        }
    }
public:
    bool showEveryOpn = false;
    BTree() {
        root = new BTreeNode<T, ORDER> (true);
    }
    void insert(T k){
        // cout << "current children in root :  " << root -> keys[0] << endl;
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
        if(showEveryOpn){
            BFS();
        }
    }
    void traverse(){
        // cout << "root " << root -> keys[0] << endl;
        if(root){
            traverse(root);
        }
        cout << endl;
    }
    void search(T k){
        BTreeNode<T, ORDER>* nodeFound = search(root, k);
        if(search(root, k)){
            cout << "We have found the value at " << nodeFound << endl;
        }
        else {
            cout << "Couldn't find the value." << endl;
        }
    }
    void remove(T k){
        if(!root){
            cout << "Cannot remove from an empty tree" << endl;
            return;
        }
        remove(root, k);
        // What if root -> n == 0???
        if(root -> n == 0){
            BTreeNode<T, ORDER>* temp = root;
            if(root -> leaf){
                root = nullptr;
            }
            else {
                root = root -> children[0];
            }
            delete temp;
        }
        if(showEveryOpn){
            BFS();
        }
    }
    void BFS() {
        if(!root){
            cout << "Empty tree" << endl;
            return;
        }
        queue<BTreeNode<T, ORDER>*> que;
        que.push(root);
        while(!que.empty()){
            int noOfNodes = que.size();
            for(int i = 0; i < noOfNodes; i++){
                BTreeNode<T, ORDER>* tempNode = que.front();
                que.pop();
                int noOfKeys = tempNode -> n;
                for(int i = 0; i < noOfKeys; i++){
                    cout << tempNode -> keys[i] << " ";
                }
                for(int i = 0; i < noOfKeys + 1; i++){
                    if(tempNode -> children[i]){
                        que.push(tempNode -> children[i]);
                    }
                }
                cout << "\t|\t";
            }
            cout << endl << endl;
        }
        for(int i = 0;i < 40; i++){
            cout << "-";
        }
        cout << endl;
    }
};