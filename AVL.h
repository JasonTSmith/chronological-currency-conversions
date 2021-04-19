#include <iostream>
#include <queue>
using namespace std;

// Declare Node and height early, as the two each reference each other
class Node;
int height(Node* node);

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
    // Constructor that should be the only one used for inserting dates
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
    float getRate() {
        return rate;
    }
    Node* getLeft() {
        return left;
    }
    Node* getRight() {
        return right;
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
    node->setRight(rotateRight(node->getRight()));
    return rotateLeft(node);
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
void preOrder(Node* node, queue<Node*> &q) {
    if (node==nullptr) {
        return;
    }
    q.push(node);
    preOrder(node->getLeft(), q);
    preOrder(node->getRight(), q);
}
void inOrder(Node* node, queue<Node*> &q) {
    if (node==nullptr) {
        return;
    }
    inOrder(node->getLeft(), q);
    q.push(node);
    inOrder(node->getRight(), q);
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

string formatDate(int date) {
    string toReturn = "";
    toReturn += to_string(date % 100);
    date /= 100;
    toReturn = to_string(date % 100) + '/' + toReturn;
    date /= 100;
    toReturn += '/' + to_string(date);
    return toReturn;
}

class AVLTree {
    Node* root;
    string country;
    int size;
public:
    // Sole constructor
    AVLTree(string _country) {
        root = nullptr;
        country = _country;
        size = 0;
    }

    // Balance tree simply calls recursive balance method on root Node object
    void balanceTree() {
        root = balance(root);
    }

    // Insertion method, uses binary traversal and uses date to find appropriate location
    void insert(int date, string country, float rate) {
        if (!root) {
            root = new Node(date, country, rate);
            size++;
            return;
        }
        else {
            Node* current = root;
            int level = 0;
            while(current) {
                if (*current == date) {
                    cout << "unsuccessful" << endl;
                    return;
                }
                else if (*current > date) {
                    if (current->getLeft()) {
                        current = current->getLeft();
                        level++;
                    }
                    else {
                        current->setLeft(new Node(date, country, rate));
                        if (log(size)/log(2) < level) {
                            balanceTree();
                        }
                        size++;
                        return;
                    }
                }
                else if (*current < date) {
                    if (current->getRight()) {
                        current = current->getRight();
                        level++;
                    }
                    else {
                        current->setRight(new Node(date, country, rate));
                        if (log(size)/log(2) < level) {
                            balanceTree();
                        }
                        size++;
                        return;
                    }
                }
            }
        }
        cout << "unsuccessful" << endl;
    }

    // Binary traversal by date to find rate
    float search(int date) {
        Node* current = root;

        while(current != nullptr) {
            if (*current==date) {
                return current->getRate();
            }
            else if (*current > date) {
                if (current->getLeft() != nullptr) {
                    current = current->getLeft();
                }
                else {
                    return -1 * current->getDate();
                }
            }
            else {
                if (current->getRight() != nullptr) {
                    current = current->getRight();
                }
                else {
                    return -1 * current->getDate();
                }
            }
        }
    }

    // Linear traversal using preOrder queue to find minimum conversion rate
    Node* mostValRate() {
        queue<Node*> q;
        preOrder(root, q);
        Node* mVR = q.front();
        q.pop();
        while(!q.empty()) {
            if (q.front()->getRate() < mVR->getRate()) {
                mVR = q.front();
            }
            q.pop();
        }
        return mVR;
    }

    // Linear traversal using preOrder queue to find maximum conversion rate
    Node* leastValRate() {
        queue<Node*> q;
        preOrder(root, q);
        Node* mVR = q.front();
        q.pop();
        while(!q.empty()) {
            if (q.front()->getRate() > mVR->getRate()) {
                mVR = q.front();
            }
            q.pop();
        }
        return mVR;
    }

    string getCountry() {
        return country;
    }

    // Search BST for date, then use rate to multiply
    float convertFromUSD(int date, float amount) {
        float rate = search(date);
        if (rate < -1) {
            cout << "Data for " << formatDate(date) << " does not exist in " << country <<
                 ", but " << formatDate((-1 * rate)) << " does." << endl;
            rate = search(-1 * rate);
        }
        return (amount * rate);
    }

    float convertToUSD(int date, float amount) {
        float rate = search(date);
        if (rate < -1) {
            cout << "Data for " << formatDate(date) << " does not exist in " << country <<
                 ", but " << formatDate((-1 * rate)) << " does." << endl;
            rate = search(-1 * rate);
        }
        return amount / rate;
    }
};
