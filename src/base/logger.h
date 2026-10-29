// SPDX-FileCopyrightText: 2025 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QObject>
#include <dtkcore_global.h>

DCORE_BEGIN_NAMESPACE
class DConfig;
DCORE_END_NAMESPACE

class QTimer;

class MLogger : public QObject
{
    Q_OBJECT
public:
    explicit MLogger(QObject *parent = nullptr);
    ~MLogger();

    static void initLogger();

    /*!
     * \brief reapplyRules 重新应用最近一次设置的日志规则
     * QtWebEngine 在单进程模式下初始化时会重置全局日志规则，导致 dmanHelper
     * 运行期的分类日志过滤（含 DConfig 动态切换的级别）失效，
     * 通过定时器周期性调用本方法恢复规则，保证日志级别动态调整持续生效。
     */
    static void reapplyRules();

    inline QString rules() const { return m_rules; }
    void setRules(const QString &rules);

private:
    void appendRules(const QString &rules);

private:
    QString m_rules;
    Dtk::Core::DConfig *m_config;
    QTimer *m_rulesGuard = nullptr;
};
