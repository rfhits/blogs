P4V 使用笔记

-   Created: 2024-03-05T15:28+08:00
-   Published: 2024-10-22T13:10+08:00

Categories: Applications

P4V 表示 Perforce Visual Client，Perforce 还有服务器端之类的。

教程：[Working with Perforce and P4V \- YouTube](https://www.youtube.com/playlist?list=PLxdnSsBqCrrGq_8ecmdE7A6KnRfbhHE4Q)
最常用步骤：[Basic Operations with Perforce \- YouTube](https://www.youtube.com/watch?v=_5_LK_0H22I)

要把文件添加到 pending list 里才可以提交，pending list 相当于 git 的 add 和 commit 合二为一。

修改文件工作流：

1. 新建 pending list
2. 将自己要修改的文件右键 checkout 到对应的 pending list，checkout 后的文件 read only 会被修改，并且左上角会有红色 √
3. 一旦 checkout，将通知服务器，服务器也会通知所有客户端，这个文件被某人 checkout 了
4. get latest revision 会拉取服务器文件，为了避免本地文件被覆盖，最好 checkout 到 pending list 中

添加/删除文件工作流：

1. 新建文件，右键 mark for add，会要求 add this change to pending list
2. 删除文件：右键问价 mark for delete，也会将这个 delete 的文件添加到指定的 pending list

恢复文件：在 pending list 中右键 revert
显示 diff：在 pending list 中 diff against have revision

submit change：右键 pending list 选择 submit

B 站的，我没看过：[如何提高团队的 UE4 开发效率](https://space.bilibili.com/149146076/channel/seriesdetail?sid=1749302)
