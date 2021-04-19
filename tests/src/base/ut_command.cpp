#include "ut_command.h"

#include "base/command.h"



ut_command_test::ut_command_test()
{

}

TEST(ut_command_test, RunScriptFile)
{
    //设置当前工作目录为 manual/src/web/js
    QString jsPath = DMAN_SEARCH_WORK_DIR;
    jsPath += "/toManual/js";

    QDir dir(jsPath);
    QStringList nameFilters;
    nameFilters << "*.jsx";
    QStringList list  = dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
    qDebug() << list;
    ASSERT_FALSE(dman::RunScriptFile(list));
}

TEST(ut_command_test, RunScriptFile2)
{
    QStringList list{"app/src/app.js"};
    ASSERT_FALSE(dman::RunScriptFile(list));
}

TEST(ut_command_test, RunScriptFile3)
{
    //设置当前工作目录为 manual/src/web/js
    QString jsPath = DMAN_SEARCH_WORK_DIR;
    jsPath += "/js";

    QDir dir(jsPath);
    QStringList nameFilters;
    nameFilters << "*.jsx";
    QStringList list  = dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
    QString output;
    QString err;
//    ASSERT_FALSE(dman::RunScriptFile(list, output, err));
}


TEST(ut_command_test, RunScriptFile4)
{
    QStringList list{"app/src/app.js"};
    QString output;
    QString err;
    ASSERT_FALSE(dman::RunScriptFile(list, output, err));
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
//    ASSERT_FALSE(dman::SpawnCmd("/bin/bash", list, output));
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
    ASSERT_FALSE(dman::SpawnCmd("/bin/bas", list, output, err));
}
