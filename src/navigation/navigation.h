#pragma once

#include <QObject>
#include <QEvent>
#include <QtQml>
#include <QPointer>
#include <QtQuick/QQuickItem>

static const QEvent::Type NavEventType = (QEvent::Type) (QEvent::User + 4032);

class QKeyEvent;
class NavEvent;
class QuickNavEvent;

class Navigation : public QObject
{
    Q_OBJECT
    Q_ENUMS(Button)
public:
    enum Button {
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

    static void dispatchNavigationEvent(Button button, bool autoRepeat);
    static void registerTypes();

    static Navigation::Button translateKey(Qt::Key key);
    static Qt::Key translateButton(Navigation::Button button);
};

class NavEvent : public QEvent {
public:
    NavEvent(Navigation::Button button, bool isAutoRepeat = false)
        :   QEvent(NavEventType), m_isAutoRepeat(isAutoRepeat), m_button(button) { setAccepted(false); }

    bool isAutoRepeat() const { return m_isAutoRepeat; }
    Navigation::Button button() const { return m_button; }

private:
    bool m_isAutoRepeat;
    Navigation::Button m_button;
};

class QuickNavEvent : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool isAutoRepeat READ isAutoRepeat CONSTANT)
    Q_PROPERTY(Navigation::Button button READ button CONSTANT)
    Q_PROPERTY(bool accepted READ isAccepted WRITE setAccepted)
public:
    QuickNavEvent(Navigation::Button button, bool isAutoRepeat = false)
        :   event(button, isAutoRepeat) {  }

    bool isAutoRepeat() const { return event.isAutoRepeat(); }
    Navigation::Button button() const { return event.button(); }

    bool isAccepted() const { return event.isAccepted(); }
    void setAccepted(bool accepted) { event.setAccepted(accepted); }

private:
    NavEvent event;
};

/*
 *  The existence of this class is based solely on the fact that
 *  as of Qt 5.2.0, any events of interest passed to QQuickItem-derivatives
 *  bypass any standard Qt event mechanisms, thereby removing the possiblity of
 *  installing event filters and use attached items (if installEventFilter() would be
 *  usable on QQuickItem-derivatives, the implementation would be a whole lot simpler...)
 *
 *  See https://bugreports.qt-project.org/browse/QTBUG-3200
 */
class QuickNavigation : public QQuickItem {
Q_OBJECT
public:
    explicit QuickNavigation(QQuickItem * parent = 0) : QQuickItem(parent) { }

    void keyPressEvent(QKeyEvent *);

signals:
    void left(QuickNavEvent * event);
    void right(QuickNavEvent * event);
    void up(QuickNavEvent  *event);
    void down(QuickNavEvent * event);
    void ok(QuickNavEvent * event);
    void back(QuickNavEvent * event);
    void play(QuickNavEvent * event);
    void pause(QuickNavEvent * event);
    void playPause(QuickNavEvent * event);
    void stop(QuickNavEvent * event);
    void next(QuickNavEvent * event);
    void previous(QuickNavEvent * event);
    void record(QuickNavEvent * event);
    void volumeUp(QuickNavEvent * event);
    void volumeDown(QuickNavEvent * event);
    void mute(QuickNavEvent * event);

    void buttonPressed(QuickNavEvent * event);

private:
    template <typename MemberFunctionPointer>
    void acceptAndEmit(MemberFunctionPointer, QuickNavEvent * event);
};
