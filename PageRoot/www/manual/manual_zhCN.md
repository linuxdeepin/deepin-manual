# 文档编写|WINAMP.png|

深度帮助手册所使用的是在Github的Markdown格式的基础上，扩展而来的一套格式。

## 章节与段落|VENTRILO.png|

* 一级标题：一般作为一整篇文档的大标题，显示在帮助手册的界面里。格式为`# 文档名`。一份帮助手册里只能有一个一级标题。
* 二级标题：会在首页出现的标题。格式为`## 二级标题`。二级标题可以有多个，且必须在一级标题之下。
* 三级标题：不会在首页出现，但是会在侧边栏出现的标题。格式为`### 三级标题`。三级标题可以有多个，且必须在一二级标题之下。
* 四级标题：会被索引，搜索时会被提示。但是不会出现在导航栏的标题。格式为`#### 四级标题`。四级标题位置不限。
* 引用： 用`> 被引用的文字`来显示出这样的效果。

> 被引用的文字

## 文字|SAFARI.png|

* 使用`*斜体字*`来标示*斜体字*。
* 使用`**粗体字**`来标示**粗体字**。
* 使用\`阴影字\` 标示`阴影字`。
* 使用`~~删除文本~~`,来标示~~删除文本~~。

## 列表|PICASA.png|

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

## 表格|NOTEPAD.png|

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

## 图标与插图|ITUNES.png|

推荐将所有图标、插图文件置于文档所在文件夹，或其子文件夹。

### 标题图标

TODO

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

## 链接|COUNTERSTRIKE.png|

帮助手册现在支持两种链接跳转：**在页内跳转**和**转向其它页**。

- 在页内跳转

  TODO

- 转向其它页

  TODO
