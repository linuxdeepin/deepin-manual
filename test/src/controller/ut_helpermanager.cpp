/*
* Copyright (C) 2019 ~ 2020 Deepin Technology Co., Ltd.
*
* Author:     wangmingliang <wangmingliang@uniontech.com>
* Maintainer: wangmingliang <wangmingliang@uniontech.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "ut_helpermanager.h"

#include "controller/helpermanager.h"
#include <QTimer>

ut_helperManager::ut_helperManager(QObject *parent) : QObject(parent)
{

}

ut_helperManager::~ut_helperManager()
{

}

void ut_helperManager::SetUp()
{
    m_hm = new helperManager();
}

void ut_helperManager::TearDown()
{
    delete m_hm;
}

//TEST_F(ut_helperManager, iniweb)
//{
////    m_hm->initWeb();
////    sleep(1000);
//}

//TEST_F(ut_helperManager, iniinitDbConfig)
//{
//   m_hm->initDbConfig();
//}

//TEST_F(ut_helperManager, getModuleInfo)
//{
//    m_hm->getModuleInfo();
//}

//TEST_F(ut_helperManager, initConnect)
//{
//    m_hm->initConnect();
//}

//TEST_F(ut_helperManager, handleDb)
//{
//    QStringList deleteList;
//    QStringList addList;
//    QStringList addTime;
//    deleteList << "aaaa";
//    addList << "bbbb";
//    addTime << "001002";
//    m_hm->handleDb(deleteList, addList, addTime);

//}

//TEST_F(ut_helperManager, dbusSend)
//{
//    QStringList deleteList;
//    QStringList addList;
//    deleteList << "aaa";
//    addList << "bbb";
//    m_hm->dbusSend(deleteList, addList);

//}

//TEST_F(ut_helperManager, handlePriority)
//{
//    QStringList deleteList;
//    QStringList addList;
//    QStringList addTime;
//    deleteList << "aaaa";
//    addList << "bbbb";
//    addTime << "001002";
//    m_hm->handlePriority(deleteList);
//}

