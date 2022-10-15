现代Javascript教程第一部分读书笔记

书籍网址：https://zh.javascript.info/

# ES6

ES6，一种js标准，类似于cpp11这种，目的是规范不同平台下浏览器的行为。

# use strict

参考https://zh.javascript.info/strict-mode#usestrict

`"use strict;"`

举一个例子，较早的浏览器可以直接 `variable = sth`定义变量，但是后来新的标准不允许这么干

如果在 `<script>`开头加上 `"use strict;"`就会开启这种更加严格的特性，原来的代码就会报错

可以理解为一个启用新特性的宏

# 反引号与“刀乐”

参考https://zh.javascript.info/types

> 反引号是 **功能扩展** 引号。它们允许我们通过将变量和表达式包装在 `${…}` 中，来将它们嵌入到字符串中。例如：
>
> ```js
> let name = "John";
>
> // 嵌入一个变量
> alert( `Hello, ${name}!` ); // Hello, John!
>
> // 嵌入一个表达式
> alert( `the result is ${1 + 2}` ); // the result is 3
> ```

# 三种modal

modal译作“模态窗”，有三种和用户交互的函数，分别是 `alert`，`prompt`和 `confirm`。

提示用户，获取用户的输入，用户点击确定或者取消。

# 或运算寻找第一个真值

```js
let firstName = "";
let lastName = "";
let nickName = "SuperCoder";

alert( firstName || lastName || nickName || "Anonymous"); // SuperCoder
```

他不是alert true或者 false，而是alert一个value

# 函数也是值

```js
let sayHello = function () {
    alert("hello")
};
let temp = sayHello
temp() // 调用了这个函数
```

这样，函数可以作为函数的参数

> 函数需要提出 `question`（问题），并根据用户的回答，调用 `yes()` 或 `no()`。
>
> `ask(question, yes, no)`;
>
> 传入两个函数，根据需求调用

# 如何debug

https://zh.javascript.info/debugging-chrome

页面右键，检查，在调试栏的header部分有一个 `source`，类似于vscode，有三个页面，从左到右

文件树，代码浏览，调试信息。

# 对象的属性

https://zh.javascript.info/property-descriptors

