#ifndef SESSIONTEST_H
#define SESSIONTEST_H

#include <QCoreApplication>
#include <QTimer>
#include <libspotify/api.h>


enum SpEvents {
    SpLoggedInEvent = QEvent::User,
    SpLoggedOutEvent,
    SpNotifyMainThreadEvent,
    SpConnectionStateUpdated
};

class SessionTest : public QObject {
    Q_OBJECT

public:
    bool createSession();

    static SessionTest * instance();

    bool event(QEvent *);

public slots:
    void login(const QString &username, const QString &password);
    void logout();
private:
    explicit SessionTest(QObject * parent = 0);
    void processSpotifyEvents();

    sp_session * m_session;
    int m_timerId;

    QTimer * m_logoutTimer;

};



#endif // SESSIONTEST_H
