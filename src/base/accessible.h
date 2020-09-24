#ifndef ACCESSIBLE_H
#define ACCESSIBLE_H

#include "accessibledefine.h"
//#include "displaycontent.h"
//#include "exportprogressdlg.h"
//#include "filtercontent.h"
//#include "logcollectormain.h"
//#include "logcombox.h"
//#include "lognormalbutton.h"
//#include "logperiodbutton.h"
//#include "logspinnerwidget.h"
//#include "logtreeview.h"
//#include "logviewheaderview.h"
//#include "logdetailinfowidget.h"
#include "view/web_window.h"

#include <DSwitchButton>
#include <DBackgroundGroup>
#include <DFloatingButton>
#include <DLineEdit>
#include <DLabel>
#include <DListView>
#include <DCommandLinkButton>
#include <DSearchEdit>
#include <DTitlebar>
#include <DComboBox>
#include <DCheckBox>
#include <DTreeView>
#include <DIconButton>
#include <DToolButton>
#include <DProgressBar>
#include <DTextEdit>
#include <DDialog>
#include <DFileDialog>
#include <DFrame>
#include <DPushButton>
DWIDGET_USE_NAMESPACE
//using namespace DCC_NAMESPACE;

SET_FORM_ACCESSIBLE(WebWindow,"WebWindow")
//SET_FORM_ACCESSIBLE(DisplayContent, "DisplayContent")
//SET_FORM_ACCESSIBLE(ExportProgressDlg, "ExportProgressDlg")
//SET_FORM_ACCESSIBLE(FilterContent, "FilterContent")
//SET_FORM_ACCESSIBLE(LogCollectorMain, "LogCollectorMain")
//SET_BUTTON_ACCESSIBLE(LogCombox, "LogCombox")
//SET_FORM_ACCESSIBLE(logDetailInfoWidget, "logDetailInfoWidget")
//SET_BUTTON_ACCESSIBLE(LogIconButton, "LogIconButton")
//SET_FORM_ACCESSIBLE(LogListView, "LogListView")
//SET_BUTTON_ACCESSIBLE(LogNormalButton, "LogNormalButton")
//SET_BUTTON_ACCESSIBLE(LogPeriodButton, "LogPeriodButton")
//SET_FORM_ACCESSIBLE(LogSpinnerWidget, "LogSpinnerWidget")
//SET_FORM_ACCESSIBLE(LogTreeView, "LogTreeView")
//SET_FORM_ACCESSIBLE(LogViewHeaderView, "LogViewHeaderView")

// Qt控件
SET_FORM_ACCESSIBLE(QFrame, m_w->objectName().isEmpty() ? "frame" : m_w->objectName())
SET_FORM_ACCESSIBLE(QWidget, m_w->objectName().isEmpty() ? "widget" : m_w->objectName())
SET_BUTTON_ACCESSIBLE(QPushButton, m_w->text().isEmpty() ? "qpushbutton" : m_w->text())
SET_SLIDER_ACCESSIBLE(QSlider, "qslider")
SET_FORM_ACCESSIBLE(QMenu, "qmenu")
//SET_LABEL_ACCESSIBLE(QLabel, m_w->text().isEmpty() ? "qlabel" : m_w->text())

// Dtk控件
SET_FORM_ACCESSIBLE(DFrame, m_w->objectName().isEmpty() ? "frame" : m_w->objectName())
SET_FORM_ACCESSIBLE(DComboBox, m_w->objectName().isEmpty() ? "combox" : m_w->objectName())
SET_FORM_ACCESSIBLE(DWidget, m_w->objectName().isEmpty() ? "widget" : m_w->objectName())
SET_FORM_ACCESSIBLE(DBackgroundGroup, m_w->objectName().isEmpty() ? "dbackgroundgroup" : m_w->objectName())
SET_BUTTON_ACCESSIBLE(DSwitchButton, m_w->text().isEmpty() ? "switchbutton" : m_w->text())
SET_BUTTON_ACCESSIBLE(DFloatingButton, m_w->toolTip().isEmpty() ? "DFloatingButton" : m_w->toolTip())
SET_FORM_ACCESSIBLE(DSearchEdit, m_w->objectName().isEmpty() ? "DSearchEdit" : m_w->objectName())
SET_BUTTON_ACCESSIBLE(DPushButton, m_w->objectName().isEmpty() ? "DPushButton" : m_w->objectName())
SET_BUTTON_ACCESSIBLE(DIconButton, m_w->objectName().isEmpty() ? "DIconButton" : m_w->objectName())
SET_BUTTON_ACCESSIBLE(DCheckBox, m_w->objectName().isEmpty() ? "DCheckBox" : m_w->objectName())
SET_BUTTON_ACCESSIBLE(DCommandLinkButton, "DCommandLinkButton")
SET_FORM_ACCESSIBLE(DTitlebar, m_w->objectName().isEmpty() ? "DTitlebar" : m_w->objectName())
//SET_LABEL_ACCESSIBLE(DLabel, m_w->text().isEmpty() ? "DLabel" : m_w->text())
SET_BUTTON_ACCESSIBLE(DToolButton, m_w->objectName().isEmpty() ? "DToolButton" : m_w->objectName())
SET_FORM_ACCESSIBLE(DDialog, m_w->objectName().isEmpty() ? "DDialog" : m_w->objectName())
SET_FORM_ACCESSIBLE(DFileDialog, m_w->objectName().isEmpty() ? "DFileDialog" : m_w->objectName())

