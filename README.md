# RF Hacker

![img](https://static-1302980926.cos.ap-beijing.myqcloud.com/rfhacker/rfhacker.gif)



## 简介

因[*rc-switch*](https://github.com/sui77/rc-switch)库无法识别一些遥控器，故开发此工具，类似逻辑分析仪，全协议支持（信号频率低于MCU运行频率），可用于433MHz、315MHz、红外信号的分析处理及回放



## 说明

###### 文件结构：

* firmware
  * esp.ino => 适用于**ESP32**或**ESP8266**
  * arduino.ino => 适用于**arduino uno**或 **arduino nano**[^1]
* html
  * canvas.html => 绘图
  * rf.html => AP模式控制UI
* UI.zip => 控制软件，见首图

[^1]: 因arduino性能不足，缓存较小，不建议使用



###### 通信方式：

1. 串口通信（推荐，稳定，已完善）
2. ~~WIFI通信（不稳定，待完善）~~



## 进度

###### 已完成

+ [x] 信号可视化（Canvas绘制）
+ [x] 信号回放
+ [x] 方便的信号截取

###### 待完成

+ [ ] 一键导出回放代码
+ [ ] ESP8266 AP模式支持
+ [ ] AP模式下的更多功能![img](https://static-1302980926.cos.ap-beijing.myqcloud.com/rfhacker/rfhacker_web.jpg)