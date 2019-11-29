#include "manual_webview.h"
#include "view/web_window.h"

#include <cmath>

#include <QTimer>

ManualWebView::ManualWebView(QWidget* parent)
    : QCefWebView(parent)
    , m_window_mapped(false)
    , m_parentWin(nullptr)
{
}

ManualWebView::~ManualWebView()
{
}

void ManualWebView::setParentWindow(DMainWindow *window)
{
    m_parentWin = window;
}

void ManualWebView::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);

    if (!m_window_mapped)
    {
        m_window_mapped = true;
        QTimer::singleShot(1, this, [=]() {
            page()->remapBrowserWindow(this->winId(), m_parentWin->winId());
        });

        updateWebZoom();
    }
}

void ManualWebView::updateWebZoom()
{
    if (!m_window_mapped)
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
