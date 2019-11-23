#ifndef MANUALWEBVIEW_H
#define MANUALWEBVIEW_H

#include <DTitlebar>
#include <DMainWindow>

#include <qcef_web_page.h>
#include <qcef_web_settings.h>
#include <qcef_web_view.h>

DWIDGET_USE_NAMESPACE

class ManualWebView : public QCefWebView {

    Q_OBJECT
    Q_PROPERTY(QUrl url READ url WRITE setUrl)
    Q_PROPERTY(bool autoZoom READ autoZoom WRITE setAutoZoom)

public:
    explicit ManualWebView(QWidget* parent = nullptr);
    virtual ~ManualWebView() override;

    void setParentWindow(DMainWindow *window);

protected:
    void showEvent(QShowEvent* event) override;

private:
    void updateWebZoom();

    bool m_window_mapped;

    DMainWindow *m_parentWin;
};

#endif // MANUALWEBVIEW_H
