需求是向后端传登陆用户的ID，写成`this.$store.user.userID`，

结果后端没收到，原来是少写了一个`state`

改为`this.$store.state.user.userID`后正常。