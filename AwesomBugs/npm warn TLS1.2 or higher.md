npm notice Beginning October 4, 2021, all connections to the npm registry - including for package installation - must use TLS 1.2 or higher. 

```bash
npm notice Beginning October 4, 2021, all connections to the npm registry - including for package installation - must use TLS 1.2 or higher. You are currently using plaintext http to connect. Please visit the GitHub blog for more information: https://github.blog/2021-08
-23-npm-registry-deprecating-tls-1-0-tls-1-1/
```

解决方法：

```bash
npm install -g npm
```

也就是把npm更新到最新版本。