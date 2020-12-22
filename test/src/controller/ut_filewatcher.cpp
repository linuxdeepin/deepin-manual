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
#include "ut_filewatcher.h"
#include "base/utils.h"

#include "controller/filewatcher.h"
#include "src/third-party/stub/stub.h"
#include "base/utils.h"

ut_fileWatcher::ut_fileWatcher()
{

}

void ut_fileWatcher::SetUp()
{
    m_fw = new fileWatcher();
}

void ut_fileWatcher::TearDown()
{
    delete m_fw;
}

TEST_F(ut_fileWatcher, onChangeFile)
{
    QString  dbPath = Utils::getSystemManualDir();
    dbPath += "/search.db";
    m_fw->onChangeFile(dbPath);

}
TEST_F(ut_fileWatcher, onChangeDirSlot)
{
    QString assetsPath = Utils::getSystemManualDir();
    m_fw->onChangeDirSlot(assetsPath);
    ASSERT_TRUE(m_fw->timerObj->isActive());
}
TEST_F(ut_fileWatcher, onTimerOut)
{
    m_fw->onTimerOut();
}
TEST_F(ut_fileWatcher, setFileMap)
{
    QMap<QString, QString> map;
    map.insert("A", "a");
    map.insert("B", "b");
    m_fw->setFileMap(map);
    ASSERT_EQ(m_fw->mapOld, map);
}

TEST_F(ut_fileWatcher, checkMap)
{
    QMap<QString, QString> mapOld;
    QMap<QString, QString> mapNow;

    QStringList deleteList;
    QStringList addList;
    QStringList addTime;

    mapOld.insert("111", "aaa");
    mapNow.insert("222", "bbb");
    addList<< "ccc";
    m_fw->checkMap(mapOld, mapNow, deleteList, addList, addTime);
}

TEST_F(ut_fileWatcher, checkMap2)
{
    QMap<QString, QString> mapOld;
    QMap<QString, QString> mapNow;

    QStringList deleteList;
    QStringList addList;
    QStringList addTime;

    mapOld.insert("111", "aaa");
    mapNow.insert("222", "bbb");
    mapNow.insert("111", "aaa");
    addList<< "ccc";
    m_fw->checkMap(mapOld, mapNow, deleteList, addList, addTime);
}

TEST_F(ut_fileWatcher, checkMap3)
{
    QMap<QString, QString> mapOld;
    QMap<QString, QString> mapNow;

    QStringList deleteList;
    QStringList addList;
    QStringList addTime;

    mapOld.insert("111", "aaa");
    mapNow.insert("111", "bbb");
    addList<< "ccc";
    m_fw->checkMap(mapOld, mapNow, deleteList, addList, addTime);
}

QString stub_getSystemManualDir()
{
    return "s";
}
TEST_F(ut_fileWatcher, monitorFile)
{
    Utils ut;

    Stub s;
    s.set(ADDR(Utils, getSystemManualDir), stub_getSystemManualDir);
    m_fw->monitorFile();
}


