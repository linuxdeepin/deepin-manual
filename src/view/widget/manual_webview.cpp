#include "manual_webview.h"
#include "view/web_window.h"

#include <cmath>

#include <QTimer>
#include <QWindow>
#include <QResizeEvent>

struct QCefWebViewPrivate {
    QCefWebPage* page = nullptr;
    bool window_mapped = false;
    bool auto_zoom = true;
};

ManualWebView::ManualWebView(QWidget* parent)
    : QCefWebView(parent)
    , m_parentWin(nullptr)
    , p_(new QCefWebViewPrivate())
{
    this->setAttribute(Qt::WA_NativeWindow, true);
    this->setAttribute(Qt::WA_DontCreateNativeAncestors, true);

    connect(page(), &QCefWebPage::renderContextCreated,
            this, &ManualWebView::updateWebZoom, Qt::QueuedConnection);

    if (qApp->metaObject()->indexOfSignal("screenDevicePixelRatioChanged(QScreen*)") >= 0)
    {
        connect(qApp, SIGNAL(screenDevicePixelRatioChanged(QScreen*)),
                this, SLOT(onScreenScaleChanged(QScreen*)), Qt::QueuedConnection);
    }
}

ManualWebView::~ManualWebView()
{
    if (p_ != nullptr)
    {
        if (p_->page != nullptr)
        {
            p_->page->deleteLater();
            p_->page = nullptr;
        }

        delete p_;
        p_ = nullptr;
    }
}

void ManualWebView::setParentWindow(DMainWindow *window)
{
    m_parentWin = window;
}

QCefWebPage* ManualWebView::page() const
{
    return QCefWebView::page();
}

bool ManualWebView::autoZoom() const
{
    return p_->auto_zoom;
}

void ManualWebView::setAutoZoom(bool autoZoom)
{
    if (p_->auto_zoom == autoZoom)
    {
        return;
    }

    p_->auto_zoom = autoZoom;

    updateWebZoom();
}

void ManualWebView::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);

    if (!p_->window_mapped)
    {
        p_->window_mapped = true;
        QTimer::singleShot(1, this, [=]() {
            page()->remapBrowserWindow(this->winId(), m_parentWin->winId());
        });

        updateWebZoom();
    }
}

void ManualWebView::resizeEvent(QResizeEvent* event)
{
    QCefWebView::resizeEvent(event);
}

void ManualWebView::focusInEvent(QFocusEvent *event)
{
    if (p_->page)
    {
        p_->page->setFocus(true);
    }

    QWidget::focusInEvent(event);
}

bool ManualWebView::event(QEvent *event)
{
    if (event->type() == QEvent::ScreenChangeInternal)
    {
        updateWebZoom();
    }

    return QWidget::event(event);
}

void ManualWebView::updateWebZoom()
{
    if (!p_->window_mapped)
    {
        return;
    }

    if (autoZoom())
    {
        // chromium 中缩放比例以 20% 递增，因此，zoomLevel 的值实际上表示递增的倍数
        // 如，值为1时表示缩放为1.2倍，值为2时表示缩放为1.2*1.2倍
        page()->setZoomFactor(std::log(devicePixelRatioF()) / std::log(1.2));
    }
    else
    {
        page()->resetZoomFactor();
    }
}

void ManualWebView::onScreenScaleChanged(QScreen *screen)
{
    if (window()->windowHandle() && window()->windowHandle()->screen() == screen)
    {
        updateWebZoom();
    }
}
