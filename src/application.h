#pragma once

#include <QGuiApplication>
#include <QScopedPointer>
#include <QSharedPointer>

#include <Spotinetta/Spotinetta>

#include "navigation/lircclient.h"
#include "navigation/navigation.h"
#include "player.h"
#include "uistatecoordinator.h"
#include "audiooutput.h"
#include "searchengine.h"
#include "settings.h"

#include "utilities/pointers.h"

class QQuickView;

namespace Sonetta {

class Application : public QObject
{
    Q_OBJECT
public:
    explicit Application(QObject * parent = 0);
    ~Application();

    static ObjectSharedPointer<Spotinetta::Session> session();

    bool eventFilter(QObject *, QEvent *);
    bool initialize();

private slots:
    void onExit();
    void onLogout();

    void updateCursor();
private:
    void createSession();
    void registerQmlTypes();
    void setupQuickEnvironment();
    void showUi();
    void loadFonts();

    ObjectScopedPointer<QQuickView>             m_view;
    ObjectScopedPointer<Player>                 m_player;
    ObjectScopedPointer<UIStateCoordinator>     m_ui;
    ObjectScopedPointer<SearchEngine>           m_search;
    ObjectScopedPointer<LircClient>             m_lirc;

    ObjectSharedPointer<AudioOutput>            m_output;
    ObjectSharedPointer<Settings>               m_settings;
    ObjectSharedPointer<Spotinetta::Session>    m_session;

    bool m_exiting;
};

}
