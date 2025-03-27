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

    // Rotate right
    TreapNode* rotateRight(TreapNode* y) {
        TreapNode* x = y->left;
        y->left = x->right;
        x->right = y;
        return x;
    }

    // Rotate left
    TreapNode* rotateLeft(TreapNode* x) {
        TreapNode* y = x->right;
        x->right = y->left;
        y->left = x;
        return y;
    }

    // Insert into treap
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

    // Delete from treap
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
    unordered_map<int, ListNode*> cache;  // HashMap: key -> Node
    Treap treap;                          // Treap for ordered access
    ListNode* head;                        // Most recently used
    ListNode* tail;                        // Least recently used

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
};

// Main function to test LRU Cache
int main() {
    LRUCache lru(3);
    
    lru.put(1, 10);
    lru.put(2, 20);
    lru.put(3, 30);
    cout << lru.get(1) << endl; // Output: 10

    lru.put(4, 40);  // Evicts 2 (LRU)
    cout << lru.get(2) << endl; // Output: -1 (not found)
    
    lru.put(5, 50);  // Evicts 3 (LRU)
    cout << lru.get(3) << endl; // Output: -1 (not found)

    cout << lru.get(4) << endl; // Output: 40
    cout << lru.get(5) << endl; // Output: 50

    return 0;
}