QAccessibleInterface *accessibleFactory(const QString &classname, QObject *object)
{
    QAccessibleInterface *interface = nullptr;
    // 应用主窗口
    USE_ACCESSIBLE(QString(classname).replace("dccV20::", ""), WebWindow);
//    USE_ACCESSIBLE(QString(classname).replace("dccV20::", ""), DisplayContent);
//    USE_ACCESSIBLE(QString(classname).replace("dccV20::", ""), ExportProgressDlg);
//    USE_ACCESSIBLE(QString(classname).replace("dccV20::", ""), FilterContent);
//    USE_ACCESSIBLE(QString(classname).replace("dccV20::", ""), LogCollectorMain);
    // USE_ACCESSIBLE(QString(classname).replace("dccV20::", ""), LogCombox);
//    USE_ACCESSIBLE(QString(classname).replace("dccV20::", ""), logDetailInfoWidget);
//    USE_ACCESSIBLE(QString(classname).replace("dccV20::", ""), LogListView);
//    USE_ACCESSIBLE(QString(classname).replace("dccV20::", ""), LogIconButton);
//    USE_ACCESSIBLE(QString(classname).replace("dccV20::", ""), LogPeriodButton);
//    USE_ACCESSIBLE(QString(classname).replace("dccV20::", ""), LogSpinnerWidget);
//    USE_ACCESSIBLE(QString(classname).replace("dccV20::", ""), LogTreeView);
//   USE_ACCESSIBLE(QString(classname).replace("dccV20::", ""), LogViewHeaderView);
//    USE_ACCESSIBLE(QString(classname).replace("dccV20::", ""), LogNormalButton);

    //  Qt 控件
    USE_ACCESSIBLE(QString(classname).replace("dccV20::", ""), QFrame);
    USE_ACCESSIBLE(QString(classname).replace("dccV20::", ""), QWidget);
    USE_ACCESSIBLE(QString(classname).replace("dccV20::", ""), QPushButton);
    USE_ACCESSIBLE(QString(classname).replace("dccV20::", ""), QSlider);
    USE_ACCESSIBLE(QString(classname).replace("dccV20::", ""), QMenu);
    //    USE_ACCESSIBLE(QString(classname).replace("dccV20::", ""), QLabel);

    //  dtk 控件
    USE_ACCESSIBLE(QString(classname).replace("dccV20::", ""), DFrame);
    USE_ACCESSIBLE(QString(classname).replace("dccV20::", ""), DWidget);
    USE_ACCESSIBLE(QString(classname).replace("dccV20::", ""), DComboBox);

    USE_ACCESSIBLE(QString(classname).replace("dccV20::", ""), DBackgroundGroup);
    USE_ACCESSIBLE(QString(classname).replace("dccV20::", ""), DSwitchButton);
    USE_ACCESSIBLE(QString(classname).replace("dccV20::", ""), DFloatingButton);
    USE_ACCESSIBLE(QString(classname).replace("dccV20::", ""), DSearchEdit);
    USE_ACCESSIBLE(QString(classname).replace("dccV20::", ""), DPushButton);
    USE_ACCESSIBLE(QString(classname).replace("dccV20::", ""), DIconButton);
    USE_ACCESSIBLE(QString(classname).replace("dccV20::", ""), DCheckBox);
    USE_ACCESSIBLE(QString(classname).replace("dccV20::", ""), DCommandLinkButton);
    USE_ACCESSIBLE(QString(classname).replace("dccV20::", ""), DTitlebar);
    //    USE_ACCESSIBLE(QString(classname).replace("dccV20::", ""), DLabel);
    USE_ACCESSIBLE(QString(classname).replace("dccV20::", ""), DDialog);
    USE_ACCESSIBLE(QString(classname).replace("dccV20::", ""), DFileDialog);

    return interface;
}

#endif // ACCESSIBLE_H
