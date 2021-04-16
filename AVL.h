#include <iostream>
#include <queue>
using namespace std;

// Declare Node and height early, as the two each reference each other
class Node;
int height(Node* node);

// Used for student identifiers, thus has ID and name (as well as left and right for BST functionality)
class Node {
    int date;
    string country;
    float rate;
    Node* left;
    Node* right;
public:
    // Default constructor, although this should never be used as an empty node
    // is not part of the functionality
    Node() {};
    // Constructor that should be the only one used for inserting students
    Node(int _date, string _country, float _rate, Node* _left=nullptr, Node* _right=nullptr) {
        date = _date;
        country= _country;
        rate = _rate;
        left = _left;
        right = _right;
    }

    // < ,==, and > operators used for insertion into AVL tree
    bool operator<(int n) {
        return date < n;
    }
    bool operator==(int n) {
        return date == n;
    }
    bool operator>(int n) {
        return date > n;
    }
    // Accessors and mutators
    int getDate() {
        return date;
    }
    string getCountry() {
        return country;
    }
    float getRate() {
        return rate;
    }
    Node* getLeft() {
        return left;
    }
    Node* getRight() {
        return right;
    }
    void setDate(int _date) {
        date = _date;
    }
    void setCountry(int _country) {
         country = _country;
    }
    void setRate(float _rate) {
        rate = _rate;
    }
    void setLeft(Node* _left) {
        left = _left;
    }
    void setRight(Node* _right) {
        right = _right;
    }

    // Get balance of current node
    int getBalance() {
        int lHeight = (left) ? height(left) : 0;
        int rHeight = (right) ? height(right) : 0;
        return lHeight - rHeight;
    }
};
// Rotation methods, which are copied and slightly modified from my
// previous Stepik solutions regarding AVL trees

Node* rotateLeft(Node *node) {
    Node* newRoot = node->getRight();
    if (newRoot->getLeft()) {
        Node* newRight = newRoot->getLeft();
        node->setRight(newRight);
    }
    else {
        node->setRight(nullptr);
    }
    newRoot->setLeft(node);
    return newRoot;
}

Node* rotateRight(Node *node) {
    Node* newRoot = node->getLeft();
    if (newRoot->getRight()) {
        Node* newLeft = newRoot->getRight();
        node->setLeft(newLeft);
    }
    else {
        node->setLeft(nullptr);
    }
    newRoot->setRight(node);
    return newRoot;
}

Node* rotateLeftRight(Node *node) {
    node->setLeft(rotateLeft(node->getLeft()));
    return rotateRight(node);
}

Node* rotateRightLeft(Node* node) {
    node->setLeft(rotateRight(node->getRight()));
    return rotateRight(node);
}

// Get height of node; is a recursive method
int height(Node* node) {
    int lHeight = 0;
    int rHeight = 0;
    if (node->getLeft()) {
        lHeight = height(node->getLeft());
    }
    if (node->getRight()) {
        rHeight = height(node->getRight());
    }
    return ((rHeight > lHeight) ? rHeight : lHeight) + 1;
}

// inOrder, preOrder, and postOrder all modify queues by reference which are used for various methods
// such as printing, removingInorder, and finding inOrder successor
void inOrder(Node* node, queue<Node*> &q) {
    if (node==nullptr) {
        return;
    }
    inOrder(node->getLeft(), q);
    q.push(node);
    inOrder(node->getRight(), q);
}

void preOrder(Node* node, queue<Node*> &q) {
    if (node==nullptr) {
        return;
    }
    q.push(node);
    preOrder(node->getLeft(), q);
    preOrder(node->getRight(), q);
}

void postOrder(Node* node, queue<Node*> &q) {
    if (node == nullptr) {
        return;
    }
    postOrder(node->getLeft(), q);
    postOrder(node->getRight(), q);
    q.push(node);
}

