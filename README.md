# Deepin Manual

Deepin Manual provides user guides of the system and applications on Deepin.

## Dependencies

### Build dependencies

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

## Installation

### Build from source code

1. Make sure you have installed all dependencies.
````
sudo apt build-dep deepin-manual
````

2. Build:

````
$ mkdir build
$ cd build
$ cmake ..
$ make
````

3. Install:

````
$ sudo make install
````

## Getting help

Any usage issues can ask for help via

* [Gitter](https://gitter.im/orgs/linuxdeepin/rooms)
* [IRC channel](https://webchat.freenode.net/?channels=deepin)
* [Forum](https://bbs.deepin.org)
* [WiKi](https://wiki.deepin.org/)

## Getting involved

We encourage you to report issues and contribute changes

* [Contribution guide for developers](https://github.com/linuxdeepin/developer-center/wiki/Contribution-Guidelines-for-Developers-en). (English)

## License

Deepin Manual is licensed under [GPL-3.0-or-later](LICENSE).
