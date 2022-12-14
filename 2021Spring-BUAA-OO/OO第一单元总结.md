- [第一次作业](#第一次作业)

  - [架构介绍](#架构介绍)
  - [度量分析](#度量分析)
  - [重要方法](#重要方法)
  - [测评](#测评)
  - [得分](#得分)
- [第二次作业](#第二次作业)

  - [架构介绍](#架构介绍-1)
  - [度量分析](#度量分析-1)
  - [重构](#重构)
  - [测评](#测评-1)
- [第三次作业](#第三次作业)

  - [主要工作](#主要工作)
  - [重构](#重构-1)
  - [测评](#测评-2)
  - [得分](#得分-1)
- [心得体会](#心得体会)

  - [继承的使用](#继承的使用)
  - [工厂模式](#工厂模式)
  - [先想后做](#先想后做)
  - [遗憾](#遗憾)
- [RefandThanks](#refandthanks)

**keywords: 2021 北航 OO 第一单元 总结 BUAA 面向对象 Unit1**

## 第一次作业

第一次作业看起来较为简单，对幂函数的多项式进行求导即可。因为考虑到这次工程对的后续影响，所以在欣赏了很多学长的博客后，决定尽量按照第三次作业的要求来。

### 架构介绍

首先介绍第一次的架构，先引入几个单词：Poly、Term和Factor。

输入是多项式，叫做Poly。

Term表示项，Poly是项的和，数学上表示为$Poly = T_1 + T_2 + ... +T_n$

Factor表示因子，Term是因子的乘积，数学上表示为$Term = F_1 * F_2 *...* F_n$

Factor是一个较为抽象的概念，像常数、幂函数、三角函数，都可以称为Factor。

知道了数学上的关系后，就可以动手将其实现。

这一次作业输入的因子只有常数（Constant）和幂函数（Power）。建立常数类（Const）和幂函数类（Pow），继承自因子类。

Poly封装了一个`ArrayList`，里面存储各个Term，可以表示为`Poly =[T1, T2, ..., Tn]`。

Term封装了一个`ArrayList`，里面存储各个Factor，可以表示为`Term = [F1, F2, ..., Fn]`。

类图如下：

![](https://img2020.cnblogs.com/blog/1930568/202103/1930568-20210327101059807-759097006.jpg)

除此之外，还有一些小的"工具类"，如下：

![](https://img2020.cnblogs.com/blog/1930568/202103/1930568-20210327101149357-1502020495.jpg)

使用工厂简化创建对象的逻辑，使用Regex存储各种Factor的表达式，为后续作业打好基础，Reduction类用于化简。

### 度量分析

首先，架构清晰，符合人类的思考方式。不过在代码实现上，有所不足。

![](https://img2020.cnblogs.com/blog/1930568/202103/1930568-20210327101236786-1041853007.jpg)

可以看到，挺多红的。其中，equals方法泛红无可避免，毕竟自动生成，有挺多`if-return`分支，但是，其他红色说明了设计存在不合理的地方。

1. Poly的构造方法：将输入用加减号分割为Term，再新建Term，添加到数组中。其中，判断分割的条件较为冗杂，并且还做了对连续加减号的处理，导致构造方法复杂度过高。
2. `addTerm`和`addFactor`方法：因为做了一些优化，会对要add的Term和Factor进行特判，导致代码冗长，耦合度变高。

### 重要方法

#### 分割

分别利用加减号和乘号将Poly和Term分割为Term和Factor，这是构建整个数据结构的基础，因为判断逻辑较多，所以可读性、可维护性较差，导致条件一旦变化，就要重写。

#### 求导和输出

这也是本次作业的核心函数。Poly求导会向下调用Term的求导，Term又会向下调用各种Factor的求导方法，这样实现了层层向下，"各做各事"。

有一个冲突是常数类求导返回常数，幂函数求导返回Term，为了统一Term的接受参数，直接令常数类求导后也返回Term，这样在使用上方便了一些，但是对后来的作业而言，并不是一个好的设计。

### 测评

#### 自己的bug

在强测和互测中均未发现自己的bug。

#### 互测

采取"瞎猫碰死耗子"策略。应作业要求，特别指出该方法**未结合被测程序的代码设计结构**。

只找到了一个bug（惭愧惭愧），构造样例的思路是从简单到复杂，从特殊到普通。

先是一些普通的常数，然后加上幂函数，最后根据加减号的复杂要求来几个，比如连续的减号等。

找到了别人的一个bug。如输入`-2*x+1`，该同学程序输出`-2+`，可以判断该同学是想在输出0时优化一下，结果没考虑到0在最后的情况。

### 得分

在强测中只获得了92分，原因是大量的代码都在为后续工程铺垫，没有投入过多的精力在优化上。

## 第二次作业

### 架构介绍

第一次作业到第二次作业，难度有质的飞跃。Java的三大特性，继承、多态和分装，也表现地淋漓尽致。

以下是关于构建各个类时的考量：

首先，添加cos和sin类，考虑到第三次作业中，会出现如cos((x+1))的例子，所以，cos类的成员变量，有个Poly。

Term类中，可能出现如$1+(x+1) \times 5$的情况，这使得原本简单的情况一下复杂了起来。$(x+1)$可以理解为一个Poly，因为其是各个Term的和，$(x+1) \times 5$可以理解为一个Term，因为其中有乘号。这说明Term的成员变量可以是Poly，这和作业一中，Poly维护了一个`ArrayList<Term>`组合起来，形成了一种嵌套的结构。

最终的解决方法，就是令Poly和Term都继承自Factor，自此，完成了一种递归。Poly、Term和Factor类，可以相互包含。

直接生成所有类图，会引起读者的不适，所以选取部分类，作图如下：

![](https://img2020.cnblogs.com/blog/1930568/202103/1930568-20210327101321766-181892971.jpg)

Term类在这里起到了一个"承上启下"的作用，还要实现一些优化，所以方法多、代码长。

### 度量分析

![](https://img2020.cnblogs.com/blog/1930568/202103/1930568-20210327101416824-193666050.jpg)

`equals`方法放在一边不提😕，毕竟自动生成的。Poly构造方法因为逻辑复杂，所以不出意外地全红了。

接下来就是几个add方法，首当其冲的就是`addFactor`，此方法要将$Term \times FactorAdd = F_1 \times F_2 \times ... \times F_n \times FactorAdd$尽可能地化简，所以写了很多if，而且没写else，这是导致其见红的第一个原因。

第二个原因就是根据要添加Factor的type，分流到不同的add里，做有针对性的优化：

```{.java}
if (factorAdd instanceof Const) {
    this.addConst((Const) factorAdd);
} else if (factorAdd instanceof Pow) {
    this.addPow((Pow) factorAdd);
} else if (factorAdd instanceof Cos) {
    this.addCos((Cos) factorAdd);
} else if (factorAdd instanceof Sin) {
    this.addSin((Sin) factorAdd);
} else if (factorAdd instanceof Poly) {
    this.addPoly((Poly) factorAdd);
} else if (factorAdd instanceof Term) {
    this.addTerm((Term) factorAdd);
} else {
   System.out.println("factorAdd is an un-defined type");
}
```

以上两个原因导致了可维护性降低和耦合度的升高。

### 重构

> 通过**类图和/或者度量数据**来对比重构前和重构后的程序结构，并进行说明
>
> ————作业要求

#### 求导

在第一次作业中，强行让Factor的求导都变成Term，在这次作业中已经不再适用。因为在架构中，都继承了Factor，所以就统一了求导后的类型。Const、Cos、Power类，求导完都可以用一个Factor来接收。再添加到`ArrayList`时，做有针对性的优化。

#### 添加因子的优化

如果Term无脑add任意Factor，这次作业可以比较轻松地完成，而且bug也可以少很多，但是性能分会受到损失，所以，多少得做点优化。

这回优化的思路比较清晰。

首先，要判断Term本身的性质：

1. Term含ConstZero，此时，任何添加Factor的行为都被直接return
2. Term含ConstOne，此时，添加Factor时，可以清除ConstOne，再添加

接下来，添加Factor，判断Factor的具体类型，再做添加：

1. Factor是一个Const，若是0，则清空Term，否则，找到Term中的Const，求改其值为积
2. Factor是Power，可以合并已有的Power
3. ......

比较下复杂度：

![](https://img2020.cnblogs.com/blog/1930568/202103/1930568-20210327101457018-80862231.jpg)

因为允许括号的嵌套，Poly构造方法的复杂度上升，耦合度也上升，不利于维护；

Term的`addFactor`复杂度上升明显，调用关系变得更加复杂，原因就是因为优化带来的特判和分流。

在代码上,`addTerm`的实现两次作业其实一样，怀疑因为其调用了`Term.equals`方法，后者复杂度的下降优化了该方法。

### 测评

#### 自己的bug

强测未发现bug，在互测中被发现两个bug。

第一个bug在优化类，方法是对Term清零。在优化$0 \times 0$时，调用了作业一遗留下来的一个优化方法，它会直接清空整个Term，一个元素都不剩，导致输出报错。这是当时考虑不全导致的。解决方法**非常简单**，直接把作业一优化的代码删了。因为在作业二中，优化已经被下放到了添加Factor和合并Term里了。原来的代码也没存在的必要了。

第二个bug是Term类的`equals`方法。$Term + Term'$，如果$Term = Term'$，给Term乘个2，策略是判断两个Term中的Factor能否互相包含。

为什么会出现这个bug，这就要提到**第一次上机**了。上机时，看到多项式的equals方法采用了相互`contains`的策略，我在这一次作业中依旧保留，导致`[cos(x), cos(x), x]`和`[cos(x), x]`中的元素相互包含，会认为这两个Term相等。

究其原因，一是是没有从数学的角度深入思考这个问题，二是在迭代开发中，没有考虑旧方法的适用性。

解决方法：clone要被判断的Term，先看`FactorList`的元素个数，然后遍历这个List，`remove`相同的元素，如果clone的Term最后空了，说明相等。
可以从数学角度证明，这里就不赘述。

> 分析**出现了bug的方法**和**未出现bug的方法**在**代码行**和**圈复杂度**上的差异。
>
> ————作业要求

对第二个bug进行分析：

首先，代码行数肯定是变多了，不过圈复杂度在修复之后下降了。究其原因，是找到了判断`equals`的一个**充要条件**，从性能上更加优秀，毕竟少用了一个for循环。

#### 别人的bug

较于第一次作业，这种方法已经不管用，所以没有找到别人的bug。

顺便看了下别人被hack的样例：

输入`-cos(x)*x+sin(x)`，输出`x*sin(x)0*cos(x)`，
输入`-x**-1`，输出`-1*-x**-2`

可以发现两个问题：优化易出错，以及，有些同学的代码，在迭代开发后，连原来的样例都过不了（没错我在说自己）。

## 第三次作业

架构和作业二大致相同。如果能保证正确且无空格（即"正常人"的输入），那么我凭借第二次的代码可通实验，然而，生活总是那么艰难。

Factor的类图就不放了，和第二次作业一样，这里展示一些比较小的类：

![](https://img2020.cnblogs.com/blog/1930568/202103/1930568-20210327101525662-1317249206.jpg)

### 主要工作

这次主要做了两件事，一个是指导书要求的检测Wrong Format，另一个是继续优化。

#### Wrong Format

分成两个维度：宏观和微观。

添加`Check`类，检查非法空格和非法字符，是一个宏观的角度；新建各种因子的时候，判断是否符合正则表达式和指数，是一个微观的角度。

除此以外，添加`Exception`类，发现Wrong Format时，直接调用`Exception.quit()`。这样比较灵活，也符合类的思想。我知道有些同学会把`println("WF")`和`exit(0)`放到很多类里面，导致代码冗长，干脆直接包装成方法，未来还能根据不同的的错误类型报不同的错。

#### 继续优化

说是继续优化，其实也是小修小补。因为发现在第二次作业中，输出还含着`0*sth`的情况，分析是没有优化Term乘零的情况，说明第一次优化还是考虑不周，没有仔细地进行罗列。

### 重构

第二次作业就是针对第三次作业来的，几乎没重构。就是构造方法加上`if-else`，在MetricsReload的分析中，只有addFactor因为补上了优化而导致各个指标加一，其他都没有较大的变化。我相信对大部分同学，这次作业不需要很大的改动。

### 测评

> - 分析未通过的公测用例和被互测发现的bug：特征、问题所在的类和方法
> - 对比分析**出现了bug的方法**和**未出现bug的方法**在**代码行**和**圈复杂度**上的差异
>
> ————作业要求

#### 自己的bug

强测未出现bug，但是在互测时被揪出一个bug，是`Check`类在判断括号内的非法空格时出错。

具体原因是因为sin括号里不可以`+++1*sth`，但是Poly括号里可以`+++1*sth`，开始以为括号里都一样。解决方法就是新增一行正则，把这个区别开。

代码行数增加一行，复杂度没变。

#### 别人的bug

放弃hack别人

分析下我们这个屋子的Hack情况，仅一位同学求导错了，剩下全是Wrong Format。

### 得分

很蟾愧，只做了一点微小的优化，但在强测中获得了97.8分，距离满分只有3分不到，这已经大大超出个人预期。当然，这和强测的样例较难优化也有关系。

不过这剩下的三分要"卷"出来，还是要费一番精力，甚至可能造成超时、优化出错等问题。再加上个人课业繁重，所以放弃参加内卷。决定把能做到的先做好，"跳一跳"还是交给有能力的同学。

想要获得满分，可以考虑的优化包括但不限于：

1. 合并同类项
2. $sin^2$和$cos^2$的合并
3. `x*x`和`x**2`的取舍
4. 带括号展开和不带括号的取舍

## 心得体会

### 继承的使用

学到了继承的使用。虽然我认为本次作业实现为接口更好，因为体现的更是方法的复用性而不是数据的共用性。在Term的addFactor方法，是本次作业的~~精髓~~。面对各个类的输入，将其进行分流处理，思路很清晰。

本来在作业一想做一个`Reduct(Poly)`的方法，一劳永逸地解决优化问题，后来发现想多了————这样很难，并且需要层层向下触发，不如在构建Poly和Term时来得快。

### 工厂模式

这个在寒假有所涉猎，简化了思考和调用的逻辑。

### 先想后做

很大的一个收获就是先想再做。

比如很多同学从作业一到作业二进行了**大规模**的重构，这在我身上并没有发生。

再比如第二次作业考虑了第三次作业，减轻了一点负担。

### 遗憾

在高手云集的计算机学院，每次互测我都抱着至少被hack50次的心。这回作业的一大遗憾就是没有写测评机，导致在互测环节参与感较低。

另一个遗憾就是第一次得分较低，现在想来，在第一作业中，把合并同类项的代码实现不难，甚至在后续作业中可以逐渐改进。

## RefandThanks

[IDEA圈复杂度插件（MetricsReload）下载与使用](https://blog.csdn.net/Dkangel/article/details/106279052)

感谢所有为这门课投入精力的老师、助教和学长。