# openss-ext-sm2

#### 介绍
基于openssl密码库编写的sm2国密算法PHP扩展

#### 软件架构
zend 常规PHP扩展结构

#### 安装教程

1.  phpize
2.  ./configure
3.  make&&make install
4.  修改php.ini

#### 使用说明

1.  创建公钥和私钥
```
sm2_key_pair($pub_key, $pri_key);

$pub_key 取地址 结果为二进制
$pri_key 取地址 结果为二进制
返回值int 0 成功 其他状态失败

```
2.  xxxx
3.  xxxx

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
