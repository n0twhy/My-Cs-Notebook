[二分查找](../../project/database2022fall/src/storage/page/b_plus_tree_internal_page.cpp)
lower_bound(first, last, value)
找一个位置it，使得[first, it)严格在value左侧，也就是第一个满足*it >= value的地方
cmp(it. value) 语义上就是it是否在value前面。是就是true。lower返回第一个cmp为false的，也就是it < value 为false(which means it >= value)
upper_bound(first, last, value)
找一个位置it，使得*it > value，cmp是返回第一个cmp为true的。cmp(value, it)，第一个 value < it为true的,也就是第一个严格大于value的it

[友元](../../project/database2022fall/src/storage/page/b_plus_tree_leaf_page.cpp)
cpp相同类的不同对象为友元，可以互相调用其私有成员

注意哈  leaf的array尾巴就是size - 1，而internal的尾巴就是size