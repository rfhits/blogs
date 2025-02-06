# jieba 无法提取单个字符

-   created: 2023-11-13T15:28+08:00
-   published: 2023-11-13T15:45+08:00
-   category: python

以句子`"我喜欢赵"`为例，用「赵」代指某个人名，使用 `jieba` 提取关键词：

```python
import jieba
import jieba.analyse
import jieba.posseg as pseg

short_content = "我喜欢赵"
short_words = pseg.lcut(short_content) # [pair('我', 'r'), pair('喜欢', 'v'), pair('赵', 'nr')] # 可以看到分词的词性是对的
jieba.analyse.extract_tags(short_content) # ['喜欢'] # 但是提取到的关键词少了"赵"
```

原因是在 `jieba/analyse/tfidf.py` 里有一段代码，过滤掉了所有长度小于 2 的字符串：

```python
# jieba/analyse/tfidf.py
# extract_tags()
freq = {}
for w in words:
    if allowPOS:
        if w.flag not in allowPOS:
            continue
        elif not withFlag:
            w = w.word
    wc = w.word if allowPOS and withFlag else w
    if len(wc.strip()) < 2 or wc.lower() in self.stop_words: # <--- LOOK HERE
        continue
    freq[w] = freq.get(w, 0.0) + 1.0
total = sum(freq.values())
```

本意是过滤掉空白字符串 `\s+` 和标点符号，但是把单个字符表示的人名也给丢掉了。

解决步骤：

1. 因为要直接修改第三方库，所以以管理员权限打开这个文件，把那一行改成：
    ```python
    if len(wc.strip()) < 1 or wc.lower() in self.stop_words:
    ```
    这样只过滤掉空白符，如果不过滤空白符的话也会报错
2. 添加标点符号到 stopwords 词库，`jieba.analyse.set_stop_words('./path_to_stopwords.txt')`

附标点符号库，使用时候自行替换空格为换行符：

```
0 1 2 3 4 5 6 7 8 9 , . : ; " ' / [ ] { } \ | + - ) ( ) * & ^ % $ # @ ! ~ ` 。 ， ！ ？ ： ； 「 」 、 ： — … … — — － － ／ ［ ］ 【 】 （ ） ｛ ｝ 《 》 〈 〉 「 」 『 』 “ ” ‘ ’ ‵ ′ ＂ ＇ ﹃ ﹄ ﹁ ﹂ ﹏ ﹏ ︴ ︵ ︿ ﹀ ︹
```

**用完记得改回来**

参考：[GitHub \- fxsjy/jieba: 结巴中文分词](https://github.com/fxsjy/jieba)
