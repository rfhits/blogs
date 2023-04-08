Introduction to PyTorch 阅读笔记

Datetime: 2023-03-24T20:45+08:00

Categories: Python | MachineLearning

兜兜转转还是来到了 DL 的大门前

写了一半，结果不知道为什么都没了，明明 vscode autosave 是开的，OneDrive 也是自动上传，啊啊啊，我要鲨了你 Microsoft。。。

来自[Introduction to PyTorch](https://pytorch.org/tutorials/beginner/basics/intro.html) 的阅读笔记

[toc]

# tensor

-   tensor 初始化
-   tensor and NumPy ndarray
-   tensor's device: GPU or CPU

文档，包括 dtype 等：https://pytorch.org/docs/stable/tensors.html

tensor 是对训练参数的抽象，我觉得 3B1B 的视频很好，neuron 和 weight 还有 bias 都是 tensor

tensor 不只是数据，因为在训练 nn 的过程中，neuron 和 weight 都有梯度，tensor 也要一起保存

我知道可以直接使用 `tensor(ndarray)` 创建 tensor，但是，如果要创造到 GPU 上，还需要`tensor(ndarray).to(device)`，就会创建两遍。以下可以指定创建位置为 device，可以使用 `torch.tensor?` 查看构造方法。不知道这样指定 device 会不会在 CPU 上再创建一遍。

```python
 th.tensor(data=d, dtype=th.float32, device=device)
```

# dataset and loader

-   数据格式：tensor，使用 transform 和 target_transform
-   数据格式：shape：`[n_samples * n_feats]`

之所以提供 dataset 和 dataloader，是要把原始数据和训练过程中的迭代解耦。

dataloader 提供了 batch 的抽象

https://pytorch.org/tutorials/beginner/basics/data_tutorial.html

## 内置数据集

使用它们提供数据集的时候，有 transform 和 target_transform

```python
import torch
from torchvision import datasets
from torchvision.transforms import ToTensor, Lambda

ds = datasets.FashionMNIST(
    root="data",
    train=True,
    download=True,
    transform=ToTensor(),
    target_transform=Lambda(lambda y: torch.zeros(10, dtype=torch.float).scatter_(0, torch.tensor(y), value=1))
)
```

transform 是把 feats 格式为 PIL 或者 numpy 换成 tensor，target_transform 施加于 labels，上面的代码是 one-hot 编码。

## 自定义数据集

我一开始在自定义数据集应该返回什么样的 shape 上犯了难，实际上这个地方其实无伤大雅

dataset 要返回什么样的格式，取决于 model，或者说自己的设计。

torch 默认的行为是这样的，如果一个 dataset `get_item` 返回的 X 的 shape 为 s，

那么在 loader 中迭代的时候，一个 batch 的 shape 就是 `batch_size * s`，下面是我毕设的样例，stu 表示学生的特征向量，是一个 127 个 scalar 构成的 vector

```python
stu, y = train_dataset[0]

stu.shape
# torch.Size([127])

X, y = next(iter(train_dataloader))
X.shape
# torch.Size([64, 127])
```

我之所以有 shape 的疑问，是因为教程里 dataset 里一个 X 的 shape 是 `1 * 28 * 28`，然后把 `64 * 1* 28 * 28` 的数据喂到模型里了。

模型中的 flatten 解开了我的疑惑，该函数直接把数据压缩到二维。

可以自己加载官方数据集和 loader 查看

```python
import torch
from torch import nn
from torch.utils.data import DataLoader
from torchvision import datasets
from torchvision.transforms import ToTensor

training_data = datasets.FashionMNIST(
    root="data",
    train=True,
    download=True,
    transform=ToTensor()
)
```

从 dataset 中直接取数据来训练

```python
img, label = training_data[0]

print(img.shape)
# torch.Size([1, 28, 28])

f = nn.Flatten()
f_img = f(img)
print(f_img.shape)
# torch.Size([1, 784])

feats, labs = next(iter(train_dataloader))
feats.shape
ff = f(feats)
ff.shape
# torch.Size([64, 784])
```

这种看起来人尽皆知的东西，对新手来说是如此的难以理解。

# build NN

实现 forward() 方法即可

## flatten

flatten 会默认压扁成二维矩阵

## Linear and ReLU

## Softmax

# autograd

-   computational graph
-   accumulate the grad so we need to reset
-   detach the grad in pred tasks
-   Jacobian products

https://pytorch.org/tutorials/beginner/basics/autogradqs_tutorial.html#automatic-differentiation-with-torch-autograd

自动求导机制

可以给 tensor 的属性加上 requires_grad=True

tensor 可以有 grad_fn，是一个 callable object，看来 tensor 的属性很多

我建议先对 back propagation 有一个理解，才能看这部分。如果不知道，就看 3B1B 的教程。

对在 bp（back propagation）的过程中，需要存储每个 tensor 的梯度

从数学上来说，autograd 的基础是构建了一个 computational graph，比如：

```python
z = torch.matmul(x, w)+b
```

或者：

```python
out = (inp+1).pow(2).t()
```

输入的一点点扰动就可以改变输入，所以才有

$\frac{\partial z}{\partial x}$ 和 $\frac{\partial out}{\partial inp}$

# train your model

-   对训练过程的抽象：模型，损失函数与算法
-   model(param): arg 就是 forward 的 param
-   optimizer

这一部分和 sklearn 的接口不一样，因为训练是有 batch 和 epoch 的概念

所以相当于在每个 batch 里面去调整模型的参数，那就是可以把一个 batch 类比成 sklearn 模型

但是也绝对没有一行 model.fit() 那么简单

```
for each epoch:
    for each batch:
        back propagation then get the gradient
        params take a step
```

首先要看 PyTorch 对 nn 的架构

-   dataloader：提供迭代和 batch 的功能
-   model：自定义的 nn
-   loss_fn: 损失函数，机器学习的三大件之一（模型、损失函数与算法）
-   Hyperparameter：超参数
-   optimizer：对 model 参数的控制器，机器学习三大件之梯度下降算法的抽象

what makes a difference 就是这个 optimizer，把参数都交给它去控制

```python
# Initialize the loss function
loss_fn = nn.CrossEntropyLoss()

# optimizer
optimizer = torch.optim.SGD(model.parameters(), lr=learning_rate)

def train_loop(dataloader, model, loss_fn, optimizer):
    for batch, (X, y) in enumerate(dataloader):
        # Compute prediction and loss
        pred = model(X)
        loss = loss_fn(pred, y)

        # Backpropagation
        optimizer.zero_grad()
        loss.backward()
        optimizer.step()

# ... doesn't matter at all

epochs = 10
for t in range(epochs):
    print(f"Epoch {t+1}\n-------------------------------")
    train_loop(train_dataloader, model, loss_fn, optimizer)
    test_loop(test_dataloader, model, loss_fn)
print("Done!")
```

# save and load the model

https://pytorch.org/tutorials/beginner/basics/saveloadrun_tutorial.html

-   save and load model weights
-   save and load model

## weights

> model's weights stored in an state dictionary

要从 weights 复原一个 model，就要创建一个 model 实例，再把 weights 送进去

```python
model = models.vgg16(pretrained=True) # pretrained means use default model weights，「预训练」

# store weights
torch.save(model.state_dict(), 'model_weights.pth')

# load weights
model = models.vgg16() # we do not specify pretrained=True, i.e. do not load default weights
model.load_state_dict(torch.load('model_weights.pth'))
model.eval()
```

> be sure to call `model.eval()` method before inferencing to set the dropout and batch normalization layers to evaluation mode. Failing to do this will yield inconsistent inference results.

## model

```py
torch.save(model, 'model.pth')
model = torch.load('model.pth')
```

使用的是 pickle 这个 python package

# misc

how to reset weights in nn?

没有好的方法，只能一层一层地 reset，最快的方法就是直接 new 一个 model。

https://discuss.pytorch.org/t/how-to-re-set-alll-parameters-in-a-network/20819/10

**RuntimeError**: mat1 and mat2 must have the same dtype

我遇到的问题应该是格式不统一，比如使用 `.float()` 方法，或者在创建 tensor 的时候指定 dtype。
