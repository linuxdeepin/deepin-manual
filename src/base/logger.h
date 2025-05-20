// SPDX-FileCopyrightText: 2025 UnionTech Software Technology Co., Ltd.
//
// SPDX-Liteense-Identifier: GPL-3.0-or-later

#include <QObject>
#include <dtkcore_global.h>

DCORE_BEGIN_NAMESPACE
class DConfig;
DCORE_END_NAMESPACE

class MLogger : public QObject
{
    Q_OBJECT
public:
    explicit MLogger(QObject *parent = nullptr);
    ~MLogger();

    static void initLogger();

    inline QString rules() const { return m_rules; }
    void setRules(const QString &rules);

private:
    void appendRules(const QString &rules);

private:
    QString m_rules;
    Dtk::Core::DConfig *m_config;
};
