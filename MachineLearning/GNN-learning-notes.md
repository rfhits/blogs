GNN 学习笔记

Datetime: 2023-04-01T16:28+08:00

Categories: MachineLearning

[ToC]

初学者一定要看：[【GNN 入门】综述篇 - 知乎用户 MxLVSX 的文章 - zhihu.com](https://zhuanlan.zhihu.com/p/430518493)，包括频域和空域、任务类型、经典模型。

最早的 GNN，介于迭代不动点：https://zhuanlan.zhihu.com/p/76290138

# GCN

-   GCN 的运行过程
-   GCN 训练的参数
-   GCN 的局限性：Transductive

简单的理解可以参考 [何时能懂你的心——图卷积神经网络（GCN）](https://zhuanlan.zhihu.com/p/71200936)

复杂的理论可以参考：

1. [如何理解 Graph Convolutional Network（GCN）？ - superbrother 的回答 - zhihu.com](https://www.zhihu.com/question/54504471/answer/332657604)
2. [【GNN】万字长文带你入门 GCN - 阿泽的文章 - zhihu.com](https://zhuanlan.zhihu.com/p/120311352)

GCN 核心是那一行公式，我比较关心怎么使用这个东西。。。因为理论实在是过于复杂。

notation：

-   A: 邻接矩阵（Adjacent Matrix），节点如果没有自己指向自己的边（自环），对角线就是 0。一般来说，分子、人与人之间的社交网络，都没有自环的。
-   D: 度矩阵（Degree Matrix），$\tilde{D}_{ii} = \sum{j\tilde{A}_{ij}}$，对角矩阵
-   H：隐藏层，对于输入层，H 就是 X
-   N: 样本个数
-   h: 隐藏层 H 矩阵大小为 N \* h

$$
\tilde{A} = A + I\\
\tilde{D}_{ii} = \sum{j\tilde{A}_{ij}}\\
H^{l+1} = \sigma(\tilde{D}^{-\frac{1}{2}} \tilde{A} \tilde{D}^{-\frac{1}{2}}H^{(l)}W^{(l)})
$$

将 $\tilde{D}^{-\frac{1}{2}} \tilde{A} \tilde{D}^{-\frac{1}{2}}$ 记为 $\hat{A}$

$\hat{A}$ 表示了图本身的结构，一旦图的结构确定，$\hat{A}$ 也就确定。

然后可以在想象一下 GCN 是怎样跑起来的，一个表示节点隐藏特征的 H 矩阵，N \* h，被 $\hat{A}$ 左乘，然后有一个对应的 W 右乘。过一个 ReLU 或者 Softmax，就结束了。

GCN 训练的参数什么是？就是 W

> GCN 网络中就只有一个模型参数 W 需要定义。
>
> —— [怎么搭好一个 GCN？只需这四步 - 眼睛里进砖头了的文章 - 知乎](https://zhuanlan.zhihu.com/p/422380707)

GCN 的官方样例是「半监督」图节点预测。就是说：
在一个图上面，有的节点有 label，有的节点没有，
对 labeled 的节点划分训练集和测试集，对训练集的节点，聚合信息，这个过程中，使用到了 unlabelled 的节点。

下面是一些个人看法：

我觉得 GCN 是在训练每一层的权重矩阵 W，
这个 W 还依赖于图的结构，也就是邻接矩阵 A，因为 W 是跟在 A 的后面的乘起来的，不同的 A，或许 W 的表示不一样。

$W^{(l)}$ 的大小为 $h^{(l)} \times h^{(l+1)}$，在 DGL 等模型中定义一个卷积层，使用的参数估计就是 W 的大小。

所以有说法：[为什么 GCN 是 Transductive 的？](https://www.zhihu.com/question/363234280/answer/2617877951)

# GraphSAGE

[【Graph Neural Network】GraphSAGE: 算法原理，实现和应用](https://zhuanlan.zhihu.com/p/79637787)

1. 通过聚合函数 $agg_k$ 得到邻居节点的隐藏层信息 $h^k_{N(v)}$
2. 拼接 $h^k_{N(v)}$ 和 $h^{(k-1)}_v$，左乘 $W^k$，过一个 $\sigma$，得到当前节点的 $h^k_v$
3. $h_v^k \leftarrow h_v^k/||h_v^k||_2$
4. 重复上述 1、2、3 步

需要挑选好聚合函数 $agg_k$，然后模型就是在训练多个 $W_k$，每一层聚合，都要指定 $W_k$ 的大小。

# Graph Attention Network

-   使用 $W$ embedding
-   multi-head 机制

[向往的 GAT(图注意力网络的原理、实现及计算复杂度)](https://zhuanlan.zhihu.com/p/81350196)

本应该叫做「GAN」，但是「GAN」已经是别的模型的名字，所以叫做「GAT」。

对于一层 GAT，我觉得重点是学习一个 $W$，$Wh_i$ 表示把当前节点特征隐藏状态 $h_i$ embed 到一个其他维度的空间，两个节点映射后的特征 concat 后，是一个 vector，和 $\vec{a}$ 点乘即可得到标量，记为 $e_{ij}$。

$$
e_{i j}=a\left(\left[W h_{i} \| W h_{j}\right]\right), j \in \mathcal{N}_{i}
$$

然后就很稀疏平常，

1. $LeakyReLU(e_{ij})$ 加上 Softmax 求邻居节点对当前节点的权重，
2. 加权求和得到新的 $h_i$

最后使用 multi-head 机制，在一个 layer 里，复制粘贴多个 1、2 步骤，训练多个 $W$，记为 $W^k$，把这些 $h_i^k$ 拼起来，过一个激活函数，做一个最后的 output。

对了，在 DGL 这个机器学习库里，用 GAT 要加自环，千万不要忘记了。

---- 完 ----
