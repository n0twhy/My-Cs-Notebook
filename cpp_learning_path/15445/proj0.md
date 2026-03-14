[返回unique_ptr*](../../project/database2022fall/src/include/primer/p0_trie.h#L127)
注意这里是返回指向unique_ptr的指针
为什么不写成&children_[key_char]?
因为不安全，如果前面move失败 operator就会自动生成一个来返回，会出错

unique_ptr<TrieNode>* p;
p是指向这个盒子的指针
*p是盒子本身（TrieNode&）
 **p是两次解引用，就是对象(TrieNode)本身

[emplace](../../project/database2022fall/src/include/primer/p0_trie.h#L135)
unordered_map也是可以emplace的，他会在容器内原地构造这个对象
push_back则是要经历一次拷贝，再移动

[dynamic_cast](../../project/database2022fall/src/include/primer/p0_trie.h#462)
如果不成功  会返回nullptr，这里需要检查一下

[shared_lock](../../project/database2022fall/src/include/common/rwlatch.h)
.lock():拿到写锁
.lock_shared()拿到读锁
另一种类:unique_lock是自动析构的