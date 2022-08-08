# openss-ext-sm2

#### 介绍
基于openssl密码库编写的sm2国密算法PHP扩展

#### 软件架构
zend 常规PHP扩展结构

#### 安装教程
解压进入openss-ext-sm2目录

1.  phpize
2.  ./configure
3.  make&&make install
4.  修改php.ini

#### 使用说明

1.  创建公钥和私钥
```
$pub_key 取地址 结果为二进制
$pri_key 取地址 结果为二进制

sm2_key_pair($pub_key, $pri_key);

返回值int 0 成功 其他状态失败

```
2.  签名
```
$msg 信息
$signature 输出签名结果
$pri_key 私钥 二进制
$iv userid 

sm2_sign($msg, $signature, $pri_key, $iv)


返回值int 0 成功 其他状态失败

```
3.  验签
```
$msg 信息
$signature 输入签名结果
$pub_key 公钥 二进制
$iv userid

sm2_sign_verify($msg, $signature, $pub_key, $iv)；

返回值int 0 成功 其他状态失败
```
4. 公钥加密
```
$msg 信息
$encrypt 输出加密结果 二进制 
$pub_key 公钥 二进制
sm2_encrypt($msg, $encrypt, $pub_key)

返回值int 0 成功 其他状态失败
```
5. 私钥解密
```
$encrypt 加密信息 二进制
$string 输出结果 明文
$pri_key 私钥
sm2_decrypt($encrypt, $string, $pri_key)

返回值int 0 成功 其他状态失败
```
6. 演示
```

 $msg = '这是测试';
 $iv = '1234567812345678';

 sm2_key_pair($pub_key, $pri_key);

 #公钥:BHSAPGXtrHNxqJ3/b0+eNu2mdO0mpDfTGNJUMoEWpNpSL53Dw+YM/B/QT5OoLm4xQtw0hZY5wlWTR+cD629Grek=
 #私钥:++BuzKd1mPa0RXAJcY6DHDq9SUzo3T6/engbKReQRqI=

 sm2_sign($msg, $signature, $pri_key, $iv);

 #私钥签名:+YHNtKkXbsRSs2nk5amd/YNqsiH8Kyr+oyLVVzuvRl+lqb40uzPxjsRo9QTYw7kZdWSfvM5lbxDMfF0cugQNfQ==

 sm2_sign_verify($msg, $signature, $pub_key, $iv);

 #公钥验签:0

 sm2_encrypt($msg, $encrypt, $pub_key);

 #公钥加密:BBdm04Uh5EgzYKG3Ff8rBFJQZxRSXnrh9/WDZxS6PmzfnTDz0O0C115BPxMDfBNnOK5Ixs9kHTJPNSDoiHoiEmrnuotKN53rxnJtNd3MTbRjJOQ0sas9Kdktl1eHzj2/eseNaGh0LHZIOrBxAQ==
 sm2_decrypt($encrypt, $string, $pri_key);

 #私钥解密:这是测试

```
#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request


#### 特技

1.  使用 Readme\_XXX.md 来支持不同的语言，例如 Readme\_en.md, Readme\_zh.md
2.  Gitee 官方博客 [blog.gitee.com](https://blog.gitee.com)
3.  你可以 [https://gitee.com/explore](https://gitee.com/explore) 这个地址来了解 Gitee 上的优秀开源项目
4.  [GVP](https://gitee.com/gvp) 全称是 Gitee 最有价值开源项目，是综合评定出的优秀开源项目
5.  Gitee 官方提供的使用手册 [https://gitee.com/help](https://gitee.com/help)
6.  Gitee 封面人物是一档用来展示 Gitee 会员风采的栏目 [https://gitee.com/gitee-stars/](https://gitee.com/gitee-stars/)
