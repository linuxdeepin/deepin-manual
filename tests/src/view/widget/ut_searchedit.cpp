// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

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
