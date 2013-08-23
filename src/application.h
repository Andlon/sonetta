#pragma once

#include <QGuiApplication>

#include <Spotinetta/Spotinetta>

#include "navigation/navigation.h"
#include "player.h"
#include "uistatecoordinator.h"

class QQuickView;

namespace Sonetta {

class Application : public QGuiApplication
{
    Q_OBJECT
public:
    explicit Application(int & argc, char ** argv);
    ~Application();

    bool notify(QObject *receiver, QEvent *event);

    int run();
    
private:
    QQuickView * m_view;
    Navigation * m_nav;

    Player *                m_player;
    Spotinetta::Session *   m_session;
    UIStateCoordinator *    m_ui;

};

}
