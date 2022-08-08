# openssl-ext-sm2

#### 介绍
基于openssl密码库编写的sm2国密算法PHP扩展

#### 软件架构
zend 常规PHP扩展结构

#### 安装教程
解压进入openssl-ext-sm2目录

1.  phpize
2.  ./configure  --with-openssl=/usr/local/openssl 你的openssl lib里必须包含 动态库否则程序异常
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
