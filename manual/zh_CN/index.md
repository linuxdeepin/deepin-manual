# 文档编写|../common/accessories-calculator.svg|

深度帮助手册所使用的是在Github的Markdown格式的基础上，扩展而来的一套格式。

## 章节与段落|../common/accessories-text-editor.svg|

* 一级标题：一般作为一整篇文档的大标题，显示在帮助手册的界面里。格式为`# 文档名`。一份帮助手册里只能有一个一级标题。
* 二级标题：会在首页出现的标题。格式为`## 二级标题`。二级标题可以有多个，且必须在一级标题之下。
* 三级标题：不会在首页出现，但是会在侧边栏出现的标题。格式为`### 三级标题`。三级标题可以有多个，且必须在一二级标题之下。
* 四级标题：会被索引，搜索时会被提示。但是不会出现在导航栏的标题。格式为`#### 四级标题`。四级标题位置不限。
* 引用： 用`> 被引用的文字`来显示出这样的效果。

> 被引用的文字

## 文字|../common/chmsee-icon.svg|

* 使用`*斜体字*`来标示*斜体字*。
* 使用`**粗体字**`来标示**粗体字**。
* 使用\`阴影字\` 标示`阴影字`。
* 使用`~~删除文本~~`,来标示~~删除文本~~。

## 分栏|../common/crossover.svg|

```
!←←

* 分栏的栏目内
* 也可以使用
* 各种 深度手册 提供的标签
* 没有任何限制

←!→

![1|一张壁纸](wallpaper.jpg)

→→!
```
可以显示出以下效果。

!←←

* 分栏的栏目内
* 也可以使用
* 各种 **深度手册** 提供的标签
* 没有任何限制

←!→

![1|一张壁纸](wallpaper.jpg)

→→!



## 列表|../common/deepin-boot-maker.svg|

### 无次序列表

* 北京
* 上海
* 天津
* 重庆

要生成这样的**不标次序的列表**，方法如下：
```
* 北京
* 上海
* 天津
* 重庆
```
> 用`-`来替换`*`也是可以的。

### 次序列表
1. January
2. February
3. March
4. April

### 多级列表

用
```
- 亚洲
  1. 中国
  2. 日本
- 美洲
  1. 美国
  2. 墨西哥
- 欧洲
  - 英国
  - 意大利
```
来生成
- 亚洲
  1. 中国
  2. 日本
- 美洲
  1. 美国
  2. 墨西哥
- 欧洲
  - 英国
  - 意大利

## 表格|../common/deepin-emacs.svg|

表格排版较为复杂，需要使用嵌入 HTML 的方法。

示例一，table无任何class。宽度自动。
```
<table>
    <caption>人员名录</caption>
    <tbody>
        <tr>
            <td>张三</td>
            <td>男</td>
            <td>2000-13-32</td>
        </tr>
        <tr>
            <td>李四</td>
            <td>女</td>
            <td>FFFF-FF-FF</td>
        </tr>
    </tbody>
</table>
```

<table>
    <caption>人员名录</caption>
    <tbody>
        <tr>
            <td>张三</td>
            <td>男</td>
            <td>2000-13-32</td>
        </tr>
        <tr>
            <td>李四</td>
            <td>女</td>
            <td>FFFF-FF-FF</td>
        </tr>
    </tbody>
</table>

示例二，table的class设置为block1。此时每个表格横向占用整个屏幕宽度。代码及效果如下。
```
<table class="block1">
    <caption>人员名录</caption>
    <tbody>
        <tr>
            <td>张三</td>
            <td>男</td>
            <td>2000-13-32</td>
        </tr>
        <tr>
            <td>李四</td>
            <td>女</td>
            <td>FFFF-FF-FF</td>
        </tr>
    </tbody>
</table>
```

<table class="block1">
    <caption>人员名录</caption>
    <tbody>
        <tr>
            <td>张三</td>
            <td>男</td>
            <td>2000-13-32</td>
        </tr>
        <tr>
            <td>李四</td>
            <td>女</td>
            <td>FFFF-FF-FF</td>
        </tr>
    </tbody>
</table>

示例三，table带有block2。此时每个表格横向占半屏位置，并可联排。代码及效果如下。
```
<table class="block2">
    <caption>横向并排两个</caption>
    <tbody>
        <tr>
            <td>张三</td>
            <td>男</td>
            <td>2000-13-32</td>
        </tr>
        <tr>
            <td>李四</td>
            <td>女</td>
            <td>FFFF-FF-FF</td>
        </tr>
    </tbody>
