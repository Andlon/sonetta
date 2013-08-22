#ifndef SONATAAPPLICATION_H
#define SONATAAPPLICATION_H

#include <QGuiApplication>
#include <QQuickView>

#include <Sonata/Spotify>
#include "navigation/navigation.h"
#include "sonataplayer.h"
#include "uistatemanagement.h"

class SonataApplication : public QGuiApplication
{
    Q_OBJECT
public:
    explicit SonataApplication(int & argc, char ** argv);
    ~SonataApplication();

    bool notify(QObject *receiver, QEvent *event);

    int start();

    
private:
    QQuickView * m_view;
    Navigation * m_nav;

    SonataPlayer * m_player;
    SpotifySession * m_spotify;

    UiStateManagement * m_ui;

};

#endif // SONATAAPPLICATION_H
