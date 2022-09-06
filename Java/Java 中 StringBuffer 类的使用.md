Java 中 StringBuffer 类的使用

+ 注意事项
+ 初始化
+ curd
  + append(int number), append(String str): 追加
  + insert(index, str): 在指定的位置插入
  + setCharAt(int index, char ch): 改变指定位置的字符
  + charAt(int index)
  + deleteCharAt(int index): 删除指定位置的字符
  + delete(int start, int end): 删除指定区间的字符，从 start 到 end-1 都被删除
+ 常用
  + reverse(): 自己反转，并返回自己
  + toString(): 变化为字符串
+ StringBuffer 为什么是线程安全的？
+ 参考资料
  
# 注意事项

+ 使用 `StringBuffer` 不需要导入任何包
+ StringBuffer 线程安全， StringBuilder 线程不安全

# 样例

```java
public class test {
    public static void main(String[] args) {
        StringBuffer sb = new StringBuffer(); // 在 jdk13 中，默认是16个长度的byte[]，不存放任何东西的
        sb.append("world"); // "world"
        sb.insert(0, "hello "); // "hello world"
        System.out.println(sb.toString());
        sb.delete(4, sb.length()); // "hell"
        System.out.println(sb.toString());
        sb.reverse(); // "lleh"
        System.out.println(sb.toString());
    }
}
```

# 参考资料

1. [StringBuffer的用法](https://www.cnblogs.com/liu-chao-feng/p/5636063.html)
2. [为什么StringBuilder是线程不安全的？StringBuffer是线程安全的？](https://zhuanlan.zhihu.com/p/83297925)