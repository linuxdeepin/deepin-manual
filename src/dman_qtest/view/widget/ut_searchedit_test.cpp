/*
* Copyright (C) 2019 ~ 2020 Deepin Technology Co., Ltd.
*
* Author:     wangmingliang <wangmingliang@uniontech.com>
* Maintainer: wangmingliang <wanmgmingliang@uniontech.com>
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
#include "ut_searchedit_test.h"
#include <QToolButton>
#include <QAction>

namespace dman {

ut_SearchEdit_test::ut_SearchEdit_test()
{

}

void ut_SearchEdit_test::SetUp()
{
    if (m_se == nullptr)
        qDebug() << "null";
    else {
        qDebug() << "!= null";
    }

    m_se = new SearchEdit;


//    m_se->show();
}

void ut_SearchEdit_test::TearDown()
{
    delete m_se;
}

TEST_F(ut_SearchEdit_test, getStr)
{
    ASSERT_EQ(m_se->getStr(), "abc");
//    m_se->show();
}
TEST_F(ut_SearchEdit_test, settext)
{
    m_se->setText("abc");
    ASSERT_EQ(m_se->text(), "abc");
}

TEST_F(ut_SearchEdit_test, keyEvent)
{
    QTest::keyClick(m_se->lineEdit(), Qt::Key_Up);
    QTest::keyClick(m_se->lineEdit(), Qt::Key_Down);
    QTest::keyClick(m_se->lineEdit(), Qt::Key_Enter);

    QTest::keyClick(m_se->lineEdit(), Qt::Key_A);
    qDebug() << "se.text()-->" << m_se->text();
    ASSERT_EQ(m_se->text(), 'a');
    QAction *clearAction = m_se->lineEdit()->findChild<QAction *>(QLatin1String("_q_qlineeditclearaction"));
    if (clearAction != nullptr) {
        QList<QToolButton *> list = m_se->lineEdit()->findChildren<QToolButton *>();
        for (int i = 0; i < list.count(); i++) {
            if (list.at(i)->defaultAction() == clearAction) {
                QToolButton *clearBtn = list.at(i);
                QTest::mouseClick(clearBtn, Qt::LeftButton);
                ASSERT_EQ(m_se->text(), "");
            }
        }
    }
}

}