</table>
<table class="block2">
    <caption>横向并排两个</caption>
    <tbody>
        <tr>
            <td>张三</td>
            <td>男</td>
            <td>2000-13-32</td>
        </tr>
        <tr>
            <td>李四</td>
            <td>女</td>
            <td>FFFF-FF-FF</td>
        </tr>
    </tbody>
</table>
```
<table class="block2">
    <caption>横向并排两个</caption>
    <tbody>
        <tr>
            <td>张三</td>
            <td>男</td>
            <td>2000-13-32</td>
        </tr>
        <tr>
            <td>李四</td>
            <td>女</td>
            <td>FFFF-FF-FF</td>
        </tr>
    </tbody>
</table>
<table class="block2">
    <caption>横向并排两个</caption>
    <tbody>
        <tr>
            <td>张三</td>
            <td>男</td>
            <td>2000-13-32</td>
        </tr>
        <tr>
            <td>李四</td>
            <td>女</td>
            <td>FFFF-FF-FF</td>
        </tr>
    </tbody>
</table>

示例四，table带有block3。此时每个表格横向占三分之一屏幕宽度，并可联排。代码及效果如下。
```
<table class="block3">
    <caption>横向并排两个</caption>
    <tbody>
        <tr>
            <td>张三</td>
            <td>男</td>
            <td>2000-13-32</td>
        </tr>
        <tr>
            <td>李四</td>
            <td>女</td>
            <td>FFFF-FF-FF</td>
        </tr>
    </tbody>
</table>
<table class="block3">
    <caption>横向并排两个</caption>
    <tbody>
        <tr>
            <td>张三</td>
            <td>男</td>
            <td>2000-13-32</td>
        </tr>
        <tr>
            <td>李四</td>
            <td>女</td>
            <td>FFFF-FF-FF</td>
        </tr>
    </tbody>
</table>
<table class="block3">
    <caption>横向并排两个</caption>
    <tbody>
        <tr>
            <td>张三</td>
            <td>男</td>
            <td>2000-13-32</td>
        </tr>
        <tr>
            <td>李四</td>
            <td>女</td>
            <td>FFFF-FF-FF</td>
        </tr>
    </tbody>
</table>
```
<table class="block3">
    <caption>横向并排三个</caption>
    <tbody>
        <tr>
            <td>张三</td>
            <td>男</td>
            <td>2000-13-32</td>
        </tr>
        <tr>
            <td>李四</td>
            <td>女</td>
            <td>FFFF-FF-FF</td>
        </tr>
    </tbody>
</table>
<table class="block3">
    <caption>横向并排三个</caption>
    <tbody>
        <tr>
            <td>张三</td>
            <td>男</td>
            <td>2000-13-32</td>
        </tr>
        <tr>
            <td>李四</td>
            <td>女</td>
            <td>FFFF-FF-FF</td>
        </tr>
    </tbody>
</table>
<table class="block3">
    <caption>横向并排三个</caption>
    <tbody>
        <tr>
            <td>张三</td>
            <td>男</td>
            <td>2000-13-32</td>
        </tr>
        <tr>
            <td>李四</td>
            <td>女</td>
            <td>FFFF-FF-FF</td>
        </tr>
    </tbody>
</table>

### 隐形单元格
通过控制单元格的class，可以让某些单元格隐形。如下：
```
<table>
    <caption>人员名录</caption>
    <tbody>
        <tr>
            <td>张三</td>
            <td>男</td>
            <td>2000-13-32</td>
            <td class="blank"></td>
            <td>王五</td>
            <td>男</td>
            <td>2000-13-32</td>
        </tr>
        <tr>
            <td>李四</td>
            <td>女</td>
            <td>FFFF-FF-FF</td>
            <td class="blank"></td>
            <td>赵六</td>
            <td>女</td>
            <td>FFFF-FF-FF</td>
        </tr>
    </tbody>
