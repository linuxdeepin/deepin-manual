#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QPushButton>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    // hash--> key: old name   value: new name
    const QHash<QString, QString> hash = {
        {"image-viewer", "deepin-image-viewer"}, //看图
        {"album", "deepin-album"}, //相册
        {"draw", "deepin-draw"}, //画板
        {"screen-capture", "deepin-screen-recorder"}, //截屏录屏
        {"music", "deepin-music"}, //音乐
        {"movie", "deepin-movie"}, //影院
        {"calendar", "dde-calendar"}, //日历
        {"voice-note", "deepin-voice-note"}, //语音记事本
        {"archive-manager", "deepin-compressor"}, //归档管理器
        {"font-manager", "deepin-font-manager"}, //字体管理器
        {"editor", "deepin-editor"}, //编辑器
        {"calculator", "deepin-calculator"}, //计算器
        {"document-viewer", "deepin-reader"}, //文档查看器
        {"package-installer", "deepin-deb-installer"}, //软件包安装器
        {"system-monitor", "deepin-system-monitor"}, //系统监视器
        {"log-viewer", "deepin-log-viewer"}, //日志收集工具
        {"terminal", "deepin-terminal"}, //终端
        {"app-store", "deepin-app-store"}, //应用商店
        //{"education-center", "deepin-app-store"}, //应用商店家庭版教育中心
        {"support", "uos-service-support"}, //服务与支持
        {"security-center", "deepin-defender"}, //安全中心
        {"phone-master", "deepin-phone-assistant"}, //手机助手
        {"contacts", "deepin-contacts"}, //联系人
        {"browser", "org.deepin.browser"}, //浏览器
        {"email-client", "deepin-mail"}, //邮箱
        {"downloader", "uos-downloadmanager"}, //下载器
        {"chinese-input-method", "chineseime-setting"}, //中文输入法
        {"print-manager", "dde-printer"}, //打印管理器
        {"scan-manager", "org.deepin.scanner"}, //扫描管理器
        {"device-manager", "deepin-devicemanager"}, //设备管理器
        {"graphics-driver-manager", "deepin-graphics-driver-manager"}, //显卡驱动管理器
        {"camera", "deepin-camera"}, //相机
        {"repair-tools", "deepin-repair-tools"}, //系统修复工具
        {"boot-maker", "deepin-boot-maker"}, //启动盘制作工具
        {"clone", "deepin-clone"}, //备份还原工具
        {"file-manager", "dde-file-manager"}, //文件管理器
        {"disk-manager", "deepin-diskmanager"}, //磁盘管理器

        {"voice-recorder", "deepin-voice-recorder"}, //录音 **
        {"remote-assistance", "deepin-remote-assistance"}, //远程协助**
        {"presentation-assistant", "deepin-presentation-assistant"}, //演示助手**
        {"picker", "deepin-picker"}, //取色器**
        {"cloud-scan", "deepin-cloud-scan"}, //云扫描**
        {"cloud-print-configurator", "deepin-cloud-print-configurator"}, //云打印**
    };

    QWidget widget;
    widget.setMinimumSize(QSize(400, 200));
    QHBoxLayout *layout = new QHBoxLayout;
    QPushButton *btn = new QPushButton("Open", &widget);
    btn->setFixedSize(80, 40);
    layout->addWidget(btn);
    QObject::connect(btn, &QPushButton::clicked, [ = ]() {
        QString curPath = QDir::currentPath();  //获取系统当前目录
        QString dlgTitle = "translate dir";     //对话框标题
        QString strDirPath = QFileDialog::getExistingDirectory(nullptr, dlgTitle, curPath);
        if (!strDirPath.isEmpty()) {
            QDir dirDir(strDirPath);
            for (QString &strFile :
                    QDir(strDirPath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
                QHashIterator<QString, QString> iterator(hash);
                while (iterator.hasNext()) {
                    iterator.next();
                    if (!iterator.key().isEmpty()) {
                        if (strFile.contains(iterator.key())) {
                            bool b = dirDir.rename(strFile, iterator.value());
                            qDebug() << b << " ===  old:" << strFile << "  new:" << iterator.value();
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
