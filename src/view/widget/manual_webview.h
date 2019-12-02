#ifndef MANUALWEBVIEW_H
#define MANUALWEBVIEW_H

#include <qcef_web_page.h>
#include <qcef_web_settings.h>
#include <qcef_web_view.h>

#include <QShortcut>

#include <DTitlebar>
#include <DMainWindow>

DWIDGET_USE_NAMESPACE

struct QCefWebViewPrivate;

class ManualWebView : public QCefWebView {

    Q_OBJECT
    Q_PROPERTY(QUrl url READ url WRITE setUrl)
    Q_PROPERTY(bool autoZoom READ autoZoom WRITE setAutoZoom)

public:
    explicit ManualWebView(QWidget* parent = nullptr);
    virtual ~ManualWebView() override;

    void setParentWindow(DMainWindow *window);

    QCefWebPage* page() const;
    bool autoZoom() const;

public Q_SLOTS:
    void setAutoZoom(bool autoZoom);

protected:
    void showEvent(QShowEvent* event) override;

    void resizeEvent(QResizeEvent* event) override;
    void focusInEvent(QFocusEvent *event) override;
    bool event(QEvent *event) override;

private:
    void updateWebZoom();

    Q_SLOT void onScreenScaleChanged(QScreen *screen);
    DMainWindow *m_parentWin;
    QCefWebViewPrivate* p_ = nullptr;
};

#endif // MANUALWEBVIEW_H
