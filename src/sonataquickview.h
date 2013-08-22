#ifndef SONATAQUICKVIEW_H
#define SONATAQUICKVIEW_H

#include <QQuickView>

class SonataQuickView : public QQuickView
{
    Q_OBJECT
public:
    explicit SonataQuickView(QWindow *parent = 0);

    void keyPressEvent(QKeyEvent *);
    
signals:

    
public slots:
    
};

#endif // SONATAQUICKVIEW_H
