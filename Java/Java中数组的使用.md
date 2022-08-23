+ 注意事项
+ 实例
    + 数组声明的写法
    + 以二维数组为例解析数组创建

# 注意事项

+ 数组是对象，具体内容需要在堆上创建，其引用是在栈上的。
+ 在堆上创建需要指定数组的大小
+ 所以定义数组，必须指定第一维的大小

[指定列_为什么Java二维数组不用指定列的长度](http://t.csdn.cn/ohvEv)

# 实例

```java
public class test {
    public static void main(String[] args) {
        int[] onedArray = new int[3];
        onedArray[0] = 1;
        onedArray[1] = 2;
        onedArray[2] = 3;
        int[][] twodArray = new int[3][3]; // 传统的定义数组的方法
        int[][] twodArray2 = new int[3][]; // 堆上多了三个引用，三个指向一维数组的引用
        twodArray2[0] = onedArray; // 为堆上第一个引用赋值，指向onedArray
        twodArray2[1] = onedArray;
        twodArray2[2] = onedArray;

        for (int i = 0; i < 3; i++) {
            printOnedArray(twodArray2[i]);
            System.out.println("");
        }

        int[][][] threedArray = new int[3][][];
        int[][][] threedArray2 = new int[3][3][];
    }

    public static void printOnedArray(int[] onedArray) {
        for (int i = 0; i < onedArray.length; i++) {
            System.out.print(onedArray[i]);
            System.out.print(" ");
        }
    }
}
```