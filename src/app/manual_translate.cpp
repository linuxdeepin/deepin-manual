#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QHash>
#include <QPushButton>
#include <QWidget>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    // hash--> key: old name   value: new name
    const QHash<QString, QString> hash = {
        {"deepin-downloader", "downloader"},                  //下载器
        {"", "deepin-voice-recorder"},                        //录音 ***
        {"deepin-voice-note", "deepin-voice-note"},           //语音记事本
        {"deepin-terminal", "deepin-terminal"},               //终端
        {"deepin-system-monitor", "deepin-system-monitor"},   //系统监视器
        {"", "deepin-screenshot"},                            //截屏
        {"deepin-screen-capture", "deepin-screen-recorder"},  //录屏
        {"deepin-repair", "deepin-repair-tools"},             //系统修复工具
        {"", "deepin-remote-assistance"},                     //远程协助**
        {"deepin-document-viewer", "deepin-reader"},          //文档查看器**
        {"", "deepin-presentation-assistant"},                //演示助手**
        {"deepin-picker", "deepin-picker"},                   //取色器
        {"deepin-music", "deepin-music"},                     //音乐
        {"deepin-movie", "deepin-movie"},                     //影院
        {"", "deepin-log-viewer"},                            //日志收集工具**
        {"deepin-image-viewer", "deepin-image-viewer"},       //看图
        {"deepin-graphics-driver-manager", "deepin-graphics-driver-manager"},  //显卡驱动管理器
        {"deepin-font-manager", "deepin-font-manager"},                        //字体管理器
        {"deepin-text-editor", "deepin-editor"},                               //编辑器
        {"deepin-draw", "deepin-draw"},                                        //画板
        {"", "deepin-document-viewer"},                                        //文档查看器**
        {"", "deepin-devicemanager"},                                          //设备管理器**
        {"deepin-package-manager", "deepin-deb-installer"},                    //软件包安装器
        {"deepin-contact", "deepin-contacts"},                                 //联系人
        {"deepin-compressor", "deepin-compressor"},                            //归档管理器
        {"", "deepin-cloud-scan"},                                             //云扫描**
        {"", "deepin-cloud-print"},                                            //云打印**
        {"deepin-clone", "deepin-clone"},                                      //备份还原工具
        {"deepin-calculator", "deepin-calculator"},                            //计算器
        {"", "deepin-browser"},                                                //浏览器**
        {"deepin-boot-maker", "deepin-boot-maker"},   //启动盘制作工具
        {"deepin-appstore", "deepin-app-store"},      //应用商店
        {"deepin-album", "deepin-album"},             //相册
        {"deepin-print management", "dde-printer"},   //打印管理器 **
        {"deepin-file-manager", "dde-file-manager"},  //文件管理器**
        {"deepin-calendar", "dde-calendar"},          //日历
        {"dde", "dde"},                               //桌面环境
        {"", "chineseime"}                            //中文输入法**
    };

    QWidget widget;
    widget.setMinimumSize(QSize(400, 200));
    QHBoxLayout *layout = new QHBoxLayout;
    QPushButton *btn = new QPushButton("Open", &widget);
    btn->setFixedSize(80, 40);
    layout->addWidget(btn);
    QObject::connect(btn, &QPushButton::clicked, [=]() {
        QString curPath = QDir::currentPath();  //获取系统当前目录
        QString dlgTitle = "translate dir";     //对话框标题
        QString strDirPath = QFileDialog::getExistingDirectory();
        if (!strDirPath.isEmpty()) {
            QDir dirDir(strDirPath);
            for (const QString &strFile :
                 QDir(strDirPath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
                QHashIterator<QString, QString> iterator(hash);

                while (iterator.hasNext()) {
                    iterator.next();
                    if (!iterator.key().isEmpty()) {
                        if (strFile.contains(iterator.key())) {
                            bool b = dirDir.rename(strFile, iterator.value());
                            qDebug()
                                << b << " ===  old:" << strFile << "  new:" << iterator.value();
                            break;
                        }
                    } else {
                        continue;
                    }
                }
            }
        }
    });
    widget.setLayout(layout);
    widget.show();
    return app.exec();
}
