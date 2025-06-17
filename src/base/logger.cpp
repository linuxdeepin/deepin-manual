// SPDX-FileCopyrightText: 2025 UnionTech Software Technology Co., Ltd.
//
// SPDX-Liteense-Identifier: GPL-3.0-or-later

#include "logger.h"
// #include "dtkcore_global.h"
// #include "qglobal.h"
#include "ddlog.h"
#include <QLoggingCategory>
#include <QObject>

#include <DConfig>

DCORE_USE_NAMESPACE

MLogger::MLogger(QObject *parent)
    : QObject(parent), m_rules(""), m_config(nullptr)
{
    QByteArray logRules = qgetenv("QT_LOGGING_RULES");
    // qunsetenv 之前一定不要有任何日志打印，否则取消环境变量设置不会生效
    qunsetenv("QT_LOGGING_RULES");
    qCDebug(app) << "QT_LOGGING_RULES unset";

    // set env
    m_rules = logRules;
    qCDebug(app) << "Initial rules set from environment:" << m_rules;

    // set dconfig
    qCDebug(app) << "Creating DConfig instance";
    m_config = DConfig::create("org.deepin.manual", "org.deepin.dman");
    logRules = m_config->value("log_rules").toByteArray();
    qCDebug(app) << "DConfig log_rules value:" << logRules;
    appendRules(logRules);
    setRules(m_rules);

    // watch dconfig
    qCDebug(app) << "Connecting DConfig valueChanged signal";
    connect(m_config, &DConfig::valueChanged, this, [this](const QString &key) {
        qCCritical(app) << "value changed:" << key;
        if (key == "log_rules") {
            setRules(m_config->value(key).toByteArray());
        }
    });
}

MLogger::~MLogger()
{
    m_config->deleteLater();
}

void MLogger::initLogger()
{
    qCDebug(app) << "Initializing logger configuration";
    // set log format and register console and file appenders
    const QString logFormat = "%{time}{yy-MM-ddTHH:mm:ss.zzz} [%{type:-7}] [%{category}] <%{function}:%{line}> %{message}";
    qCDebug(app) << "Setting log format:" << logFormat;
    DLogManager::setLogFormat(logFormat);

    DLogManager::registerJournalAppender();
    DLogManager::registerFileAppender();

#ifdef QT_DEBUG
    DLogManager::registerConsoleAppender();
#endif

    qCDebug(app) << "Logger initialization completed";
}

void MLogger::setRules(const QString &rules)
{
    qCDebug(app) << "Setting logging rules, original rules:" << rules;
    auto tmpRules = rules;
    m_rules = tmpRules.replace(";", "\n");
    qCDebug(app) << "Processed rules:" << m_rules;
    QLoggingCategory::setFilterRules(m_rules);
    qCDebug(app) << "Logging rules applied successfully";
}

void MLogger::appendRules(const QString &rules)
{
    qCDebug(app) << "Appending logging rules, new rules:" << rules;
    QString tmpRules = rules;
    tmpRules = tmpRules.replace(";", "\n");
    qCDebug(app) << "Processed new rules:" << tmpRules;
    
    auto tmplist = tmpRules.split('\n');
    qCDebug(app) << "Existing rules:" << m_rules;

    for (int i = 0; i < tmplist.count(); i++) {
        if (m_rules.contains(tmplist.at(i))) {
            qCDebug(app) << "Rule already exists, skipping:" << tmplist.at(i);
            tmplist.removeAt(i);
            i--;
        }
    }

    if (tmplist.isEmpty()) {
        qCDebug(app) << "No new rules to append";
        return;
    }

    qCDebug(app) << "New rules to append:" << tmplist;
    m_rules.isEmpty() ? m_rules = tmplist.join("\n")
                      : m_rules += "\n" + tmplist.join("\n");

    qCDebug(app) << "Final rules after append:" << m_rules;
}
