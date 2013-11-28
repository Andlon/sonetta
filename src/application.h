#pragma once

#include <QGuiApplication>
#include <QScopedPointer>

#include <Spotinetta/Spotinetta>

#include "navigation/navigation.h"
#include "player.h"
#include "uistatecoordinator.h"
#include "audiooutput.h"
#include "searchengine.h"

class QQuickView;

namespace Sonetta {

class Application : public QGuiApplication
{
    Q_OBJECT
public:
    explicit Application(int & argc, char ** argv);

    bool notify(QObject *receiver, QEvent *event);

    static Application * instance();

    Spotinetta::Session * session() const;

    int run();

private slots:
    void onExit();
    void onLogout();
    
private:
    void createSession();
    void registerQmlTypes();
    void setupQuickEnvironment();
    void showUi();

    QScopedPointer<QQuickView> m_view;
    Navigation * m_nav;

    Player *                m_player;
    Spotinetta::Session *   m_session;
    UIStateCoordinator *    m_ui;
    AudioOutput *           m_output;
    SearchEngine *          m_search;

    bool m_exiting;

};

}
