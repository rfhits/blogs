PriorityQueue

+ 注意事项
+ new 一个 PriorityQueue
+ curd
    + offer(obj), add(obj)
    + element(), peek()：查询堆顶的元素，peek很形象，中文是“窥视”的意思，表示只看而不移除
    + remove(), poll()：优先队列自带的函数，剔除堆顶元素
    + remove(obj)，继承自 Collection 的函数，精确移除



# 注意事项

+ 使用PriorityQueue时，需要`import Java.util.PriorityQueue`
+ 使用优先队列，常常需要我们自定义比较函数

```java
import java.util.Comparator;
import java.util.PriorityQueue;

public class test {
    public static void main(String[] args) {
        // 创建了一个comparator
        Comparator<Integer> cmp = new Comparator<Integer>() {
            @Override
            public int compare(Integer o1, Integer o2) {
                return o2 - o1;
            }
        };

        // 提供了优先队列的比较方法和大小，大小可以缺省
        PriorityQueue<Integer> maxHeap = new PriorityQueue<Integer>(5, cmp);

        for (int i = 0; i < 6; i++) {
            maxHeap.offer(i);
        }
        System.out.println("maxHeap: " + maxHeap);

        System.out.println("first of maxHeap: " + maxHeap.peek());

        System.out.println("try to remove first of maxHeap: " + maxHeap.poll());
        System.out.println("now maxHeap: " + maxHeap);

        System.out.println("use remove(obj) to remove specific object");
        maxHeap.remove(3);
        System.out.println("now maxHeap: " + maxHeap);
    }
}
```