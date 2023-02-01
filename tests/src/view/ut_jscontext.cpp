// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ut_jscontext.h"
#include "../src/view/jscontext.h"

#include <QSignalSpy>

ut_JsContext::ut_JsContext()
{

}

void ut_JsContext::SetUp()
{
    m_jc = new JsContext();
}

void ut_JsContext::TearDown()
{
    delete m_jc;
}

TEST_F(ut_JsContext, recvParseMsg)
{
    QSignalSpy spy(m_jc, SIGNAL(parseMsg(QString, QString)));
    m_jc->recvParseMsg("aaaaa", "path");
    QList<QVariant> argu = spy.takeFirst();
    ASSERT_EQ(argu.at(0).toString(), "aaaaa");
    ASSERT_EQ(argu.at(1).toString(), "path");
}
