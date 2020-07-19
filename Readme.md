# 共享书柜客户端

## 1.简介

本程序为共享书柜控制面板客户端，主页效果如下（演示视频见demo文件夹）：

![](images/main_page.png)

## 2.环境要求

1. 界面库：Qt 5.10.1

   从[官网](https://www.qt.io/)下载安装包，或者[备用下载](https://pan.baidu.com/s/14h2cb8roLD9HscljsF-iVw?errno=0&errmsg=Auth%20Login%20Sucess&&bduss=&ssnerror=0&traceid=)，提取码：zerh。给下载的`*.run`文件添加执行权限，运行进入图形安装向导安装即可。

   ```shell
   sudo chmod +x qt-opensource-linux-x64-5.10.1.run
   ./qt-opensource-linux-x64-5.10.1.run
   ```

2. 人脸识别库:[face_recognition](https://github.com/ageitgey/face_recognition)

   修改pip镜像源为清华镜像
   ```shell
   mkdir ~/.pip
   vim ~/.pip/pip.conf
   # pip.conf文件内容如下：
   [global]
   index-url = https://pypi.tuna.tsinghua.edu.cn/simple
   ```
   之后再`pip install face_recognition`即可安装各种依赖(或临时使用镜像`pip install -i https://pypi.tuna.tsinghua.edu.cn/simple face_recognition`)。若安装dlib库时出现问题，可能是`gcc`或`g++`版本的问题，在`pip install recognition`之前，可以指定`gcc`或`g++`的位置
   ```shell
   export CC=/usr/local/bin/gcc
   export CXX=/usr/local/bin/g++ 
   ```

## 3.编译与运行

- 确保face_recognition正确安装后，执行`./config.sh`配置程序调用`face_recognition`命令时的环境变量
- 在Qt Creator中打开ShareBooks.pro，点击![](images/run.png)调试即可