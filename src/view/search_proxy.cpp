/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "view/search_proxy.h"

#include <DLog>

SearchProxy::SearchProxy(QObject *parent)
    : QObject(parent)
{
}

SearchProxy::~SearchProxy()
{
}

/**
 * @brief SearchProxy::getKeyword
 * @param keyword 搜索关键字
 * JS根据页面关键字回调设置搜索框
 */
void SearchProxy::getKeyword(const QString &keyword)
{
    qDebug() << "-->" << keyword;
    emit setKeyword(keyword);
}

/**
 * @brief SearchProxy::updateSearch
 * @node 更新搜索结果
 * @param keyword
 */
void SearchProxy::updateSearch(const QString &keyword)
{
    qDebug() << Q_FUNC_INFO;
    //WebWindow::onSetKeyword 设置搜索框值
    emit setKeyword(keyword);

    //延时查找数据库： 立即查找会出现无法查到刚插入数据的情况
    QTimer::singleShot(1000, this, [ = ]() {
        //查询结果并显示 WebWindow::onTitleBarEntered
        emit updateSearchResult();
    });

}
