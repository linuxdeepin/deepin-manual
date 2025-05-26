// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "view/search_proxy.h"
#include "base/ddlog.h"

#include <DLog>
#include <QTimer>

SearchProxy::SearchProxy(QObject *parent)
    : QObject(parent)
{
    qCDebug(app) << "SearchProxy constructor called";
}

SearchProxy::~SearchProxy()
{
    qCDebug(app) << "SearchProxy destructor called";
}

/**
 * @brief SearchProxy::getKeyword
 * @param keyword 搜索关键字
 * JS根据页面关键字回调设置搜索框
 */
void SearchProxy::getKeyword(const QString &keyword)
{
    qCDebug(app) << "Received search keyword:" << keyword;
    emit setKeyword(keyword);
}

/**
 * @brief SearchProxy::updateSearch
 * @node 更新搜索结果
 * @param keyword
 */
void SearchProxy::updateSearch(const QString &keyword)
{
    qCDebug(app) << "Updating search for keyword:" << keyword;
    //WebWindow::onSetKeyword 设置搜索框值
    emit setKeyword(keyword);

    //延时查找数据库： 立即查找会出现无法查到刚插入数据的情况
    qCDebug(app) << "Scheduling search update in 1 second";
    QTimer::singleShot(1000, this, [ = ]() {
        qCDebug(app) << "Executing delayed search update";
        //查询结果并显示 WebWindow::onTitleBarEntered
        emit updateSearchResult();
    });

}
