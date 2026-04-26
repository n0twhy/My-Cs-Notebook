## elevator pitch
实现了proj1的三大核心组件：
`extendible_hashmap`
`lruk_replacer`
`bufferpoolmanager`

## 1.整体架构与职责划分

数据路径
`DiskManager <-> BufferPoolManager <-> Page(frame) <-> 上层执行器`

BPM内部三件套：
`page_table` : `page_id -> frame` 方便定位
`free_list`:从未使用过的frame
`replacer`:LRUK

### 关键变量
`pin_count`
`replacer.size()`
`page_id` <-> `frame_id`

## Extendible Hash Table

### 核心机制：
- 目录索引: Hash(key) & (1 << global_size - 1)
- 桶满触发分裂，如果bucket_size == global_size，global_size也要double

###  为什么使用这种结构
- 桶满时做**局部扩容**，比全局 rehash 更可控

### tradeoff 
- 优点：扩容优雅，平滑，查询均摊
- 实现复杂 

## LRUK Replacer

### 算法语义
- 仅仅从evictable里选择挑中victim来进行evict
- 访问次数小于l的k-distance视为inf

### 数据结构
- cold_list
- hot_list

### why LRUK
- 避免缓存污染