# TarsServant

从[腾讯Tars](https://github.com/Tencent/Tars)框架中剥离出来的Servant基础代码（C++），用于基于Tars框架的RPC服务开发。

从源分支`2018/7/5`提交`7d6da5b85848f8d24130fc240e7432ef2f5fcb3d`剥离而来，并做了如下调整：

1. 重新组织代码结构

2. 清理了语言上不规范的用法

    1. 去掉了头文件中的`using namespace std`

    2. 去掉了`hash_map`的使用

    3. 去掉了头文件中对虚析构函数的实现

    4. 其他调整

## 使用方式

1. 作为`submodule`引入本仓库

2. 在CMake中通过`add_subdirectory`的方式引用本项目
