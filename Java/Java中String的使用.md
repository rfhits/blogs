Java 中 String 类的使用

+ 注意事项
+ 创建 String 对象
+ 常用接口
    + charAt(index): 
    + 转整数
+ String 对象的判等问题

# 注意事项

+ String 不是 Java 的基本数据类型，是被封装好的对象
+ 编译时能确定的 String 被存放到常量池，“能被确定的”指的是字面量或者final修饰的变量
+ 直接 new 出来的 String 会指向堆里，但是同时也会检查常量池中是否已经存在，不在的话就会放一个到常量池里
+ String 是不可变对象，原因可以看[沉默王二的回答 - 知乎](https://www.zhihu.com/question/20618891/answer/1285622839)，考虑了缓存、安全性、线程等因素
+ 如果想要频繁地改变一个字符串，可以使用 StringBuilder 或 StringBuffer 类

关于String 对象的判等问题，可以参考[一文秒懂String对象的那些事](https://juejin.cn/post/6907580432485711886)

```java
import java.math.BigInteger;

public class test {
    public static void main(String[] args) {
        String he = "he"; // he 变量编译时无法确定
        String llo = "llo"; // llo 变量编译时无法确定
        String name0 = he + llo; // name0 编译时无法确定，+ 的内部逻辑是 new StringBuilder 然后 append
        String name1 = "hello"; // 在常量池中
        String name2 = "hello"; // 直接引用常量池中已经创建的对象
        String name3 = new String("hello"); // 在堆中创建，但是会瞅一眼常量池看看有没有 hello
        


        // 测试几个 name 是否指向相同
        System.out.println("name0 == name1? " + (name0 == name1)); // false
        System.out.println("name1 == name2? " + (name1 == name2)); // true
        System.out.println("name1 == name3? " + (name1 == name3)); // false

        System.out.println(name1.charAt(0));

        // 转整数
        String number = "123";
        int i=Integer.parseInt(number);
        System.out.println(i);

        BigInteger big = new BigInteger(number);
        System.out.println(big);
    }  
}
```