#include "ut_command.h"

#include "base/command.h"
#include "../third-party/stub/stub.h"

#include <QProcess>
#include <QIODevice>

ut_command_test::ut_command_test()
{

}

int cntemp = 0;

bool stub_isempty()
{
    if(cntemp == 0)
    {
        cntemp++;
        return false;
    }
    return  true;
}


void stub_waitFinish()
{
    return ;
}

QString stub_errorString()
{
    return "nostart";
}

TEST(ut_command_test, RunScriptFile)
{
    //设置当前工作目录为 manual/src/web/js
    QString jsPath = DMAN_SEARCH_WORK_DIR;
    jsPath += "/toManual/js";
    Stub st;
    st.set((void (QProcess::*)(QIODevice::OpenMode))ADDR(QProcess, start), stub_waitFinish);
    st.set(ADDR(QProcess, waitForFinished), stub_waitFinish);
    st.set(ADDR(QProcess, errorString), stub_errorString);
    QDir dir(jsPath);
    QStringList nameFilters;
    nameFilters << "*.jsx";
    QStringList list  = dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
    qDebug() << list;
    dman::RunScriptFile(list);

    st.set((bool (QStringList::*)() const)ADDR(QStringList, isEmpty), stub_isempty);
    dman::RunScriptFile(list);
}

TEST(ut_command_test, RunScriptFile2)
{
    QStringList list{"app/src/app.js"};
    dman::RunScriptFile(list);
}

TEST(ut_command_test, RunScriptFile3)
{
    //设置当前工作目录为 manual/src/web/js
    QString jsPath = DMAN_SEARCH_WORK_DIR;
    jsPath += "/toManual/js";
    Stub st;

    st.set(ADDR(QProcess, waitForFinished), stub_waitFinish);
    st.set((void (QProcess::*)(QIODevice::OpenMode))ADDR(QProcess, start), stub_waitFinish);
    st.set(ADDR(QProcess, errorString), stub_errorString);
    QDir dir(jsPath);
    QStringList nameFilters;
    nameFilters << "*.jsx";
    QStringList list  = dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
    qDebug() << list;
    QString output;
    QString err;
    dman::RunScriptFile(list, output, err);
    cntemp = 0;

    st.set((bool (QStringList::*)() const)ADDR(QStringList, isEmpty), stub_isempty);

    dman::RunScriptFile(list, output, err);
}


TEST(ut_command_test, RunScriptFile4)
{
    QStringList list{"app/src/app.js"};
    QString output;
    QString err;
    Stub st;
      st.set((void (QProcess::*)(QIODevice::OpenMode))ADDR(QProcess, start), stub_waitFinish);
    st.set(ADDR(QProcess, waitForFinished), stub_waitFinish);
    st.set(ADDR(QProcess, errorString), stub_errorString);
    dman::RunScriptFile(list, output, err);
}

TEST(ut_command_test, SpawnCmd)
{
    //设置当前工作目录为 manual/src/web/js
    QString jsPath = DMAN_SEARCH_WORK_DIR;
    jsPath += "/js";

    QDir dir(jsPath);
    QStringList nameFilters;
    nameFilters << "*.jsx";
    QStringList list  = dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
    QString output;
    Stub st;
    st.set(ADDR(QProcess, waitForFinished), stub_waitFinish);
      st.set((void (QProcess::*)(QIODevice::OpenMode))ADDR(QProcess, start), stub_waitFinish);
      st.set(ADDR(QProcess, errorString), stub_errorString);
   dman::SpawnCmd("/bin/bash", list, output);
}

TEST(ut_command_test, SpawnCmd2)
{
    //设置当前工作目录为 manual/src/web/js
    QString jsPath = DMAN_SEARCH_WORK_DIR;
    jsPath += "/js";

    QDir dir(jsPath);
    QStringList nameFilters;
    nameFilters << "*.jsx";


    QStringList list{"app/src/app.js"};
    QString output;
    QString err;
    Stub st;
    st.set(ADDR(QProcess, waitForFinished), stub_waitFinish);
      st.set((void (QProcess::*)(QIODevice::OpenMode))ADDR(QProcess, start), stub_waitFinish);
      st.set(ADDR(QProcess, errorString), stub_errorString);
   dman::SpawnCmd("/bin/bas", list, output, err);
}
