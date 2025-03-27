#include <bits/stdc++.h>
using namespace std;

// Node for Treap
struct TreapNode {
    int key, priority;
    TreapNode *left, *right;
    
    TreapNode(int k) : key(k), priority(rand()), left(nullptr), right(nullptr) {}
};

// Treap implementation
class Treap {
private:
    TreapNode* root;

    TreapNode* rotateRight(TreapNode* y) {
        TreapNode* x = y->left;
        y->left = x->right;
        x->right = y;
        return x;
    }

    TreapNode* rotateLeft(TreapNode* x) {
        TreapNode* y = x->right;
        x->right = y->left;
        y->left = x;
        return y;
    }

    TreapNode* insert(TreapNode* root, int key) {
        if (!root) return new TreapNode(key);
        if (key < root->key) {
            root->left = insert(root->left, key);
            if (root->left->priority > root->priority)
                root = rotateRight(root);
        } else {
            root->right = insert(root->right, key);
            if (root->right->priority > root->priority)
                root = rotateLeft(root);
        }
        return root;
    }

    TreapNode* remove(TreapNode* root, int key) {
        if (!root) return nullptr;
        if (key < root->key)
            root->left = remove(root->left, key);
        else if (key > root->key)
            root->right = remove(root->right, key);
        else {
            if (!root->left) return root->right;
            if (!root->right) return root->left;
            if (root->left->priority > root->right->priority) {
                root = rotateRight(root);
                root->right = remove(root->right, key);
            } else {
                root = rotateLeft(root);
                root->left = remove(root->left, key);
            }
        }
        return root;
    }

public:
    Treap() : root(nullptr) {}

    void insert(int key) {
        root = insert(root, key);
    }

    void remove(int key) {
        root = remove(root, key);
    }
};

// Node for doubly linked list
struct ListNode {
    int key, value;
    ListNode* prev;
    ListNode* next;

    ListNode(int k, int v) : key(k), value(v), prev(nullptr), next(nullptr) {}
};

// LRU Cache Implementation
class LRUCache {
private:
    int capacity;
    unordered_map<int, ListNode*> cache;
    Treap treap;
    ListNode* head;
    ListNode* tail;

    void moveToHead(ListNode* node) {
        if (node == head) return;
        removeNode(node);
        addNode(node);
    }

    void addNode(ListNode* node) {
        node->next = head;
        node->prev = nullptr;
        if (head) head->prev = node;
        head = node;
        if (!tail) tail = node;
        treap.insert(node->key);
    }

    void removeNode(ListNode* node) {
        if (node->prev) node->prev->next = node->next;
        if (node->next) node->next->prev = node->prev;
        if (node == tail) tail = node->prev;
        if (node == head) head = node->next;
        treap.remove(node->key);
    }

public:
    LRUCache(int cap) : capacity(cap), head(nullptr), tail(nullptr) {}

    int get(int key) {
        if (cache.find(key) == cache.end()) return -1;
        ListNode* node = cache[key];
        moveToHead(node);
        return node->value;
    }

    void put(int key, int value) {
        if (cache.find(key) != cache.end()) {
            ListNode* node = cache[key];
            node->value = value;
            moveToHead(node);
            return;
        }

        ListNode* newNode = new ListNode(key, value);
        cache[key] = newNode;
        addNode(newNode);

        if (cache.size() > capacity) {
            cache.erase(tail->key);
            removeNode(tail);
            delete tail;
        }
    }

    void displayCache() {
        ListNode* current = head;
        cout << "\nCurrent LRU Cache: \n";
        while (current) {
            cout << "[Key: " << current->key << ", Value: " << current->value << "] ";
            current = current->next;
        }
        cout << endl;
    }
};

// Menu-driven program
int main() {
    int capacity;
    cout << "Enter LRU Cache Capacity: ";
    cin >> capacity;

    LRUCache lru(capacity);

    while (true) {
        cout << "\nMENU\n";
        cout << "1. Put (Insert key-value pair)\n";
        cout << "2. Get (Retrieve value by key)\n";
        cout << "3. Display Cache\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";

        int choice, key, value;
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter key: ";
                cin >> key;
                cout << "Enter value: ";
                cin >> value;
                lru.put(key, value);
                cout << "Inserted successfully!\n";
                break;
            case 2:
                cout << "Enter key to retrieve: ";
                cin >> key;
                value = lru.get(key);
                if (value != -1)
                    cout << "Value: " << value << endl;
                else
                    cout << "Key not found!\n";
                break;
            case 3:
                lru.displayCache();
                break;
            case 4:
                cout << "Exiting program.\n";
                return 0;
            default:
                cout << "Invalid choice! Try again.\n";
        }
    }
}

