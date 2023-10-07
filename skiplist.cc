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
    Node(K k, V v, int l) key(k), value(v), level(l) {
        forward = new Node<K, V>* [l];
        for (int i = 0; i < l; ++i) forward[i] = nullptr; 
    }
    ~Node() {
        if (forward != nullptr) delete[] forward;
    }
};

//跳表参数列表
namespace {
    const int kMaxLevel = 32;
    const int kSkipNum = 2;
}

template <typename K, typename V>
class SkipList {
public:
    SkipList() size_(0), level_(0) { Initialize(); }
    ~SkipList() { Free(); }
    Node<K, V>* Find(K key);
    bool Insert(K key, V value);
    bool Remove(K key);
    void SetRandom(unsigned int seed) { srand(seed); }
    size_t size() const { return size_; }
private:
    int RandomLevel();
    void Initialize();
    void Free();
private:
    size_t size_;
    int level_;
    Node<K, V>* head_;
};

template <typename K, typename V>
void SkipList<K, V>::Initialize(){
    srand(time(0));
    K k, V v;
    head_ = new Node<K, V>(k, v, kMaxLevel);
}

template <typename K, typename V>
Node<K, V>* SkipList<K, V>::Find(K key) {
    Node<K, V>* cur = head_;
    for (int i = level_ - 1; i >= 0; --i) {
        while (cur->forward[i] && cur->forward[i]->key < key) {
            cur = cur->forward[i];
        }
    }
    cur = cur->forward[0];
    if (cur && cur->key == key) return cur;
    return nullptr; 
}

template <typename K, typename V>
bool SkipList<K, V>::Insert(K key, V value) {
    Node<K, V>* update[kMaxLevel];
    Node<K, V>* cur = head_;
    for (int i = level_ - 1; i >= 0; --i) {
        while (cur->forward[i] && cur->forward[i]->key < key) {
            cur = cur->forward[i];
        }
        update[i] = cur;
    }
    cur = cur->forward[0];
    if (cur && cur->key == key) return false;
    int node_level = RandomLevel();
    //更新跳表的层数
    while (node_level > level_) {
        update[level_++] = head_;
    }
    Node<K, V>* new_node = new Node<K, V>(k ,v, node_level);
    for (int i = node_level - 1; i >= 0; --i) {
        new_node->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = new_node;
    }
    ++size_;
    return true;
}

template <typename K, typename V>
bool SkipList<K, V>::Remove(K key) {
    Node<K, V>* update[kMaxLevel];
    Node<K, V>* cur = head_;
    for (int i = level_ - 1; i >= 0; --i) {
        while (cur->forward[i] && cur->forward[i]->key < key) {
            cur = cur->forward[i];
        }
        update[i] = cur;
    }
    cur = cur->forward[0];
    if (cur == nullptr || cur->key != key) return false;
    for (int i = 0; i < level_; ++i) {
        if (update[i]->forward[i] != cur) break;
        update[i]->forward[i] = cur->forward[i];
    }
    while (level_ > 1 && head_->forward[level_ - 1] == nullptr) --level_;
    
    delete cur;
    --size_;
    return true;
}

template <typename K, typename V>
/*
随机生成 1~MAX_LEVEL 之间的数，例如，当kSkipNum = 2：
    1/2 的概率返回 1
    1/4 的概率返回 2
    1/8 的概率返回 3 以此类推
*/
int SkipList<K, V>::RandomLevel() {
    int level = 1;
    // 当 level < MAX_LEVEL，且能被kSkipNum整除，level + 1，也就是每kSkipNum个数添加一级索引
    while (rand() % kSkipNum == 0 && level < kMaxLevel)
        ++level;
    return level;
}