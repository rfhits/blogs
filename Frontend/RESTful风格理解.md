
https://www.bilibili.com/video/BV1BA411c7Sd

理解RESTful风格的最好方式是举例

以知乎为例

# 知乎的api

## 关注问题

POST https://www.zhihu.com/api/v4/questions/291486382/followers

## 取消关注

Request URL: https://www.zhihu.com/api/v4/questions/291486382/followers
Request Method: DELETE

## 发布回答

POST https://www.zhihu.com/api/v4/questions/291486382/answers

## 删除回答

Request URL: https://www.zhihu.com/api/v4/answers/2422133388
Request Method: DELETE

## 点赞回答

相当于新增了一个voter

Request URL: https://www.zhihu.com/api/v4/answers/1947177736/voters
Request Method: POST

## 新建收藏

Request URL: https://www.zhihu.com/api/v4/collections
Request Method: POST

## 收藏回答

Request URL: https://www.zhihu.com/api/v4/collections/794647382/contents?content_id=476717945&content_type=answer
Request Method: POST

## 问题

如何设计获取一个问题下的所有回答？

`get questions/<id>/`

参数带page,带 order

获取某个特定的回答呢？

`get answers/<id>`