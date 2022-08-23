Java 中 HashMap 的使用

+ 注意事项
+ curd
    + create
        + put(key, value)
    + update
        + replace(key, newValue)
    + retrieve
        + get(key)
        + containsKey(key)
        + containsValue(value)
        + keySet(): 获取 key 的集合
    + delete
        + remove(key): 删除某个键值对
        + clear(): 删除所有键值对
+ 遍历
    + foreach 结构
+ HashMap 的底层原理
  + 数据少时，数组+链表，数据多时，数组+红黑树
  + 容量永远为2的次方，初始容量为16
  + 加载因子：对应扩容的阈值， capacity * Load_Factor
  + 添加元素
    + hash的计算方式：异或`(h = key.hashCode()) ^ (h >>> 16)`
    + 插入：直接放入、扔到树里、连表尾部

```java
import java.util.HashMap;

public class test {
    public static void main(String[] args) {
        HashMap<Integer, String> idToName = new HashMap<>();

        // 新增数据
        idToName.put(1, "John");
        idToName.put(2, "Mary");
        idToName.put(3, "Mike");

        // 遍历输出
        printIntToStrMap(idToName);

        // 更新
        idToName.replace(1, "Tom");

        // 查找数据
        System.out.println(idToName.get(1));

        // 删除数据
        idToName.remove(1);

        printIntToStrMap(idToName);
    }

    public static void printIntToStrMap(HashMap<Integer, String> idToName) {
        for (int id : idToName.keySet()) {
            System.out.println(id + ": " + idToName.get(id));
        }
    }
}
```

# 参考资料

1. [HashMap 的底层原理](https://juejin.cn/post/6844904127693078536)