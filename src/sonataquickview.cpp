#include "sonataquickview.h"

SonataQuickView::SonataQuickView(QWindow *parent) :
    QQuickView(parent)
{
}

void SonataQuickView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return && event->modifiers() & Qt::AltModifier)
    {
        // Alt + Enter pressed
        if (windowState() & Qt::WindowFullScreen)
            showMaximized();
        else
            showFullScreen();
    }

    QQuickView::keyPressEvent(event);
}