- `.`和 `[]`
- 删除对象的属性 `del`
- 关键字 `in`判断某个属性是否在对象内
- 使用 `for let <key> in <obj>`获取对象的属性
  什么时候key能被for获取，见[属性标志](https://zh.javascript.info/property-descriptors)

访问对象属性的方法有两种，分别是用 `.`和 `[]`。

使用 `[]`可以在 `[]`中用变量，所以更加灵活。

```js
let obj1 = {
  name: "Alice"
};

let obj2 = {
  "name": "Alice"
};

let attrib = "name"
console.log(obj1[attrib]) // "Alice"
```

如果像下面这样，把对象作为key，那么会被转换为字符串。

怎样才能把obj作为key呢，后文会提到map。

```js
let obj1 = {
  name: "Alice"
};

let obj2 = {
  "name": "Alice"
};

obj1[obj2] = "test"
console.log(obj1[obj2])
```

# this

https://zh.javascript.info/object-methods#fang-fa-zhong-de-this

this却决于引擎运行时的上下文

> `this` 的值就是在点之前的这个对象，即调用该方法的对象。

# new

https://zh.javascript.info/constructor-new

```js
function User(name, age) {
  this.name = name;
  this.age = age;
}

let u = new User("Alice", 18); // 构造一个对象

console.log(User("Alice", 18))
console.log(new User("Alice", 18))
```

因为这是一种新的语法，需要使用关键字 `new`。

# 对象原始值转换

https://zh.javascript.info/object-toprimitive

当输出一个对象时，发生了什么？

```js
function User(name, age) {
  this.name = name;
  this.age = age;
}

let u = new User("Alice", 18); // 构造一个对象
console.log(u)

// User { name: 'Alice', age: 18 }
```

答案是发生了原始值转换

# 数据类型

## 原始类型的方法

https://zh.javascript.info/primitives-methods

“对象包装器”对于每种原始类型都是不同的，它们被称为 `String`、`Number`、`Boolean`、`Symbol` 和 `BigInt`。因此，它们提供了不同的方法。

例如，字符串方法 [str.toUpperCase()](https://developer.mozilla.org/en/docs/Web/JavaScript/Reference/Global_Objects/String/toUpperCase) 返回一个大写化处理的字符串。

用法演示如下：

```javascript
let str = "Hello";

alert( str.toUpperCase() ); // HELLO
```

很简单，对吧？以下是 `str.toUpperCase()` 中实际发生的情况：

1. 字符串 `str` 是一个原始值。因此，在访问其属性时，会创建一个包含字符串字面值的**特殊对象**，并且具有可用的方法，例如 `toUpperCase()`。
2. 该方法运行并返回一个新的字符串（由 `alert` 显示）。
3. 特殊对象被销毁，只留下原始值 `str`。

## 数字类型

https://zh.javascript.info/number#zong-jie

精度损失，导致 `0.1+0.2 != 0.3`，也导致 `6.35.toFixed(1) == 6.3`

- `isNaN`和 `isFinite`
- `parseInt`和 `parserFloat`
- 数不尽的trick...

## 数组

数组本身是基于对象的，只是以数字作为key

我们可以给数组添加各种属性，但是一旦这么做，引擎就不会帮助我们优化

## 迭代器

https://zh.javascript.info/iterable

让一个对象的[Symbol.iterator](https://zh.javascript.info/iterable#symboliterator)key对应到一个特定的函数，那么就可以对这个对象迭代

`for let key in obj`

好处是可以实现更高程度的抽象，不必真的实现一个数组。

## map

https://zh.javascript.info/map-set

普通的object的key是string，map的key可以是任何东西，比如obj。

我们应当使用map的set和get方法来访问和更改key-value

## weakMap和weakSet

https://zh.javascript.info/weakmap-weakset

key只能是obj的map和元素只能是obj的set

特点是当引用不在时，可以垃圾回收

> 这是以不支持 `clear`、`size`、`keys`、`values` 等作为代价换来的……

## 解构

https://zh.javascript.info/destructuring-assignment

语法糖，用来获取一个对象或数组中部分属性的值

一个经典的例子：

```js
let user = {
  name: "John",
  years: 30
};

let {name, years: age, isAdmin = false} = user;

alert( name ); // John
alert( age ); // 30
alert( isAdmin ); // false
```

根据变量名直接获取到了对象的key的value

# 函数进阶

## 环境记录

```js
let x = 1;

function func() {
  console.log(x); // ReferenceError: Cannot access 'x' before initialization
  let x = 2;
}

func();
```

这段代码有助于理解环境记录

调用函数的时候，是有x的，但是还没初始化，虽然外部有，但是没用

## 闭包（closure）

https://zh.javascript.info/closure

> 在 JavaScript 中，每个运行的函数，代码块 `{...}` 以及整个脚本，都有一个被称为 **词法环境（Lexical Environment）** 的内部（隐藏）的关联对象。
>
> 词法环境对象由两部分组成：
>
> 1. **环境记录（Environment Record）** —— 一个存储所有局部变量作为其属性（包括一些其他信息，例如 `this` 的值）的对象。
> 2. 对 **外部词法环境** 的引用，与外部代码相关联。

很像写编译器的“栈”，比如第几层 `{}`

变量和函数也是“环境记录”的属性，这很JavaScript

> 一个函数其实也是一个值，就像变量一样。
>
> **不同之处在于函数声明的初始化会被立即完成。**

```js
hello();

function hello() {
  console.log("hello")
}
```

闭包通过最后返回的那个函数对象的引用没有被销毁，然后一直引用到最初的词法环境。

如果return的函数失去了引用，就被销毁。

和c语言中static的区别是，函数闭包可以同时存在多个不同，而static只是内存中一个地方。

## 命名函数表达式

NFE

为不起名字的函数内部调用函数提供方法

```js
let sayHi = function func(who) {
  if (who) {
    alert(`Hello, ${who}`);
  } else {
    func("Guest"); // 现在一切正常
  }
};

let welcome = sayHi;
sayHi = null;

welcome(); // Hello, Guest（嵌套调用有效）
```

## setTimeout

可以嵌套调用，保证每次函数调用间隔

下面这个例子，我们创建了一个闭包函数，再用一个叫做go的函数将其包起来，每次go结束后，调用go。

```js
let clo = function () {
  let num = 1;
  return () => {
    console.log(num);
    num++;
  }
}

let con = clo();

let call = setTimeout(function go() {
  con();
  setTimeout(go, 1000);
}, 1000);
```

嵌套 `setTimeout`和 `setInterval`的区别在于，

前者是callback，先call，然后函数执行结束后，再延时，再执行

后者是call后立刻延时，延时到了就执行

## 箭头函数

https://zh.javascript.info/arrow-functions

```js
let a = {
  title: "hello",
  go: function () {
    setTimeout(() => {
      console.log(this.title)
    }, 1000);
  },

  out: function () {
    setTimeout(function () {
      console.log(this.title)
    }, 1000);
  }
}

a.go(); // "hello"
a.out(); // undefined
```

箭头函数没有this，若访问，则回到外部的词法环境中寻找

## 装饰器

做一个闭包函数，这个函数接收的参数也是函数，并且在内部调用这个函数

```js
function cachingDecorator(func) {
  let cache = new Map();

  return function(x) {
    if (cache.has(x)) {    // 如果缓存中有对应的结果
      return cache.get(x); // 从缓存中读取结果
    }

    let result = func(x);  // 否则就调用 func

    cache.set(x, result);  // 然后将结果缓存（记住）下来
    return result;
  };
}
```

## func.call(context, args)

https://zh.javascript.info/call-apply-decorators

可以传递上下文，也就是this

## 防抖

https://zh.javascript.info/task/debounce

假设有一个非常频繁的upd

upd, upd, upd, upd

我们希望，如果一个upd后1000ms没有upd，则执行这个upd

解决方法就是每次先取消上一次的setTimeout，然后启动自己的setTimeout

```js
function debounce(func, ms) {
  let timeout;
  return function() {
    clearTimeout(timeout);
    timeout = setTimeout(() => func.apply(this, arguments), ms);
  };
}
```

## 节流

https://zh.javascript.info/task/throttle

防抖强调“前后无关”，只需要最后一次

节流需要记录每100ms中的调用，

如果100ms仅有一次调用，调用之

如果100ms中有多次，调用第一次和最后一次的upd

1. 一开始，状态叫做未进入节流false，
2. 若有调用，立刻调用，节流转true
3. 开始计时，100ms后，若有调用被存储，则放掉
4. 节流为true时，记录调用，直接return，因为第三步有setimeout会调用

```js
function throttle(func, ms) {

  let isThrottled = false,
    savedArgs,
    savedThis;

  function wrapper() {

    if (isThrottled) { // (2)
      savedArgs = arguments;
      savedThis = this;
      return;
    }
    isThrottled = true;

    func.apply(this, arguments); // (1)

    setTimeout(function() {
      isThrottled = false; // (3)
      if (savedArgs) {
        wrapper.apply(savedThis, savedArgs);
        savedArgs = savedThis = null;
      }
    }, ms);
  }

  return wrapper;
}
```

## this丢失和bind

https://zh.javascript.info/call-apply-decorators

进入setTimeout后，this会变，如果是浏览器，this是window，如果是node，this是timer

解决方法，对于传入setTimeout的函数，绑定this

```js
let user = {
  name: "Alice",
  sayHi() {
    console.log(this.name);
  }
}

setTimeout(user.sayHi, 1000); // undefined
```

```js
let user = {
  name: "Alice",
  sayHi() {
    console.log(this.name);
  }
}

let user1 = {
  name: "Bob",
}

let sayBob = user.sayHi.bind(user1);
setTimeout(sayBob, 1000); // Bob
```

把一个对象的函数抽离出来了

不过绑定是死的，没法再改变

# 原型

## \_\_proto\_\_

https://zh.javascript.info/prototype-inheritance

> 属性 `[[Prototype]]` 是内部的而且是隐藏的，但是这儿有很多设置它的方式。
>
> 其中之一就是使用特殊的名字 `__proto__`

手动为对象指定一个另一个对象作为proto，当该对象找不到对应的方法或者属性时，到被指定的对象去查找。

注意上下文是 `.`前的对象

但是delete只会在当前对象里寻找，不会到proto中寻找

https://zh.javascript.info/task/hamster-proto

```js
let hamster = {
  stomach: [],

  eat(food) {
    this.stomach.push(food);
  }
};

let speedy = {
  __proto__: hamster
};

let lazy = {
  __proto__: hamster
};

// 这只仓鼠找到了食物
speedy.eat("apple");
alert( speedy.stomach ); // apple

// 这只仓鼠也找到了食物，为什么？请修复它。
alert( lazy.stomach ); // apple
```

## F.prototype

https://zh.javascript.info/function-prototype

上面讲了如何手动指定一个对象的[[prototype]]，但是如果要通过 `new <Func>()`来批量生成一堆对象，就可以通过改写函数的 `prototype`属性。

```js
function Animal() {
  this.eat = function() {
    console.log("animal eat");
  }
}
Animal.prototype = { // 指定一个proto
  name: "animal",
}

let cat = new Animal();
console.log(cat.name); // animal

Animal.prototype = { // change the proto
    name: "animal get change"
}

console.log(cat.name);
let dog = new Animal();// still "animal"
console.log(dog.name) // animal got change
```

默认的proto是一个对象，有constructor属性，指向函数自身，所以可以通过constructor调用函数新建对象

## native proto

https://zh.javascript.info/native-prototypes

还记得吗，对于原始类型，我们可以 `<primative_type>.<function>`，这是因为引擎创造了一个临时对象，这个对象有这些方法，这些对象也有自己的prototype，就是Number等。

对于Array，因为它本身就是对象，所以就没有“临时对象包装器”这个说法，本身就靠 `[[prototype]]`引用到Array上。

方法借用可以向这些Number、Array借用方法。

## 原型方法

https://zh.javascript.info/prototype-methods

最好不要直接使用 `__proto__`，因为 `__proto__`对于对象而言，只是一个普通的字符串，而且是一个非常重要的字符串，JavaScript没有提供private这种东西，这个重要的东西可能被“意想不到的”方式更改。

考虑一个需求：

1. 用户输入一个字符串
2. 用户创建一个对象
3. 使用用户提供的这俩玩意，放到一个obj中

如果用户输入 `__proto__`，那么就惨了，toString可能不work。

> 正如我们所知道的，`__proto__` 不是对象的属性，而是 `Object.prototype` 的访问器属性

所以，我们使用 `Object.<prto_method>`

# 类

https://zh.javascript.info/class

类不仅仅是语法糖，

首先类做了什么，我们知道类构造出的对象，都有某种方法，这是通过proto实现的。

这个proto的constructor，就是类的名字，类的名字会变成一个function

## 重写方法

https://zh.javascript.info/class-inheritance#zhong-xie-constructor

constructor会创建对象，并指定this

子类的constructor要求调用父类的constructor，使用父类constructor产生的this

```js
class Animal {
  name = 'animal';

  constructor() {
    alert(this.name); // (*)
  }
}

class Rabbit extends Animal {
  name = 'rabbit';
}

new Animal(); // animal
new Rabbit(); // animal
```

> 所以，`new Rabbit()` 调用了 `super()`，因此它执行了父类构造器，并且（根据派生类规则）只有在此之后，它的类字段才被初始化。在父类构造器被执行的时候，`Rabbit` 还没有自己的类字段，这就是为什么 `Animal` 类字段被使用了。

## [[HomeObject]]

继承方法不是通过 `this.__proto__.<method>.call(this)`实现的

因为这样当追溯到父类时，this依旧是子类，没法继续向上追溯

```sh
animal
|
x # can not go higher
|
craw # "this" is still lizard
^
|
|this.__proto__
|
lizard
```

在往上找proto的时候，不能改变this，而是通过类方法的 `[[HomeObject]]`获取 `prototype`，**可能**是

`[[HomeObject]].__proto__`，因为教程没有细讲，我猜的

## instance of

https://zh.javascript.info/instanceof

首先看静态方法

```js
class Animal {
  static [Symbol.hasInstance](obj) {
    if (obj.canEat) return true;
  }
}
```

然后看原型链

## mixin

https://zh.javascript.info/mixins

类似于接口。把一个对象的的行为拷贝到另一个类里面。

# 错误处理

## try...catch...[finally ...]

https://zh.javascript.info/try-catch

为了防止脚本死掉，try-catch

- 可以手动在try中throw一个error抛出，然后在catch中处理
- rethrowing，我们可以判断捕获到的error类型，如果不是想要的，可以在 catch 中 throw出去
- 哪怕try中有return，finally也会执行

# Promise

https://zh.javascript.info/promise-basics

promise是一个状态机，接收一段能够修改自己状态的代码，

当自己状态改变后，对应地执行通过 `then`和 `catch`”注册“到自己身上的函数

promise在自己被创建时立刻执行

```js
// 下面这 promise 在被创建后立即变为 resolved 状态
let promise = new Promise(resolve => resolve("done!"));

promise.then(console.log); // done!（现在显示）
promise.then(console.log); // done!（现在显示）
promise.then(console.log); // done!（现在显示）
```

连续显示三次done。内部实现上，肯定有把注册到promise上的函数存储起来

## finally

当状态变成 `fulfilled`时执行 `then`上注册的函数。

## then

then是一个很特别的东西，我们可以使用then来进行链式调用，why？

参考：

某位coder自己实现的promise：https://blog.csdn.net/qq_36297981/article/details/95354423

promiseA+规范英文：https://promisesaplus.com/

promiseA+规范中文翻译：https://www.ituring.com.cn/article/66566

> `then` 方法必须返回一个 `promise` 对象
>
> `promise2 = promise1.then(onFulfilled, onRejected);   `

> Promise A+ 对onFulfilled和onRejected的返回值x的处理做了明确的规定：
> 如果 onFulfilled 或者 onRejected 返回一个值 x ，则运行下面的 Promise 解决过程：
> Promise解决过程:
> Promise 解决过程是一个抽象的操作，其需输入一个 promise 和一个值，我们表示为
>
> `[[Resolve]](promise, x)`，
>
> 如果 x 有 then 方法且看上去像一个 Promise ，解决程序即尝试使 promise 接受 x 的状态；否则其用 x 的值来执行 promise 。
>
> 这种 thenable 的特性使得 Promise 的实现更具有通用性：只要其暴露出一个遵循 Promise/A+ 协议的 then 方法即可；这同时也使遵循 Promise/A+ 规范的实现可以与那些不太规范但可用的实现能良好共存。
>
> 运行 `[[Resolve]](promise, x)`需遵循以下步骤：
>
> - x 与 promise 相等
>   如果 promise 和 x 指向同一对象，以 TypeError 为据因拒绝执行 promise
> - x 为 Promise
>   如果 x 为 Promise ，则使 promise 接受 x 的状态:
>
>   如果 x 处于等待态， promise 需保持为等待态直至 x 被执行或拒绝
>   如果 x 处于执行态，用相同的值执行 promise
>   如果 x 处于拒绝态，用相同的据因拒绝 promise
> - x 为对象或函数
>
>   把 x.then 赋值给 then
>   如果取 x.then 的值时抛出错误 e ，则以 e 为据因拒绝 promise
>   如果 then 是函数，将 x 作为函数的作用域 this 调用之。传递两个回调函数作为参数，第一个参数叫做 resolvePromise ，第二个参数叫做 rejectPromise:
>   如果 resolvePromise 以值 y 为参数被调用，则运行 ` [[Resolve]](promise, y)`
>   如果 rejectPromise 以据因 r 为参数被调用，则以据因 r 拒绝 promise
>   如果 resolvePromise 和 rejectPromise 均被调用，或者被同一参数调用了多次,则优先采用首次调用并忽略剩下的调用
>   如果调用 then 方法抛出了异常 e：
>   如果 resolvePromise 或 rejectPromise 已经被调用，则忽略之
>   否则以 e 为据因拒绝 promise
>   如果 then 不是函数，以 x 为参数执行 promise
> - 如果 x 不为对象或者函数，以 x 为参数执行 promise

这里有两个地方很难看懂

### x为对象或函数

https://zh.javascript.info/promise-chaining#:~:text=Thenables

```js
class Thenable {
  constructor(num) {
    this.num = num;
  }
  then(resolve, reject) {
    alert(resolve); // function() { native code }
    // 1 秒后使用 this.num*2 进行 resolve
    setTimeout(() => resolve(this.num * 2), 1000); // (**)
  }
}

new Promise(resolve => resolve(1))
  .then(result => {
    return new Thenable(result); // (*)
  })
  .then(alert); // 1000ms 后显示 2
```

把这个这个 `thenable`的then作为参数，给这个新的promise

直到这里，想想我们初学promise时候，promise接受的，正是一个 `then`函数。

### x不为对象或函数

”以 x 为参数执行 promise“，什么傻逼翻译，给我看乐了

英文原文是https://promisesaplus.com/

> If `x` is not an object or function, fulfill `promise` with `x`.

就是直接 `promise2.resolve(x)`

## 错误处理

https://zh.javascript.info/promise-error-handling

链式调用，catch处理前面reject的promise，很方便

如果前面是通过throw抛出错误，也会被catch捕捉，这叫做”隐式try catch“

注意，catch处理完了，正常return，会是一个fullfill的promise，接着走then

当然，也可以catch只处理一种错误，抛出其他的错误，让下面的catch处理

## promise化

https://zh.javascript.info/promisify

原来的函数需要一个回调函数，那我们九把原来的函数包起来，在一个新的promise中调用，并构造一个新的回调函数，然后原来的函数call的时候，把自己的回调函数传进去。

trick使用了 `...args`

## async await

https://zh.javascript.info/async-await

async修饰函数，把返回的结果包装成promise，参考promiseA+规范

await等待async修饰的函数的promise settled，拿到结果value

# generator

https://zh.javascript.info/generators

```js
function* generateSequence() {
  yield 1;
  yield 2;
  return 3;
}

// "generator function" 创建了一个 "generator object"
let generator = generateSequence();
alert(generator); // [object Generator]

for(let value of generator) {
  alert(value); // 1，然后是 2
}
```

调用 `generator.next()`，返回一个对象

> `next()` 的结果始终是一个具有两个属性的对象：
>
> - `value`: 产出的（yielded）的值。
> - `done`: 如果 generator 函数已执行完成则为 `true`，否则为 `false`。

注意，使用for let调用，最后

> 当 `done: true` 时，`for..of` 循环会忽略最后一个 `value`。因此，如果我们想要通过 `for..of` 循环显示所有的结果，我们必须使用 `yield` 返回它们

## yield 是双向的

- 第一次调用不能传参数
- 先yield出来，然后才能在第二次yield的时候把值传入

```js
function* gen() {
  let ask1 = yield "2 + 2 = ?";

  alert(ask1); // 4

  let ask2 = yield "3 * 3 = ?"

  alert(ask2); // 9
}

let generator = gen();

alert( generator.next().value ); // "2 + 2 = ?"

alert( generator.next(4).value ); // "3 * 3 = ?"

alert( generator.next(9).done ); // true
```

## 异步generator

https://zh.javascript.info/async-iterators-generators

对象内部需要实现 `[Symbol.asyncIterator]`方法

> 我们使用 `for await(let value of range)` `(4)` 来进行迭代，也就是在 `for` 后面添加 `await`。它会调用一次 `range[Symbol.asyncIterator]()` 方法一次，然后调用它的 `next()` 方法获取值。

# module

https://zh.javascript.info/modules-intro

- 一个文件就是一个module
- module之间可以通过export和import交互

`	export sth`，`import {sth} from "sth"`是最简单的import方式

module只会被加载一次，比如一个module export一个对象，很多module又import了这个对象，那么内存里还是只有一个对象。因此一个module可以export一个config对象，弄成“可配置”的module。

`<script type="module">`和 `<scirpt>`是不一样的

```html
<script>
  alert(this); // window
</script>

<script type="module">
  alert(this); // undefined
</script>
```

- this不一样
- 延迟执行，但是如果使用async，就是立刻执行

## 导入导出

常规的导入导出就不提了

`import * as obj from './xx.js'`，可以通过 `obj.<export_thing>`调用

export也有as语法

## export default

这解决了我在项目开发中什么时候import要用 `{}`什么时候不用的疑惑

一个文件只允许一个default export

- 导入时不用指定 `{}`
- 如果export时候没有指定名称也是允许的，因为default只能有一个

export也能单独列出一句，有利于实现与导出分离

```js
function sayHi(user) {
  alert(`Hello, ${user}!`);
}

// 就像我们在函数之前添加了 "export default" 一样
export {sayHi as default};
```

## 重新导出

为父文件导入子文件，再导出子文件，为项目的访问提供统一的入口

```js
// 📁 auth/index.js

// 导入 login/logout 然后立即导出它们
import {login, logout} from './helpers.js';
export {login, logout};

// 将默认导出导入为 User，然后导出它
import User from './user.js';
export {User};
```

新的语法：`export from`

```js
// 📁 auth/index.js
// 重新导出 login/logout
export {login, logout} from './helpers.js';

// 将默认导出重新导出为 User
export {default as User} from './user.js';
...
```

这不是语法糖，因为后者无法再使用login

## 动态导入

https://zh.javascript.info/modules-dynamic-imports

使用 `import()`语法，返回一个promise

> resolve 为一个包含其所有导出的模块对象。我们可以在代码中的任意位置调用这个表达式。

```html
<!doctype html>
<script>
  async function load() {
    let say = await import('./say.js');
    say.hi(); // Hello!
    say.bye(); // Bye!
    say.default(); // Module loaded (export default)!
  }
</script>
<button onclick="load()">Click me</button>
```

# 杂项

## 代理

新建一个对象，拦截对元对象的操作。如get属性，列出属性等操作

## 柯里化

https://zh.javascript.info/currying-partials

`sum(a,b)`可以以 `sum(a)(b)`的方式调用

```js
function curry(func) {
  return function curried(...args) {
    if (args.length >= func.length) {
      return func.apply(this, args);
    } else {
      return function(...args2) {
        return curried.apply(this, args.concat(args2));
      }
    }
  };
}
```

递归实现，每接收一个参数，就返回一个偏函数

柯里化的用处：构造新的，预先填充参数的函数

## reference type

https://zh.javascript.info/reference-type

`obj.method`时候，返回了一个`(obj, method_name, strict?)`

如果`sth = obj.method`，reference type被转换，丢失了this，所以

```js
let user = {
  name: "John",
  hi() { alert(this.name); }
};

// 把获取方法和调用方法拆成两行
let hi = user.hi;
hi(); // 报错了，因为 this 的值是 undefined
```
