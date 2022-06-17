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
#include "ut_searchedit.h"

#include <QToolButton>
#include <QAction>

ut_SearchEdit_test::ut_SearchEdit_test()
{

}



TEST_F(ut_SearchEdit_test, settext)
{
    SearchEdit m_se;
    m_se.setText("abc");
    ASSERT_EQ(m_se.text(), "abc");
}

TEST_F(ut_SearchEdit_test, keyEvent)
{
    SearchEdit m_se;
    QTest::keyClick(m_se.lineEdit(), Qt::Key_Up);
    QTest::keyClick(m_se.lineEdit(), Qt::Key_Down);
    QTest::keyClick(m_se.lineEdit(), Qt::Key_Enter);

    QTest::keyClick(m_se.lineEdit(), Qt::Key_A);
    qDebug() << "se.text()-->" << m_se.text();
    ASSERT_EQ(m_se.text(), 'a');
    QAction *clearAction = m_se.lineEdit()->findChild<QAction *>(QLatin1String("_q_qlineeditclearaction"));
    if (clearAction != nullptr) {
        QList<QToolButton *> list = m_se.lineEdit()->findChildren<QToolButton *>();
        for (int i = 0; i < list.count(); i++) {
            if (list.at(i)->defaultAction() == clearAction) {
                QToolButton *clearBtn = list.at(i);
                QTest::mouseClick(clearBtn, Qt::LeftButton);
                ASSERT_EQ(m_se.text(), "");
            }
        }
    }
}
