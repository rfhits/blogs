Java中队列的使用

+ 注意事项
+ curd
    + create
        + add(obj): 增加一个元素，如果队列已满，则抛出异常
        + offer(obj): 添加一个元素并返回true，如果队列已满，则返回false
    + update
        + set(index, obj)
    + retrieve
        + element(): 返回队列头部的元素，如果队列为空，则抛出一个NoSuchElementException异常
        + peek(): 返回队列头部的元素，如果队列为空，则返回null
    + delete
        + remove(): 移除并返回队列头部的元素，如果队列为空，则抛出一个NoSuchElementException异常
        + poll(): 移除并返问队列头部的元素，如果队列为空，则返回null
+ 样例
+ 内部实现
  + 内部类 Node
  + 双向链表，有头尾指针
  + 查找的时候会根据 index 决定用 head 往后还是用 last 往前

# 注意事项

+ `java.util.Queue`是一个抽象类，不能实例化
+ `java.util.LinkedList`实现了`Queue`，可以当作队列来使用

# 样例

```java
import java.util.*;

/*
 * public class TreeNode {
 *   int val = 0;
 *   TreeNode left = null;
 *   TreeNode right = null;
 * }
 */

public class Solution {
    /**
     * 
     * @param root TreeNode类 
     * @return int整型ArrayList<ArrayList<>>
     */
    public ArrayList<ArrayList<Integer>> levelOrder (TreeNode root) {
        // write code here
        ArrayList<ArrayList<Integer>> res = new ArrayList<>();
        if (root == null) return res;
        LinkedList<TreeNode> nodes = new LinkedList<>();
        nodes.add(root);
        while (!nodes.isEmpty()) {
            int n = nodes.size();
            ArrayList<Integer> layer = new ArrayList<>(n);
            for (int i = 0; i < n; i++) {
                TreeNode head = nodes.poll();
                
                layer.add(head.val);
                if (head.left != null) nodes.add(head.left);
                if (head.right != null) nodes.add(head.right);
                
            }
            res.add(layer);
        }
        return res;
        
    }
}

```

# 内部实现

[LinkedList 底层实现](https://blog.csdn.net/sky1373516909/article/details/89156995)

# 参考

[java中queue的使用](https://www.cnblogs.com/end/archive/2012/10/25/2738493.html)