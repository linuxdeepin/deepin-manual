#ifndef THEME_PROXY_H
#define THEME_PROXY_H

#include <dtkgui_global.h>
#include <DGuiApplicationHelper>
#include <QObject>

namespace dman {
class ThemeProxy : public QObject
{
    Q_OBJECT
public:
    explicit ThemeProxy(QObject *parent = nullptr);
    ~ThemeProxy() override;
signals:
    void themeChange(const QString &themetype);
public slots:
    void slot_ThemeChange();
    QString getTheme() const;
};
} // namespace dman
#endif // THEME_PROXY_H
