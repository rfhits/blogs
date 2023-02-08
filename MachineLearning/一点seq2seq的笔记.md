一点 seq2seq 的笔记

Date: 2023-02-08T21:33:00+08:00

毕设，看了点神经网络的内容。

RNN 的输入是 a sequence of vector，感觉 RNN 像在训练一个 cell，喂给 cell 一个 vector，然后它输出一个 hidden vector，把这个 hidden vector 和 next vector in sequence 拼接在一起，再作为 cell 的输入。至于 sequence 里的第一个 vector 和什么拼接在一起，我想可能是随机的，也可能是 $\vec{0}$。

这样，每次输入 vector 都会得到一个输出 h，这个 h 就可以去全连接做真正的输出了，比如判定单词的属性：动词、名词啥的。

还可以只保留 sequence 中最后一个 vector 的输出 h，这个 h 可以认为保留了整个 sequence 的信息，可以全连接做别的输出，比如判断一个句子里的感情是积极、消极或者中性。

RNN 的阅读资料可以参考知乎上的这个回答：[能否简单易懂的介绍一下 RNN（循环神经网络）？ - Peace 的回答](https://www.zhihu.com/question/68552209/answer/329359882)

LSTM 是 RNN 的变体，为 RNN 增加了遗忘机制。公式我肯定是记不下来的，目的是为了解决 RNN 梯度消失和梯度爆炸的问题。

LSTM 可以参考这一篇英文资料：[Understanding LSTM Networks](http://colah.github.io/posts/2015-08-Understanding-LSTMs/)。

我只能记下来每次 cell 要应对的输入有：

1. 上次 cell 的状态。当然，本次 cell 也要输出自己的状态
2. 上次 cell 的隐藏层输出，就是 h(idden)。当然本次 cell 也要输出 hidden
3. 当前这个 step 接受的 vector

门这个东西，是由 $h_{t-1}$ 和 $x_{t}$ 决定的，过一次全连接层再激活一下

cell state 需要过 forget gate 遗忘一下，让后加上 input gate 记录现在的东西

hidden 由 $h_{t-1}$、$x_{t}$ 和 cell state 决定。

下面就是 Encoder-Decoder 的内容，可以参考这一篇知乎文章：[完全图解 RNN、RNN 变体、Seq2Seq、Attention 机制](https://zhuanlan.zhihu.com/p/28054589)，它甚至贴心地总结了 attention。

Encoder-Decoder 目的是为了解决输入和输出长度不一致的问题。

输入一个 sequence vector，得到一个 vector，这个 vector 包含了整个 sequence 的信息，我们叫它 $\vec{c}$，感觉有点像是 compressed，把输入的 sequence 信息“压缩”成了一个向量，这个步骤就是 Encoder，最后需要根据 $\vec{c}$ Decoder 出新的信息。

怎么 Decoder，就是再接入一个类似 RNN 的结构，这个 RNN 的 初始 $h_0$ 就是 Encoder 得到的大向量，接受的 sequence of vector 可以是 cell 上一步的输出，也可以不要，也可以是每个 step 都用 $\vec{c}$。

最后是 attention，看知乎罢。

至于更加详细的网络和运行过程，可以参考 Bilibili 视频：[【深度学习】seq2seq 模型 / Encoder-Decoder 模型及数学原理讲解](https://www.bilibili.com/video/BV1d4411R72S/)
