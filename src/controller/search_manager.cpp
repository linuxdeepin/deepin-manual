// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "controller/search_manager.h"
#include "controller/search_db.h"
#include "base/ddlog.h"

#include <DSysInfo>
#include <QThread>

SearchManager::SearchManager(QObject *parent)
    : QObject(parent)
    , db_(nullptr)
    , db_thread_(nullptr)
{
    qCDebug(app) << "Creating SearchManager instance";
    initSearchManager();
    qCDebug(app) << "SearchManager created successfully";
}

/**
 * @brief SearchManager::initSearchManager
 * 初始化数据库类对象， 绑定信号槽
 */
void SearchManager::initSearchManager()
{
    qCDebug(app) << "Initializing search manager";

    db_thread_ = new QThread(this);
    qCDebug(app) << "Created database thread:" << db_thread_;

    db_ = new SearchDb();
    qCDebug(app) << "Created database instance:" << db_;

    db_thread_->start();
    qCDebug(app) << "Database thread started";

    db_->moveToThread(db_thread_);

    connect(this, &SearchManager::searchAnchor, db_, &SearchDb::searchAnchor);
    connect(db_, &SearchDb::searchAnchorResult, this, &SearchManager::searchAnchorResult);
    connect(this, &SearchManager::searchContent, db_, &SearchDb::searchContent);
    connect(db_, &SearchDb::searchContentResult, this, &SearchManager::searchContentResult);
    connect(db_, &SearchDb::searchContentMismatch, this, &SearchManager::searchContentMismatch);
    connect(this, &SearchManager::updateModule, db_, &SearchDb::updateModule);
    connect(this, &SearchManager::updateDb, db_, &SearchDb::updateDb, Qt::BlockingQueuedConnection);
    connect(db_thread_, &QThread::destroyed, db_, &QObject::deleteLater);

    //初始化创建数据库SearchDb::initDb
    emit db_->initDbAsync();
    qCDebug(app) << "Search manager initialized";
}

SearchManager::~SearchManager()
{
    qCDebug(app) << "Destroying SearchManager instance";
    db_thread_->quit();
    db_thread_->wait();
    delete db_thread_;
    db_thread_ = nullptr;
    delete db_;
    db_ = nullptr;
    qCDebug(app) << "SearchManager destroyed";
}