</table>
```
<table>
    <caption>人员名录</caption>
    <tbody>
        <tr>
            <td>张三</td>
            <td>男</td>
            <td>2000-13-32</td>
            <td class="blank"></td>
            <td>王五</td>
            <td>男</td>
            <td>2000-13-32</td>
        </tr>
        <tr>
            <td>李四</td>
            <td>女</td>
            <td>FFFF-FF-FF</td>
            <td class="blank"></td>
            <td>赵六</td>
            <td>女</td>
            <td>FFFF-FF-FF</td>
        </tr>
    </tbody>
</table>

## 图标与插图|../common/deepin-game-center.svg|

推荐将所有图标、插图文件置于文档所在文件夹，或其子文件夹。

### 标题图标

标题图标以以下方式来扩展：

```
## 标题名|图片相对路径|
```
例如
```
## 一个标题|image.png|
```

> **注意** 图片路径中不可以出现空格等奇葩字符。

### 正文图标

正文中的所有图标，不论源图像尺寸、长款比例，一律被缩放到与汉字大小类似的正方形中显示。边长即行高。

### 插图

插图一般指尺寸比较大的图像。在帮助手册中，插图的高宽比总与源图像保持一致，按比例缩放是可以的。

- 以原尺寸显示插图。使用`![0|一张壁纸](wallpaper.jpg)`。效果如下：

  ![0|一张壁纸](wallpaper.jpg)

- 一张图片的宽度占满页宽。使用`![1|一张壁纸](wallpaper.jpg)`。效果如下：

  ![1|一张壁纸](wallpaper.jpg)

- 两张图片的宽度占满页宽。使用`![2|一张壁纸](wallpaper.jpg)`。两张联排的效果如下：

  ![2|一张壁纸](wallpaper.jpg) ![2|一张壁纸](wallpaper.jpg)

- 三张图片的宽度占满页宽。使用`![3|一张壁纸](wallpaper.jpg)`。三张联排的效果如下：

  ![3|一张壁纸](wallpaper.jpg) ![3|一张壁纸](wallpaper.jpg) ![3|一张壁纸](wallpaper.jpg)

## 链接|../common/deepin-installer.svg|

帮助手册现在支持两种链接跳转：**在页内跳转**和**转向其它页**。

- 在页内跳转

  点击此处可在本窗口内回滚[回到插图](#插图)。

  ```
  点击此处可在本窗口内回滚[回到插图](#插图) 。
  ```

- 转向其它深度帮助页

  可以像这样，在新开的窗口中打开其它程序的帮助页。

  [在新窗口打开其它程序的帮助页面](dman://license#Others)。

  ```
  [在新窗口打开其它程序的帮助页面](dman://license#Others)
  ```

- 实现其它功能的特殊链接

  暂未实现。

## 文档组织结构|../common/deepin-movie.svg|

深度帮助手册需要以特定的结构组织放置。要求如下：

1. 主文档名必须名为`index.md`，区分大小写。
2. 文档内图片引用必须以**主文档**为起点计算的相对路径。
3. 应用程序安装文档时，必须安装到`/usr/share/dman/[应用程序名称]/[语言编码]`下。


## 模糊匹配词典|../common/deepin-music-player.svg|

深度帮助手册提供一项功能，可以帮助文档编写者更好地服务用户，那就是自定义模糊词典。
此功能适用于所有语言，包括已经提供了中文分词功能的简体中文。

在编写文档的时候，在文档路径下编写一个名为`synonym.txt`的文本文件。此文件需要以UTF-8格式编码。

文件格式要求如下：

1. 每个模糊概念占一行。
2. 每行的模糊概念词用竖杠分开。
3. 两个不同行内，不可以出现相同的模糊词。
4. (针对字母语言)模糊词不区分大小写。
5. 区分“连字符”等特殊符号。

例如：

```
Turn off|Power off
CPU|处理器|Central Processing Unit
显示器|监视器|屏幕|显示屏
```

此文件为可选内容，文档编写者可以根据本语言的情况灵活编写。
例如，在中文文档里加入简繁体的定义；在各个语言的文档里，加入同义词的定义。

为每个应用编写的同义词典，不应用于其它程序的帮助手册。

## 其它|../common/deepin-screenshot.svg|

暂无。

## 安全性|../common/deepin-software-center.svg|

请注意目前的版本允许页面中执行脚本，在发布Markdown文档时请检查没有恶意代码在内。

## 打包者须知|../common/deepin-terminal.svg|

见 git 根目录的README.md。

## 帮助手册开发者须知|../common/deepin-translator.svg|

见 git 根目录的README.md。

## 其它应用开发者须知|../common/driver-manager.svg|

见 git 根目录的README.md。