// Recursive Method - Uses preorder traversal so that lower nodes will be rotated before upper ones
Node* balance(Node* node) {
    Node* newRoot = nullptr;
    //left
    if (node->getLeft()) {
        node->setLeft(balance(node->getLeft()));
    }
    //right
    if (node->getRight()) {
        node->setRight(balance(node->getRight()));
    }
    //root
    if (node->getBalance() > 1) {
        if (node->getLeft()->getBalance() > 0) {
            newRoot = rotateRight(node);
        }
        else {
            newRoot = rotateLeftRight(node);
        }
    }
    else if (node->getBalance() < -1) {
        if (node->getRight()->getBalance() < 0) {
            newRoot = rotateLeft(node);
        }
        else {
            newRoot = rotateRightLeft(node);
        }
    }
    if (newRoot == nullptr) {
        return node;
    }
    else {
        return newRoot;
    }
}

class AVLTree {
    Node* root;
    string country;
    int size;
public:
    // Default and sole constructor; essentially does nothing (except creating a Tree object)
    AVLTree(string _country) {
        root = nullptr;
        country = _country;
        size = 0;
    }

    // Balance tree simply calls recursive balance method on root Node object
    void balanceTree() {
        root = balance(root);
    }

    // inOrderSuccessor uses inOrder queue to get Node immediately after inOrder (used for removal)
    Node* inOrderSuccessor(Node* node) {
        queue<Node*> q;
        inOrder(root, q);
        while(q.front()!=node) {
            q.pop();
        }
        q.pop();
        return q.front();
    }

    // Insertion method, uses binary traversal and converts string to ID to find appropriate location
    void insert(int date, string country, float rate) {
        if (!root) {
            root = new Node(date, country, rate);
            balanceTree();
            return;
        }
        else {
            Node* current = root;
            while(current) {
                if (*current == date) {
                    cout << "unsuccessful" << endl;
                    return;
                }
                else if (*current > date) {
                    if (current->getLeft()) {
                        current = current->getLeft();
                    }
                    else {
                        current->setLeft(new Node(date, country, rate));
                        balanceTree();
                        size++;
                        return;
                    }
                }
                else if (*current < date) {
                    if (current->getRight()) {
                        current = current->getRight();
                    }
                    else {
                        current->setRight(new Node(date, country, rate));
                        balanceTree();
                        size++;
                        return;
                    }
                }
            }
        }
        cout << "unsuccessful" << endl;
    }

    // Binary traversal by ID to find name
    void search(int date) {
        Node* current = root;

        while(current != nullptr) {
            if (*current==date) {
                cout << current->getDate() << endl;
                return;
            }
            else if (*current > date) {
                current = current->getLeft();
            }
            else
                current = current->getRight();
        }
        cout << "unsuccessful" << endl;
    }

    // Linear traversal using preOrder queue (although order is irrelevant) to find multiple copies of name
    void search(float rate) {
        queue<Node*> q;
        preOrder(root, q);
        bool found = false;
        while(!q.empty()) {
            if (q.front()->getRate() == rate) {
                cout << q.front()->getRate() << endl;
                found = true;
            }
            q.pop();
        }
        if(!found) {
            cout << "unsuccessful" << endl;
        }
    }

    // Printing in different order methods uses queues (generated in helper methods)
    // to create correct order, then uses one "print" function regardless of order
    void printInOrder() {
        queue<Node*> q;
        inOrder(root, q);
        print(q);
    }

    void printPreOrder() {
        queue<Node*> q;
        preOrder(root, q);
        print(q);
    }

    void printPostOrder() {
        queue<Node*> q;
        postOrder(root, q);
        print(q);
    }

    // levelCount is same as height of root, which is public Node method
    // (0 is default if tree is empty)
    void printLevelCount() {
        if (root) {
            cout << height(root) << endl;
        }
        else {
            cout << 0 << endl;
        }
    }

    // Simple print method iterates through queues created by one of three
    // standard traversal methods and prints with proper formatting
    void print(queue<Node*> q) {
        while(!q.empty()) {
            cout << q.front()->getDate() << ": " << q.front()->getRate();
            q.pop();
            if (!q.empty()) {
                cout << endl;
            }
        }
        cout << endl;
    }

    Node* getRoot() {
        return root;
    }
    string getCountry() {
        return country;
    }

    int getSize() {
        return size;
    }
};