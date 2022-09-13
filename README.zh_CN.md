# Deepin Manual

Deepint Manual 提供 Deepin 系统和应用的用户引导.

您应该首先阅读 [Deepin应用程序规范](\ref doc/Specification).

## 依赖

### 编译依赖

* ca-certificates
* pkg-config
* libsass-dev
* libgtest-dev
* libgmock-dev
* libdtkwidget-dev
* libdtkgui-dev
* Qt5(>=5.6) with modules:
  * qt5-default
  * qt5-qmake
  * qtbase5-dev
  * qtwebengine5-dev
  * qttools5-dev
  * qttools5-dev-tools
  * qtbase5-private-dev

## 安装

### 从源代码构建

1. 确保已经安装了所有的编译依赖.
````bash
sudo apt build-dep deepin-manual
````

2. 构建:

```bash
mkdir build
cd build
cmake ..
make
```

3. 安装:

```bash
sudo make install
```

## 用法

命令行执行 `deepin-manual`

## 文档

 - 无

## 帮助

任何使用问题都可以通过以下方式寻求帮助:

* [Gitter](https://gitter.im/orgs/linuxdeepin/rooms)
* [IRC channel](https://webchat.freenode.net/?channels=deepin)
* [Forum](https://bbs.deepin.org)
* [WiKi](https://wiki.deepin.org/)

## 参与贡献

我们鼓励您报告问题并作出更改

* [开发者代码贡献指南](https://github.com/linuxdeepin/developer-center/wiki/Contribution-Guidelines-for-Developers)

## 协议

Deepin Manual 遵循协议 [GPL-3.0-or-later](LICENSE).
