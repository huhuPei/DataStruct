#include <iostream>
#include <cstdlib>

using namespace std;

template <typename K, typename V>
struct Node {
    K key;
    V value;
    int level;
    Node<K, V>** forward;

    Node() = default;
    Node(K k, V v, int l) key(k), value(v), level(l), forward(nullptr) {}
    ~Node() {
        if (forward != nullptr) delete[] forward;
    }
};

template <typename K, typename V>
class SkipList {
public:
    SkipList() size_(0), level_(0) { Initialize(); }
    ~SkipList() { Free(); }
    bool Insert(K key, V value);
    bool Remove(K key);
    int Find(K key);
private:
    int RandomLevel();
    void CreateNode(int level, Node<K, V>*& node);
    void CreateNode(int level, K key, V value, Node<K, V>*& node);
    void Initialize();
    void Free();
private:
    static const int kMaxLevel_ = 32;
    size_t size_;
    int level_;
    Node<K, V>* head_;
    Node<K, V>* tail_;
};

template <typename K, typename V>
void SkipList<K, V>::Initialize(){

}

template <typename K, typename V>
void SkipList<K, V>::CreateNode(int level, Node<K, V>& node){

}
