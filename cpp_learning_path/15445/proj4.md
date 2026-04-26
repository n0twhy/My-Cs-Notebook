LockTable的语义就是如果申请的锁合法，必须拿到锁，如果拿不到锁，就一直等

## Abort
- LOCK_ON_SHRINKING:此时在shrink阶段，不允许加锁(如果是RC，可以加读锁，为了极致的高并发性能)
- UPGRADE_CONFLICT:此时已经有等待升级的锁
- LOCK_SHARED_ON_READ_UNCOMMITTED:RU不允许加读锁
- TABLE_LOCK_NOT_PRESENT:如果在对某一个tuple进行加锁时，却没有对应表的锁
- ATTEMPTED_INTENTION_LOCK_ON_ROW:对行加表级锁
- TABLE_UNLOCKED_BEFORE_UNLOCKING_ROWS:表级锁解锁时，表内还有行级锁
- INCOMPATIBLE_UPGRADE:升级锁不符合规则
- ATTEMPTED_UNLOCK_BUT_NO_LOCK_HELD:没有对应的锁却要解锁

lockmode
第一道防线：shrink（RC特殊处理）检查和RU状态不准读
第二道防线：拿到队列锁，看自己是不是升级，升级o不ok
进入队列：等待唤醒

注意这个request队列里，不需要map，因为同一个表的等待lock事务一般很低

等待唤醒用lambda函数
注意这个wait，传参有队列latch，如果睡眠就释放锁，醒来自动获得锁，内部while(predicate)然后继续wait

更新要检查是否冲突

最后要记录lockset

时刻注意，除了更新要删除原来的节点，其他时候删除记得唤醒其他线程，让他们起来看看是不是可以进来了

找图：我们的逻辑是，waitfor图，不关心事务之间到底有几条边，仅仅关心yes or no。而且我们从txn_id升序遍历，set是一个完美的数据结构，改成这个

记录路径,用栈，标记1的时候，入栈，标记为2的时候，出栈，当查到邻居节点是1，这时候就是一个完整环了

has一次找一个环，返回最年轻的事务，run函数循环调用has，abort，删除边

为了性能，拿到map的时候，复制hashmap做模糊快照，因为死锁快照时候存在，以后也是一定存在的，它具有永久性

要删除的事务，为了唤醒他我需要对应的lockrequestqueue
注意一个语义，一个事务可能同时hold多个lockrequest，但是他只能在一个事务里等待（等待才会进入死锁环），所以在waiter里存map就行