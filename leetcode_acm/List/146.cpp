/*
 * 力扣 146 - LRU 缓存（LRU Cache）（ACM 题面）
 * ----------------------------------------
 *
 * 【题目描述】
 * 请你设计并实现一个满足「最近最少使用」（LRU）约束的数据结构。
 * - 实现 LRU 缓存：容量为 capacity，get(key) 与 put(key, value) 均需在平均 O(1) 时间内完成。
 * - get(key)：若 key 存在则返回对应 value，并将该键值对视为「最近使用」；否则返回 -1。
 * - put(key, value)：若 key 已存在则更新 value 并视为「最近使用」；否则插入 (key, value)。
 *   若插入后键数量超过 capacity，则淘汰「最久未使用」的键值对后再插入。
 *
 * 【输入格式】
 * - 第一行：一个正整数 capacity，表示缓存容量。
 * - 第二行：一个正整数 m，表示操作次数。
 * - 接下来 m 行：每行一个操作。
 *   - "G key"：表示 get(key)，key 为整数。
 *   - "P key value"：表示 put(key, value)，key、value 均为整数。
 *
 * 【输出格式】
 * - 对每次 get 操作，输出一行一个整数：key 存在则输出其 value，否则输出 -1。
 * - put 操作不产生任何输出。
 *
 * 【样例输入】
 * 2
 * 9
 * P 1 1
 * P 2 2
 * G 1
 * P 3 3
 * G 2
 * P 4 4
 * G 1
 * G 3
 * G 4
 *
 * 【样例输出】
 * 1
 * -1
 * -1
 * 3
 * 4
 *
 * 【样例说明】
 * 容量为 2。put(1,1) put(2,2) 后缓存为 {(1,1),(2,2)}；get(1) 返回 1 且 1 变为最近使用。
 * put(3,3) 时容量已满，淘汰最久未使用的 2，缓存变为 {(1,1),(3,3)}；get(2) 返回 -1。
 * put(4,4) 淘汰 1，缓存变为 {(3,3),(4,4)}；get(1) 返回 -1；get(3) 返回 3；get(4) 返回 4。
 *
 * 【数据范围】
 * 1 <= capacity <= 3000
 * 0 <= key <= 10^4
 * 0 <= value <= 10^5
 * 1 <= m <= 10^5
 */

 #include <iostream>
 #include <unordered_map>
using namespace std;
class LRU {
public:
struct Node {
    int key;
    int val;
    Node* next;
    Node* prev;
    Node() : key(0), val(0), next(nullptr), prev(nullptr) {}
    Node(int k, int v) : key(k), val(v), next(nullptr), prev(nullptr) {}
};
    LRU(int _capacity) : size(0), capacity(_capacity) {
        head = new Node;
        tail = new Node;
        head->next = tail;
        tail->prev = head;
    }
    void addToHead(Node* node) {
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }
    Node* removeTail() {
        Node* node = tail->prev;
        delNode(node);
        return node;
    }
    void delNode(Node* node) {
        node->next->prev = node->prev;
        node->prev->next = node->next;
    }
    void put(int _key, int _value) {
        if(cache.count(_key)) {
            cache[_key]->val = _value;
            delNode(cache[_key]);
            addToHead(cache[_key]);
        } else {
            Node* node = new Node(_key, _value);
            addToHead(node);
            size++;
            cache[_key] = node;
        }
        if(size > capacity) {
            Node* rm = removeTail();
            size--;
            cache.erase(rm->key);
            delete rm;
        }
    }
    
    int get(int _key) {
        if(cache.count(_key)) {
            delNode(cache[_key]);
            addToHead(cache[_key]);
            return cache[_key]->val;
        } else {
            return -1;
        }
    }
private:
    int size;
    int capacity;
    Node* head;
    Node* tail; 
    unordered_map<int, Node*> cache;
};

int main() {
    int n;
    cin >> n;
    LRU* lru = new LRU(n);
    int c;
    cin >> c;
    for(int i = 0; i < c; ++i) {
        char op;
        cin >> op;
        if(op == 'G') {
            int k;
            cin >> k;
            cout << lru->get(k) << endl;
        }
        if(op == 'P') {
            int k, v;
            cin >> k >> v;
            lru->put(k, v);
        }
    }
    
}