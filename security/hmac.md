# HMAC（Hash-based Message Authentication Code）

-   created: 2025-01-09T16:08+08:00
-   published: 2025-01-14T18:46+08:00
-   category: security

## 数据签名（MAC）

[数据签名 md5 数据签名的意义](https://juejin.cn/post/7124906198960340999)

对称加密，比如请求的参数，client 可以直接 `sign = md5(args, timestamp, secret_key)`
这个 secret_key 是事先双方约定好的，别人不知道。

server 获取到 `args` 和 `timestamp`，也根据这个 secret_key 计算下 sign，一致就说明参数正确。

这种办法会被 [哈希长度扩展攻击（hash length extension attacks）](https://ventureyu.github.io/2018/08/14/hash%E9%95%BF%E5%BA%A6%E6%89%A9%E5%B1%95/)

> 原因在于基于 Merkle–Damgård 构造哈希函数有类似的特点：
> 以区块为单位操作数据（MD5, SHA1, SHA256 的区块长度是 512 bits，大多数 message 的长度不会刚好可以被哈希函数的区块长度整除。这样一来，message 就必须被填充(padding)至区块长度的整数倍）
>
> 每个消息块都会和一个输入向量做一个运算，把这个计算结果当成下个消息块的输入向量，
> 初始化向量是定义好的，在最后一块的时候，才会将其对应的链接变量转换为 hash 值。

所以要改成 HMAC

[hash 长度扩展攻击 \- dre0m1 \- 博客园](https://www.cnblogs.com/dre0m1/p/15858202.html)

## HMAC（Hash-based Message Authentication Code）

参考：[HMAC 与 CMAC_cmac 和 hmac\-CSDN 博客](https://blog.csdn.net/lianyunyouyou/article/details/118557410)

HMAC 简单来说就是 `H(key || H(key || msg))`，什么 ipad opad 不管他，  
较于 MAC 可以防止一些破解方式。

通常 hmac-sha256 后的结果是 `bytes[]`，可以通过 base64 encode 后携带在请求体中。

关于 base64 可以参考文章：

[base64 \- Python 教程 \- 廖雪峰的官方网站](https://liaoxuefeng.com/books/python/built-in-modules/base64/)

简单来说就是 字节流中，每 6 个 bit 一组，映射到 `A B C ... /` 64 个基本字符。
每 3 个 byte 映射到 `3 * 8 / 6 = 4` 个 char。

如果 byte 数为 3k+1，多的 1 个 byte 可以产出 1.x 个 char，还需再补 2 个 `0x00` byte，两个末尾 `0x00` char 用 `==`，  
如果 byte 数为 3k+2，多的 2 个 byte 可以产出 2.x 个 char，就要再补 1 个 `0x00` byte，一个末尾 `0x00` char 用 `=`。
