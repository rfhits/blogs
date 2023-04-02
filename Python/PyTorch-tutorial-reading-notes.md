Introduction to PyTorch 阅读笔记

Datetime: 2023-03-24T20:45+08:00

Categories: Python | MachineLearning

兜兜转转还是来到了 DL 的大门前

记录的目的不是为了示众，而是为了提醒未来的自己。

写了一半，结果不知道为什么都没了，明明 vscode autosave 是开的，OneDrive 也是自动上传，啊啊啊，我要鲨了你 Microsoft。。。

来自[Introduction to PyTorch](https://pytorch.org/tutorials/beginner/basics/intro.html) 的阅读笔记

[toc]

# tensor

-   tensor 初始化
-   tensor and NumPy ndarray
-   tensor's device: GPU or CPU

tensor 是对训练参数的抽象，我觉得 3B1B 的视频很好，neuron 和 weight 还有 bias 都是 tensor

tensor 不只是数据，因为在训练 nn 的过程中，neuron 和 weight 都有梯度，tensor 也要一起保存

# dataset and loader

提供 batch 的抽象

# build NN

实现 `forward()` 方法即可

## use gpu for training

如果要在 gpu 上训练模型，一定一定不要忘记，把模型移动到 GPU 上

因为模型本身也是有参数的，这些参数也算得上是「数据」了

```python
device = "cuda" if torch.cuda.is_available() else "cpu"
print(f"Using {device} device")

# define modle, doesn't matter
class NeuralNetwork(nn.Module):
    def __init__(self):
        super().__init__()
        self.flatten = nn.Flatten()
        self.linear_relu_stack = nn.Sequential(
            nn.Linear(28*28, 512),
            nn.ReLU(),
            nn.Linear(512, 512),
            nn.ReLU(),
            nn.Linear(512, 10),
        )

    def forward(self, x):
        x = self.flatten(x)
        logits = self.linear_relu_stack(x)
        return logits

model = NeuralNetwork().to(device)
print(model)
```

## flatten

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

# optimize params

-   对训练过程的抽象：模型，损失函数与算法
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
