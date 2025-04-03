#include <bits/stdc++.h>
using namespace std;

struct TreapNode {
    int seqKey;     // Sequence number (BST key)
    int cacheKey;   // User's key
    int priority;
    int size;
    shared_ptr<TreapNode> left, right;
    TreapNode(int s, int k) : seqKey(s), cacheKey(k), priority(rand()), size(1), left(nullptr), right(nullptr) {}
};

int getSize(shared_ptr<TreapNode> node) {
    return node ? node->size : 0;
}

void updateSize(shared_ptr<TreapNode> node) {
    if (node) node->size = getSize(node->left) + getSize(node->right) + 1;
}

class Treap {
private:
    shared_ptr<TreapNode> root;

    shared_ptr<TreapNode> rotateRight(shared_ptr<TreapNode> y) {
        auto x = y->left;
        y->left = x->right;
        x->right = y;
        updateSize(y);
        updateSize(x);
        return x;
    }

    shared_ptr<TreapNode> rotateLeft(shared_ptr<TreapNode> x) {
        auto y = x->right;
        x->right = y->left;
        y->left = x;
        updateSize(x);
        updateSize(y);
        return y;
    }

    shared_ptr<TreapNode> insert(shared_ptr<TreapNode> root, int seqKey, int cacheKey) {
        if (!root) return make_shared<TreapNode>(seqKey, cacheKey);
        if (seqKey < root->seqKey) {
            root->left = insert(root->left, seqKey, cacheKey);
            if (root->left->priority > root->priority)
                root = rotateRight(root);
        } else {
            root->right = insert(root->right, seqKey, cacheKey);
            if (root->right->priority > root->priority)
                root = rotateLeft(root);
        }
        updateSize(root);
        return root;
    }

    shared_ptr<TreapNode> remove(shared_ptr<TreapNode> root, int seqKey) {
        if (!root) return nullptr;
        if (seqKey < root->seqKey) {
            root->left = remove(root->left, seqKey);
        } else if (seqKey > root->seqKey) {
            root->right = remove(root->right, seqKey);
        } else {
            if (!root->left) return root->right;
            if (!root->right) return root->left;
            if (root->left->priority > root->right->priority) {
                root = rotateRight(root);
                root->right = remove(root->right, seqKey);
            } else {
                root = rotateLeft(root);
                root->left = remove(root->left, seqKey);
            }
        }
        updateSize(root);
        return root;
    }

    int findKthHelper(shared_ptr<TreapNode> root, int k) {
        if (!root) return -1;
        int leftSize = getSize(root->left);
        if (k < leftSize) return findKthHelper(root->left, k);
        else if (k == leftSize) return root->cacheKey;
        else return findKthHelper(root->right, k - leftSize - 1);
    }

public:
    Treap() : root(nullptr) {}

    void insert(int seqKey, int cacheKey) {
        root = insert(root, seqKey, cacheKey);
    }

    void remove(int seqKey) {
        root = remove(root, seqKey);
    }

    int findKth(int k) {
        return findKthHelper(root, k);
    }

    void clear() {
        root = nullptr;
    }
};

struct ListNode {
    int key, value;
    shared_ptr<ListNode> prev, next;
    ListNode(int k, int v) : key(k), value(v), prev(nullptr), next(nullptr) {}
};

class LRUCache {
private:
    int capacity;
    int access_counter;
    unordered_map<int, pair<shared_ptr<ListNode>, int>> cache; // Maps user key to (ListNode, currentSeqKey)
    Treap treap;
    shared_ptr<ListNode> head, tail;

    void moveToHead(shared_ptr<ListNode> node) {
        if (node == head) return;
        removeNode(node);
        addNode(node);
    }

    void addNode(shared_ptr<ListNode> node) {
        node->next = head;
        node->prev = nullptr;
        if (head) head->prev = node;
        head = node;
        if (!tail) tail = node;
    }

    void removeNode(shared_ptr<ListNode> node) {
        if (node->prev) node->prev->next = node->next;
        if (node->next) node->next->prev = node->prev;
        if (node == tail) tail = node->prev;
        if (node == head) head = node->next;
        node->prev = node->next = nullptr;
    }

public:
    LRUCache(int cap) : capacity(cap), access_counter(0), head(nullptr), tail(nullptr) {}

    void resize(int newCapacity) {
        capacity = newCapacity;
    }

    int get(int key) {
        if (cache.find(key) == cache.end()) return -1;
        auto& entry = cache[key];
        auto node = entry.first;
        int oldSeqKey = entry.second;
        treap.remove(oldSeqKey);
        access_counter++;
        int newSeqKey = access_counter;
        treap.insert(newSeqKey, key);
        entry.second = newSeqKey;
        moveToHead(node);
        return node->value;
    }

    void put(int key, int value) {
        if (cache.find(key) != cache.end()) {
            auto& entry = cache[key];
            auto node = entry.first;
            int oldSeqKey = entry.second;
            treap.remove(oldSeqKey);
            access_counter++;
            int newSeqKey = access_counter;
            treap.insert(newSeqKey, key);
            entry.second = newSeqKey;
            node->value = value;
            moveToHead(node);
            return;
        }
        auto newNode = make_shared<ListNode>(key, value);
        access_counter++;
        int newSeqKey = access_counter;
        treap.insert(newSeqKey, key);
        cache[key] = {newNode, newSeqKey};
        addNode(newNode);
        if ((int)cache.size() > capacity) {
            int tailSeqKey = cache[tail->key].second;
            treap.remove(tailSeqKey);
            cache.erase(tail->key);
            removeNode(tail);
        }
    }

    void displayCache() {
        auto current = head;
        while (current) {
            cout << "[Key: " << current->key << ", Value: " << current->value << "] ";
            current = current->next;
        }
        cout << endl;
    }

    void clear() {
        head = nullptr;
        tail = nullptr;
        cache.clear();
        treap.clear();
        access_counter = 0;
    }

    int findKth(int k) {
        int cacheKey = treap.findKth(k - 1);
        if (cacheKey == -1 || cache.find(cacheKey) == cache.end()) return -1;
        return cache[cacheKey].first->value;
    }
};

int main() {
    int capacity, choice, key, value, k;
    cout << "Enter LRU Cache Capacity: ";
    cin >> capacity;
    LRUCache cache(capacity);

    while (true) {
        cout << "\nMENU:\n";
        cout << "1. Put\n2. Get\n3. Display Cache\n4. Clear Cache\n5. Resize Cache\n6. Find Kth element\n7. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter key and value: ";
                cin >> key >> value;
                cache.put(key, value);
                break;
            case 2:
                cout << "Enter key: ";
                cin >> key;
                cout << "Value: " << cache.get(key) << endl;
                break;
            case 3:
                cache.displayCache();
                break;
            case 4:
                cache.clear();
                cout << "Cache cleared." << endl;
                break;
            case 5:
                cout << "Enter new capacity: ";
                cin >> capacity;
                cache.resize(capacity);
                break;
            case 6:
                cout << "Enter k: ";
                cin >> k;
                cout << "Kth element: " << cache.findKth(k) << endl;
                break;
            case 7:
                exit(0);
            default:
                cout << "Invalid choice. Try again." << endl;
        }
    }
    return 0;
}
