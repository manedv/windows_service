#ifndef COCOAHELPERS_H
#define COCOAHELPERS_H

#include <QWidget>

#ifdef Q_OS_MAC
void SetAsOverlayWindow(QWidget * w, int offset = 0);
QPixmap GrabScreen(unsigned int screenId);

#elif defined(Q_OS_WIN)
static void SetAsOverlayWindow(QWidget * w, int offset = 0)
{
    Q_UNUSED(offset);
    w->setWindowFlags(w->windowFlags() | Qt::WindowStaysOnTopHint);
}
#endif

#endif // COCOAHELPERS_H
