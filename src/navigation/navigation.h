#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <QObject>
#include <QEvent>
#include <QtQml>
#include <QPointer>

#include "navigationnativeeventfilter.h"

#ifdef Q_OS_LINUX
#include "lircremote.h"
#endif


static const QEvent::Type NavEventType = (QEvent::Type) (QEvent::User + 4032);

class QKeyEvent;
class NavEvent;
class QuickNavEvent;

/************************************************************/
class NavigationAttached : public QObject
{
    Q_OBJECT

public:
    explicit NavigationAttached(QObject *parent = 0);

signals:
    void left(QuickNavEvent *event);
    void right(QuickNavEvent *event);
    void up(QuickNavEvent *event);
    void down(QuickNavEvent *event);
    void ok(QuickNavEvent *event);
    void back(QuickNavEvent *event);
    void play(QuickNavEvent *event);
    void pause(QuickNavEvent *event);
    void playPause(QuickNavEvent *event);
    void stop(QuickNavEvent *event);
    void next(QuickNavEvent *event);
    void previous(QuickNavEvent *event);
    void record(QuickNavEvent *event);
    void navigationEvent(QuickNavEvent *event);

protected:
    void customEvent(QEvent *);

};

/************************************************************/
class Navigation : public QObject
{
    Q_OBJECT
    Q_ENUMS(Key)
public:
    enum Key {
        Undefined,
        Left,
        Right,
        Up,
        Down,
        OK,
        Back,
        Play,
        Pause,
        PlayPause,
        Stop,
        FastForward,
        Rewind,
        Next,
        Previous,
        Record,
        VolumeUp,
        VolumeDown,
        Mute,
        ChannelDown,
        ChannelUp
    };

    explicit Navigation(QObject *parent = 0);
    ~Navigation();

    static NavigationAttached * qmlAttachedProperties(QObject *object);

    static bool dispatchNavigationEvent(NavEvent *event);
    static bool dispatchKeyEvent(QKeyEvent *keyEvent);
    static Navigation::Key translateKey(Qt::Key key);


private:
    NavigationNativeEventFilter * m_filter;

#ifdef Q_OS_LINUX
    LircRemote * m_lirc;
#endif
};

class NavEvent : public QEvent {
public:
    NavEvent(Navigation::Key key, bool isAutoRepeat = false)
        :   QEvent(NavEventType), m_isAutoRepeat(isAutoRepeat), m_key(key) { setAccepted(false); }

    bool isAutoRepeat() const { return m_isAutoRepeat; }
    Navigation::Key key() const { return m_key; }

private:
    bool m_isAutoRepeat;
    Navigation::Key m_key;
};

class QuickNavEvent : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool isAutoRepeat READ isAutoRepeat CONSTANT)
    Q_PROPERTY(Navigation::Key key READ key CONSTANT)
    Q_PROPERTY(bool accepted READ isAccepted WRITE setAccepted)
public:
    QuickNavEvent(Navigation::Key key, bool isAutoRepeat = false)
        :   event(key, isAutoRepeat) {  }

    bool isAutoRepeat() const { return event.isAutoRepeat(); }
    Navigation::Key key() const { return event.key(); }

    bool isAccepted() const { return event.isAccepted(); }
    void setAccepted(bool accepted) { event.setAccepted(accepted); }

private:
    NavEvent event;
};

QML_DECLARE_TYPEINFO(Navigation, QML_HAS_ATTACHED_PROPERTIES)

#endif // NAVIGATION_H
