# module-i2c_gpio

#### 介绍

相比硬件I2C，使用gpio模拟I2C具有移植性好、简单通用，方便进行对cdd快速验证。一般gpio模拟i2c千篇一律的会写6-7个函数、分别是start、stop、write、read、write_ack、write_nack、read_ack等，本项目做了一些创新，仅封装了4个函数，分别是start、write_read_ack、read_write_ack、stop。同时对接SCL和SDA的GPIO做了封装，除此之外还做了对SDA不支持开漏输出的兼容，时序上面把常用的都放出来可以配置，实测使用stm32时钟速度可以达到200k+，基本满足大多数需求。

#### 软件架构

详情见B站网址:


#### 使用说明

1.  参考user_demo文件夹下xxx_user.c和xxx_user.h，移植需要提供SCL和SDA的输出高低电平函数、对于SDA不支持开漏输出的需要实现set_sda_dir的接口。


#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request


#### 支持本项目

若这个项目帮到了你，不妨点个星标~，愿意的话也可以小额捐赠，感谢每一份认可~~


#### 捐赠者致谢

感谢以下朋友支持(按捐赠时间排序)
1. XXX
