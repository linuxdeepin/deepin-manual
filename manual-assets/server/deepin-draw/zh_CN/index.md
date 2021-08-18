# 画板|../common/deepin-draw.svg|

## 概述

画板是一款简单的绘图工具，支持旋转、裁剪、翻转、添加文字、绘制形状等功能。您可以对本地图片进行简单编辑，也可以绘制一张简单图片。

![1|mian](jpg/main.png)

## 使用入门

通过以下方式运行或关闭画板，或者创建画板的快捷方式。

### 运行画板

1. 单击任务栏上的启动器图标 ![deepin-launcher](icon/deepin-launcher.svg)，进入启动器界面。
2. 上下滚动鼠标滚轮浏览或通过搜索，找到画板图标 ![draw](icon/deepin-draw.svg)，单击运行。
3. 右键单击 ![draw](icon/deepin-draw.svg)，您可以：
 - 单击 **发送到桌面**，在桌面创建快捷方式。
 - 单击 **发送到任务栏**，将应用程序固定到任务栏。
 - 单击 **开机自动启动**，将应用程序添加到开机启动项，在电脑开机时自动运行该应用。

>![tips](icon/tips.svg)窍门：您可以在控制中心中将画板设置为默认的图片查看程序，具体操作请参考 [默认程序设置](dman:///dde#默认程序设置)。

### 关闭画板

- 在画板界面单击 ![close_icon](icon/close_icon.svg)，退出画板。
- 在任务栏右键单击 ![draw](icon/deepin-draw.svg)，选择 **关闭所有**，退出画板。
- 在画板界面单击 ![icon_menu](icon/icon_menu.svg)，选择 **退出**，退出画板。

### 查看快捷键

在画板界面，使用快捷键 **Ctrl + Shift + ?** 打开快捷键预览界面。熟练地使用快捷键，将大大提升您的操作效率。

![1|hotkey](jpg/hotkey.png)


## 常用功能

使用画板可以处理导入的图片，也可以自由绘制图形，并可以导出多种图片格式。

### 新建标签页

![1|mian](jpg/open.png)

- 在画板界面上，单击 ![menu](icon/icon_menu.svg) > **新建**，新建标签页。
- 当窗口有两个或两个以上标签页时，也可以单击按钮 ![add](icon/add.svg)， 新建标签页。

### 关闭标签页

- 鼠标左键单击标签页，单击按钮 ![close_icon](icon/close_icon.svg)，关闭当前标签页。
- 鼠标右键单击标签页，选择 **关闭标签页** 或 **关闭其他标签页**。


### 打开图片

1. 在画板界面，单击 ![import](icon/Import_normal.svg) 或 ![menu](icon/icon_menu.svg) > **打开** 。
2. 选择想要导入的图片，单击 **打开** 导入图片。

   目前仅支持导入30张图片，且支持的图片格式为JPG、PNG、TIF、BMP和DDF。



### 导出图片

1. 在画板界面，单击 ![menu](icon/icon_menu.svg) > **导出**。
2. 设置文件名、路径、文件格式、图片质量等参数。
3. 单击 **保存**。



### 保存图片

1. 在画板界面，单击 ![menu](icon/icon_menu.svg) > **保存** 或 ![menu](icon/icon_menu.svg) > **另存为**。
2. 设置保存的文件名、格式。
3. 单击 **保存**。

> ![notes](icon/notes.svg)说明：填写保存文件名时可以省略后缀名，会自动保存为文件格式栏选择的格式。

### 打印图片

1. 在画板界面，单击 ![menu](icon/icon_menu.svg) > **打印**。
2. 选择打印机，设置打印参数。
3. 单击 **打印**。

> ![notes](icon/notes.svg)说明：单击 **高级设置** 可以自定义打印效果。



## 绘画工具

使用画板的绘画工具，可以充分发挥想象力和创造力，自由绘制各种图形。


### 形状工具

1. 在画板界面，单击 ![rectangle](icon/rectangle_normal.svg) 或 ![oval](icon/oval_normal.svg) 或 ![triangle](icon/triangle_tool_normal.svg) 或 ![star](icon/drawstar.svg) 或 ![hexagon](icon/hexagon_tool_normal.svg)。
2. 您可以设置图形的参数：
 - 单击 **填充** 设置图形的填充颜色和透明度。
 - 单击 **描边** 设置图形的描边颜色和透明度。
 - 在粗细值下拉框中设置图形的描边粗细。
 - 设置锚点数（取值范围为3-50个）和半径值（取值范围为0%-100%），仅适用于星型。
 - 设置侧边数（取值范围为4-10条），仅适用于多边形。
   
   > ![notes](icon/notes.svg)说明：锚点数、半径值和侧边数既可以通过单击按钮 ![icon](icon/up.svg) 或 ![icon](icon/down.svg) 来调节，也可以选中数值后使用键盘上 ![Up](icon/Up.svg)或 ![down](icon/Down.svg) 键或者滚动鼠标来调节。

3. 在画板区域拖动鼠标来绘制形状。

> ![tips](icon/tips.svg)窍门：在使用 ![rectangle](icon/rectangle_normal.svg) 或 ![oval](icon/oval_normal.svg) 或 ![triangle](icon/triangle_tool_normal.svg) 或 ![star](icon/drawstar.svg) 或 ![hexagon](icon/hexagon_tool_normal.svg) 绘制图形时，配合键盘上的 **Shift** 或 **Shift + Alt** 键可以画出正方形、正圆、正三角、正五角星、正多边形。

![1|shape](jpg/shape1.png)

![1|shape](jpg/star.png)

![1|shape](jpg/shape3.png)



### 直线/画笔工具


1. 在画板界面，单击 ![line](icon/line_normal.svg) 或 ![line](icon/pencil_normal.svg)。
2. 您可以设置直线或画笔的参数：
   - 单击 **颜色** 设置画笔的颜色和透明度。
   - 在粗细值下拉框中设置画笔的粗细值。
   - 设置线条的起点和终点样式。
3. 在画板区域拖动鼠标来绘制线条。

![1|pencil](jpg/linepencil.png)

### 文本工具

1. 在画板界面，单击 ![text](icon/text_normal.svg)。
2. 您可以设置文本样式：
   - 单击 **颜色** 设置文本的填充颜色和透明度。
   - 在 **字体** 下拉框中选择字体样式。
   - 设置常规字体或加粗字体。
   - 在 **字号**下拉框中选择字号大小。
3. 在画板区域单击鼠标后在文本框中输入文字。

>![tips](icon/tips.svg)窍门：您也可以使用快捷键调节字号大小：当字体处于编辑状态时，单击属性栏上的字号大小，按下键盘上的 ![Up](icon/Up.svg) 键或 ![Down](icon/Down.svg) 键调节大小。

![1|text](jpg/text.png)


### 模糊工具

1. 在画板界面，单击 ![blur](icon/blur_normal.svg)。
2. 选择模糊类型。
3. 选择涂抹区域的粗细值。
4. 在画板区域拖动鼠标来涂抹需要模糊的区域。

![1|blur](jpg/blur.png)

![1|mosaic](jpg/mosaic.png)

## 编辑功能
使用画板的编辑功能可对图形进行复制、裁剪、旋转等操作；也可对图层和文字进行调整。

### 选择

绘制图形后，可以进行以下操作：

- 对已绘制的图形、文字、模糊区域进行选择。
- 支持拖动鼠标框选图形，框选中的图形变为选择状态。
- 处于选择状态中的图形，可拖拽边缘调整其大小。
- 支持按住 **Shift** 键，选择多个图形。

> ![notes](icon/notes.svg)说明：单击画板空白区域，取消当前已选图形。


### 裁剪

1. 在画板界面，单击 ![cut](icon/cut_normal.svg) 进入裁剪模式。
2. 选择裁剪模式，选定需要裁剪的区域。
  - 自由：自定义截取，手动输入尺寸或拖动裁剪边框来调整需要裁剪的区域。
  - 原始：按照原图比例截取，如1:1、2:3、8:5、 16:9。

3. 按 **Enter** 键或单击属性栏上的图标 ![check](icon/check.svg)，裁剪画布。

![1|cut](jpg/cut.png)


### 翻转

1. 在画板界面，选择已导入的图片。
2. 单击 ![flip](icon/filp_Vertical_normal.svg) 或 ![flip](icon/flip_Horizontal_normal.svg) 将图片垂直翻转或水平翻转。


### 旋转

1. 在画板界面，选中图片。
2. 单击 ![rotate](icon/rotate_right_normal.svg) 或 ![rotate](icon/rotate_left_normal.svg) 将图片顺时针或逆时针旋转90°，或鼠标左键按住旋转按钮 ![icon](icon/icon_rotate.svg) 来旋转图片。

![1|rotate](jpg/rotate.png)



### 自适应画布

1. 在画板界面，选择已导入的图片。
2. 单击 ![auto_fit](icon/auto_fit.svg)，根据图片自适应画布大小。
   - 若选择单张图片，根据图片的宽高来调整画布大小。
   - 若选择多张图片，根据最大边界范围调整画布大小。



### 调整图层

1. 在画板界面，选择一个图形。
2. 单击鼠标右键选择 **图层**，在其下拉列表选择 **向上一层**、**向下一层**、**置于顶层** 或 **置于底层** 来调整图层顺序。

![1|layer](jpg/layer.png)

### 对齐图层

1. 在画板界面，选中一个或多个图形。
2. 单击鼠标右键选择 **对齐**，在其下拉列表选择 **左对齐**、**水平居中**、**右对齐**、**顶对齐**、**垂直居中**、**底对齐**、**水平等间矩** 或 **垂直等间矩** 来调整图层对齐方式。

>![notes](icon/notes.svg)说明：
>- 仅选中一个图形进行操作时，图层则与画布对齐。
>- 选中三个或三个以上的图形时，才能进行“水平等间矩”和“垂直等间矩”的操作。

![1|align1](jpg/align1.png)

### 对齐文本

1. 在画板界面，单击 ![text](icon/text_normal.svg)，调整文本框大小。
2. 选中文本内容后单击鼠标右键选择 **左对齐**、**右对齐**、**居中对齐** 来对齐文本。

![1|layer](jpg/align.png)


### 复制和粘贴

1. 在画板界面，选中需要复制的图形。
2. 单击鼠标右键选择 **复制** 或使用快捷键 **Ctrl + C** 将图形复制到剪贴板。
3. 单击鼠标右键选择 **粘贴** 或使用快捷键 **Ctrl + V** 将图形粘贴到画板上。

![1|copy](jpg/copy.png)

### 删除

1. 在画板界面，选择一个图形。
2. 单击鼠标右键选择 **删除** 或按下键盘上的 **Delete **键将选中的图形删除。

![1|delete](jpg/delete.png)

## 主菜单

在主菜单中，您可以新建标签页、[打开图片](#打开图片)、[导出图片](#导出图片)、保存图片、打印图片、切换窗口主题、查看帮助手册，了解画板的更多信息。

### 新建

您可以通过新建添加一个标签页。

1. 在画板界面，单击 ![icon_menu](icon/icon_menu.svg)。
2. 选择 **新建**，添加一个标签页。

### 主题

窗口主题包含浅色主题、深色主题和系统主题。

1. 在画板界面，单击 ![icon_menu](icon/icon_menu.svg)。
2. 选择 **主题**，选择一个主题颜色。

### 帮助

查看帮助手册，进一步了解和使用画板。

1. 在画板界面，单击 ![icon_menu](icon/icon_menu.svg)。
2. 选择 **帮助**。
3. 查看画板的帮助手册。




### 关于

1. 在画板界面，单击 ![icon_menu](icon/icon_menu.svg)。
2. 选择 **关于**。
3. 查看画板的版本和介绍。



### 退出

1. 在画板界面，单击 ![icon_menu](icon/icon_menu.svg)。
2. 选择 **退出**。

<div class="version-info"><span>文档更新时间: 2021-08-18</span><span> 版本: 5.9</span></div>