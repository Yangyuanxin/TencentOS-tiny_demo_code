# TencentOS-tiny_MX+_demo

#### 介绍
用于存放编写基于TencentOS-tiny定制开发板MX+的例程

### 00.TencentOS_tiny_MultiTasking（已完成）

TencentOS tiny多任务例程

### 01.TencentOS_tiny_SoftTimer（已完成）

TencentOS tiny软件定时器基本使用

软件定时器：不同于硬件定时器，用软件逻辑实现的定时器，节拍利用的是系统调度所以来的系统嘀嗒时钟，也就是我们熟悉的systick。

### 02.TencentOS_tiny_Mutex（已完成）

TencentOS tiny互斥锁基本使用

互斥锁解决的问题: 不可重入资源的竞争性访问。

### 03.TencentOS_tiny_Sem（已完成）

TencentOS tiny信号量基本使用

信号量解决的问题:任务间同步，最典型的案例是生产者与消费者。

### 04.TencentOS_tiny_Event（已完成）

TencentOS tiny事件基本使用

事件解决的问题：任务间某种事件是否发生的同步问题。

### 05.TencentOS_tiny_Queue（已完成）

TencentOS tiny队列(邮箱)基本使用

队列(邮箱)解决的问题：带"负载"的消息传递，比如串口接收到一帧数据，通过邮箱来传递。

### 06.Tencent_OS_tiny_Esp8266_At_frame（已完成）

TencentOS  tiny at框架基本使用

at框架解决的问题：使无线串口通信模组更加简单易用。

本例程实现的是通过按键事件发送网络数据，控制智能小车前进、后退、左转、右转

### 07.Tencent_OS_tiny_Esp8266_At_frame_OneNet(已完成)

基于AT框架，实现随机数据上传到OneNet平台

![image-20200923093524302](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20200923093524302.png)

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
