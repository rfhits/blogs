XGBoost 使用

Datetime: 2023-03-12T20:25+08:00

Categories: Python | MachineLearning

[toc]

我从未搭建过任何机器学习模型，所以在 Youtube 上找到一个 XGBoost 的[简单教程](https://www.youtube.com/watch?v=GrJP9FLV3FE) 跟着做了一遍，大概有了一点感觉。

做完以后看官方文档，那文档真的不是给人看的。

包的安装就是 `conda install xgboost`，xgboost 提供两种接口，一种是原生的，一种是 sklearn-compatible 的，本文讲述 sklearn-compatible。

使用 xgboost 的流程也很简单：

1. 定义一个 xgb 模型，比如 `model = xgb.XGBClassifier(objective="binary:logistic")`
2. 用 xgb 去 fit 数据集
3. 用 xgb 在测试集上小试牛刀
4. 调参得到最终的模型

# 官方文档阅读

第一次阅读官方文档，得开好几个标签页。

官方文档给我的感觉就是，想要一份文档解决 Python、R 等语言，最后的结果反而是都解决不好。如果文档好的话，互联网上就不会有那么多二次解读了。

再看了一会以后决定今后写文档不使用 readthedocs.io 这个网站，什么玩意，目录直接放在头上，而且只到二级，而且还没有例子，而且还没法折叠那么多的参数。我以为之前我一直学不会是我太菜了，现在吐槽了一下发现自己的人生质量应该还在这份文档之上。

一点阅读建议：

1. 按需阅读
2. 使用 `Ctrl + F`，整篇文档没有索引链接跳转，全靠搜索

# 参数有哪些

参考文章：[XGBoost 重要参数 (调参使用)](https://www.cnblogs.com/TimVerion/p/11436001.html)，[官方文档：parameters](https://xgboost.readthedocs.io/en/stable/parameter.html#xgboost-parameters)

一上来文档就直接甩出了亿堆参数，分为三四类吧，这些参数和具体实现语言无关。

我认为有两个问题：

1. 看参数的时候，太多了，看到一半，要跳到下一个种类，只能回到文章开头
2. 顺序有问题，参数有轻缓极重的

这三类里面，先看[learning-task-params](https://xgboost.readthedocs.io/en/stable/parameter.html#learning-task-parameters)，这告诉我们 XGBoost 能做什么

我只关注两个参数，怎么用先不说：

1. objective 就是目标函数，分类任务和回归任务的目标函数肯定是不一样的
2. eval_metric，当前模型的评价标准，这个参数会影响「提前停止」

然后是 [tree-booster-parameters](https://xgboost.readthedocs.io/en/stable/parameter.html#parameters-for-tree-booster)

因为我没用回归任务，就不看了，这里也是过一下，有个大概的印象，比如学习率、最大树深度这种不痛不痒的参数，这个时候主次就出来了，要配合调参指南一起看

知道有哪些参数后直接跳 [sklearn api 页面](https://xgboost.readthedocs.io/en/latest/python/python_api.html#module-xgboost.sklearn)，然后使用 `Ctrl + F`，找到 XGBClassifier，看参数，直到现在，我们要写的代码的参数才和文档开头的 parameters 对应上

```python
# 一个最简单的分类器
model_base = xgb.XGBClassifier(objective="binary:logistic",
                            seed=42,
                            scale_pos_weight=scale_pos_weight,
                            early_stopping_rounds=50,
                            eval_metric='auc'
                            )
model_base.fit(X_train,
            y_train,
            verbose=True,
            eval_set=[(X_test, y_test)])
```

# 参数调节

[XGBoost | Complete Guide to Parameter Tuning in XGBoost With Code](https://www.analyticsvidhya.com/blog/2016/03/complete-guide-parameter-tuning-xgboost-with-codes-python/?)

[XGboost 数据比赛实战之调参篇 (完整流程)](https://segmentfault.com/a/1190000014040317)

参数并没有规定的范围，都是从一个小范围开始尝试。

两种方法，一种是一次性只调节一两个参数，位于边界就扩张，位于内部就缩小范围，第二种是直接扔到 GridSearchCV 里

| 参数             | 参考范围                     | 初始值 | 描述           |
| ---------------- | ---------------------------- | ------ | -------------- |
| n_estimator      | 100, 1000                    | 100    |                |
| max depth        | 3，10                        | 5      |                |
| min child weight | 1,6                          | 1      |                |
| gamma            | 0，0.6                       | 0      | 分裂阈值       |
| subsample        | 0.6,1                        |        | 多少样本来训练 |
| colsample_bytree | 0.6,1                        |        | 多少特征来训练 |
| reg_alpha        | [1e-5, 1e-2, 0.1, 1, 100]    |        |                |
| reg_lambda       | 同上                         |        |                |
| learning rate    | [0.01, 0.05, 0.07, 0.1, 0.2] | 0.1    |                |

我自己调参带来的收益很小，千分之一的提升，聊胜于无。

下面是使用 GridCV 对参数进行逐个调整的代码。因为懒，所以我并没有对达到边界的参数进行进一步调整。

```python
# 待调节参数及其范围
tune_params_dict = {
    'n_estimators': [50, 100, 200, 300],
    'max_depth': list(range(3,10,2)),
    'min_child_weight': list(range(1, 6)),
    'gamma': [i/ 10 for i in range(0,6)],
    'subsample': [i/10 for i in  range(6, 10)],
    'colsample_bytree': [i/10 for i in  range(6, 10)],
    'reg_alpha': [1e-5, 1e-2, 0.1, 1, 100],
    'reg_lambda': [1e-5, 1e-2, 0.1, 1, 100],
    'learning_rate': [0.01, 0.02, 0.05, 0.1, 0.15],
}

# 调参顺序
tune_params = ['n_estimators', 'max_depth', 'min_child_weight', 'gamma', 'subsample', 'colsample_bytree', 'reg_alpha', 'reg_lambda', 'learning_rate']

# 已经调节好的参数
tuned_params = {
    'objective': "binary:logistic",
    'seed': 42,
    'eval_metric': 'auc',
    'scale_pos_weight': scale_pos_weight,
    'early_stopping_rounds': 50
}

# 逐个参数调节
for param in tune_params:
    print('current tuning param:', param)
    default_model = xgb.XGBClassifier(**tuned_params)
    param_grid = {}
    param_grid[param] = tune_params_dict[param]
    tuned_model = GridSearchCV(estimator=default_model, param_grid=param_grid, scoring='roc_auc', n_jobs=4)
    tuned_model.fit(X_train, y_train, eval_set=[(X_test, y_test)])
    print(tuned_model.best_params_)
    tuned_params[param] = tuned_model.best_params_[param]
    print('update best params:', tuned_params)
```

# api

### 预测结果并输出概率

```python
model.fit(x,y)
model.predict(test_x)
model.predict_proba(test_x)
```

### 保存与加载模型

```python
model.save_model('model.json')
model = xgb.XGBClassifier()
model.load_model('./model_tuned.json')
```

### 特征重要性

参考 [How to get feature importance in xgboost?](https://stackoverflow.com/questions/37627923/how-to-get-feature-importance-in-xgboost)

两种方法：

1. 画图
2. 转 DataFrame 看

画图：

```python
from xgboost import plot_importance
plot_importance(model, max_num_features = 15)
pyplot.show()
```

转 DataFrame

```python
featimp_base = pd.DataFrame(model_base.get_booster().get_fscore().items(), columns=['feature','importance']).sort_values('importance', ascending=False)
```
