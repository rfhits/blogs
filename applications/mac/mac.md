# Mac 配置

-   created: 2024-12-06T17:25+08:00
-   published: 2024-12-07T10:47+08:00
-   categories: applications
-   tags: mac

## 键位修改

快捷键失灵的话，用 shortcutdetective 这个软件检测[^1]，已知：

-   滴答清单占据 `command + shift + p`
-   演讲者前置也占据 `command + shift + p`

```bash
brew install shortcutdetective
```

其他修改：

1. 设置里，查找 `键盘快捷键`，把 fn 改到 ctrl, 因为 fn 在键盘的左下角，这个位置给 ctrl 留着比较方便。
2. 安装搜狗输入法使用 shift 切换中英文，在搜狗输入法的「高级」中导入系统短语。

### 使用 Karabiner 映射到 vim 方向键

|   shortcut    |        target        |           备注            |
| :-----------: | :------------------: | :-----------------------: |
|   caps_lock   |        escape        |                           |
| caps_lock + j |      down_arrow      |                           |
| caps_lock+ k  |       up_arrow       |                           |
| caps_lock + h |      left_arrow      |                           |
|  caps_lock+l  |     right_arrow      |                           |
| caps_lock + w | option + right_arrow |     向后移动一个 word     |
| caps_lock + b | option + right_arrow |     向前移动一个 word     |
| caps_lock + a |   cmd + left_arrow   | 跳转到行首，不要改到 home |
| caps_lock + ; |  cmd + right_arrow   | 跳转到行末，不要改到 end  |

对应规则见：<a href="./karabiner-rule-capslock-vim.json" target="_blank">karabiner-rule-rule-capslock-vim.json</a>

这个 json 并不复杂，第一条 manipulator 设置了按下 capslock 后 `caps_lock pressed` 变量为 1，  
后面就是从 from 映射到 to，mac 下这些 command、option、ctrl 统称为 `modifiers`

#### Karabiner rule

Karabiner rule 编写，如果通过 UI `add rule`，那需要的是一个 `rule.json`，rule 这个 obj 的描述如下：

```yaml
- rule: obj
    - description: str
    - manipulators: list[manipulator]

- manipulator: obj
  - type: str
  - conditions: list[condition]
  - from: obj
  - to: obj
```

网络上有些文章直接在 `~/.config/karabiner/assets/` 添加 json[^2]，需要的是一个 `rules_description` object。

```yaml
- title: str
- rules: list[rule]
```

如果把这份 `rules_description.json` 直接填到通过 UI `add rule` 需要的 json 文件中，就会报错：

```
error: `manipulators` is missing or empty in ...
```

因为它需要的是一个对 rule object 的描述 json。

## 其他软件

-   截屏：[snipaste](https://www.snipaste.com/)
-   homebrew: [Homebrew — The Missing Package Manager for macOS \(or Linux\)](https://brew.sh/)
-   鼠标平滑：[Mos](https://github.com/Caldis/Mos)
-   剪贴板历史：[Maccy: Lightweight clipboard manager for macOS](https://github.com/p0deje/Maccy)

[^1]: https://github.com/microsoft/vscode/issues/182834#issuecomment-1822218334
[^2]: [如何在 Mac 上使用 Shift 来切换中英文 – 栋哥的赛博空间](https://liuyandong.com/archives/9740)
