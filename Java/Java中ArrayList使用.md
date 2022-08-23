ArrayList的使用

+ 注意事项
+ new一个ArrayList
+ curd
    + add(object)
    + add(index, object)
    + set(index, object)
    + get(index)
    + remove(index)
+ 循环
    + `size()`获得数组的大小
    + 迭代器
    + 在循环中删除
+ ArrayList与数组的相互转换
+ ArrayList 的内部实现
  + 数据结构：动态数组
  + 修改次数：modCount
  + add: 容量不够，就通过new 数组扩容，扩容，调用底层的方法进行copy。
    + 当插入元素时，copy 发生了两次，一次是原封不动的copy，一次是数组内部前面把后面的覆盖性的copy
  + remove: copy 后置最后一个元素为 null，防止内存泄漏
  + clear: 全部置null
  + 线程不安全


# 注意事项

+ 使用ArrayList，要`import java.util.ArrayList`
+ 使用`get(index)`来获取元素，而不是使用`arraylist[index]`来获取元素
+ 获取最后一个元素不能用`get(-1)`，就用`get(arraylist.size() -1)`
+ 获取长度用`arraylist.size()`，不是`arraylist.length`


# 使用样例

```java
import java.util.ArrayList;
import java.util.Iterator;

public class test {
    public static void main(String[] args) {
        System.out.println("Hello World");
        ArrayList<String> strings = new ArrayList<String>();
        strings.add("Hello"); // creates a new element in the list
        strings.add("World");

        strings.set(0, "Hell"); // update the element at index 0

        System.out.println(strings.get(0)); // use "get()" to get the element at index 0

        strings.remove(0); // remove the element at index 0

        for (String s : strings) {
            System.out.println(s);
        }

        Iterator<String> it = strings.iterator();
        while (it.hasNext()) {
            String s = it.next(); // 指向下一个对象
            if (s.equals("Hell")) {
                it.remove(); // 移除当前对象，再指向下一个对象
            }
        }
        System.out.println("----");

        for (String s : strings) {
            System.out.println(s);
        }
    }
}

```

# 相互转换

[Java中ArrayList、Integer[]和int[]的相互转换](https://blog.csdn.net/k909397116/article/details/107749620)

# 内部实现

1. [ArrayList内部实现](https://blog.csdn.net/dingji_ping/article/details/51014914)
2. [System.arraycopy()方法详解-jdk1.8](https://blog.csdn.net/balsamspear/article/details/85069207)