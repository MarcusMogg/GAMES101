# GAMES101

## 如何安装依赖

众所周知，C++的依赖安装是一件很头疼的事情，为了简单起见，使用[vcpkg](https://github.com/microsoft/vcpkg)

vcpkg 使用指南见https://github.com/microsoft/vcpkg/blob/master/README_zh_CN.md#%E5%9C%A8-cmake-%E4%B8%AD%E4%BD%BF%E7%94%A8-vcpkg

安装

```shell
.\vcpkg\vcpkg install opencv:x64-windows
```

### 遇到的一些坑点

1. vcpkg依赖vs的英文包，默认只装了中文包
2. 网络问题，多重试几次
3. 安装包时注意vs的生成类型要和vcpkg安装的一致，比如x64和x86

## 作业进度

[作业1](assignment1/README.md)
[作业2](assignment2/README.md)
[作业3](assignment3/README.md)
[作业4](assignment4/README.md)
[作业5](assignment5/README.md)
[作业6](assignment6/README.md)