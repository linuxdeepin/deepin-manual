// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ut_shellobj.h"
#include "controller/shellobj.h"

ut_shellObj::ut_shellObj()
{

}

void ut_shellObj::SetUp()
{
    m_so = new shellObj();
}

void ut_shellObj::TearDown()
{
    delete m_so;
}

TEST_F(ut_shellObj, execSystem)
{
    m_so->execSystem("touch /tmp/1.txt");
    sleep(1);
    QFileInfo fileinfo("/tmp/1.txt");
    ASSERT_TRUE(fileinfo.exists());
    m_so->execSystem("rm /tmp/1.txt");
    sleep(1);
    QFile fileinfo1("/tmp/1.txt");
    ASSERT_FALSE(fileinfo1.exists());
}
