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

    void updateCursor();
    
private:
    void createSession();
    void registerQmlTypes();
    void setupQuickEnvironment();
    void showUi();
    void loadFonts();

    QScopedPointer<QQuickView> m_view;

    Player *                    m_player;
    Spotinetta::Session *       m_session;
    UIStateCoordinator *        m_ui;
    QSharedPointer<AudioOutput> m_output;
    QSharedPointer<Settings>    m_settings;
    SearchEngine *              m_search;
    LircClient *                m_lirc;

    bool m_exiting;

};

}
