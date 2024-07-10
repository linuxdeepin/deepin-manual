// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

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
    QString  dbPath = Utils::getSystemManualDir().first();
    dbPath += "/search.db";
    m_fw->onChangeFile(dbPath);
    ASSERT_TRUE(m_fw->timerObj->isActive());
}
TEST_F(ut_fileWatcher, onChangeDirSlot)
{
    QString assetsPath = Utils::getSystemManualDir().first();
    m_fw->onChangeDirSlot(assetsPath);
    ASSERT_TRUE(m_fw->timerObj->isActive());
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
    addList << "ccc";
    m_fw->checkMap(mapOld, mapNow, deleteList, addList, addTime);
    ASSERT_EQ(addTime[0], "");
    ASSERT_EQ(addTime[1], "bbb");
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
    addList << "ccc";
    m_fw->checkMap(mapOld, mapNow, deleteList, addList, addTime);
    ASSERT_EQ(addTime[0], "");
    ASSERT_EQ(addTime[1], "bbb");
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
    addList << "ccc";
    m_fw->checkMap(mapOld, mapNow, deleteList, addList, addTime);
    ASSERT_EQ(addTime[0], "");
    ASSERT_EQ(addTime[1], "bbb");
}


TEST_F(ut_fileWatcher, monitorFile)
{
    QDir dir;
    dir.mkpath("./manual-assets/system/dde/dde/zh_CN/");
    dir.mkpath("./manual-assets/system/dde/dde11/zh_CN/");
    dir.mkpath("./manual-assets/application/deepin-voice-note/voice-note/zh_CN/");
    dir.mkpath("./manual-assets/professional/deepin-voice-note/zh_CN/");

    QFile file("./manual-assets/system/dde/dde/zh_CN/index.md");
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    file.close();
    QFile file1("./manual-assets/application/deepin-voice-note/voice-note/zh_CN/index.md");
    file1.open(QIODevice::ReadWrite | QIODevice::Text);
    file1.close();
    QFile file2("./manual-assets/professional/deepin-voice-note/zh_CN/index.md");
    file2.open(QIODevice::ReadWrite | QIODevice::Text);
    file2.close();

    Utils ut;
    Stub s;
    s.set(ADDR(Utils, getSystemManualDir), stub_getSystemManualDir);
    m_fw->monitorFile();
    ASSERT_EQ(m_fw->watcherObj->files()[0], "./manual-assets/application/deepin-voice-note/voice-note/zh_CN/index.md");
    ASSERT_EQ(m_fw->watcherObj->files()[1], "./manual-assets/professional/deepin-voice-note/zh_CN/index.md");

    QDir dirRemove;
    dirRemove.rmpath("./manual-assets/");
}

TEST_F(ut_fileWatcher, onTimerOut)
{
    QDir dir;
    dir.mkpath("./manual-assets/system/dde/dde/zh_CN/");
    dir.mkpath("./manual-assets/system/dde/dde11/zh_CN/");
    dir.mkpath("./manual-assets/application/deepin-voice-note/voice-note/zh_CN/");
    dir.mkpath("./manual-assets/professional/deepin-voice-note/zh_CN/");

    QFile file("./manual-assets/system/dde/dde/zh_CN/index.md");
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    file.close();
    QFile file1("./manual-assets/application/deepin-voice-note/voice-note/zh_CN/index.md");
    file1.open(QIODevice::ReadWrite | QIODevice::Text);
    file1.close();
    QFile file2("./manual-assets/professional/deepin-voice-note/zh_CN/index.md");
    file2.open(QIODevice::ReadWrite | QIODevice::Text);
    file2.close();

    Utils ut;
    Stub s;
    s.set(ADDR(Utils, getSystemManualDir), stub_getSystemManualDir);
    m_fw->onTimerOut();
    ASSERT_EQ(m_fw->watcherObj->files()[0], "./manual-assets/application/deepin-voice-note/voice-note/zh_CN/index.md");
    ASSERT_EQ(m_fw->watcherObj->files()[1], "./manual-assets/professional/deepin-voice-note/zh_CN/index.md");

    QDir dirRemove;
    dirRemove.rmdir("./manual-assets/");
//    qDebug() << "=======+>===========" << b;
}


