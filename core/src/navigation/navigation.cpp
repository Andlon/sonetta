#include "navigation.h"

#include <QCoreApplication>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QDebug>

#include <QtQuick>

void Navigation::dispatchNavigationEvent(Navigation::Button button, bool autoRepeat)
{
    QKeyEvent * keyEvent = new QKeyEvent(QEvent::KeyPress, translateButton(button), Qt::NoModifier, QString(), autoRepeat);
    QCoreApplication::postEvent(QGuiApplication::focusWindow(), keyEvent);
}

void Navigation::registerTypes()
{
    qmlRegisterUncreatableType<Navigation>("Navigation", 0, 1, "Navigation", "Cannot instantiate Navigation.");
    qmlRegisterUncreatableType<QuickNavEvent>("Navigation", 0, 1, "NavEvent", "Cannot instantiate navigation event. ");
}

Navigation::Button Navigation::translateKey(Qt::Key key)
{
    switch (key)
    {
    case Qt::Key_Left:
        return Navigation::Left;
    case Qt::Key_Right:
        return Navigation::Right;
    case Qt::Key_Up:
        return Navigation::Up;
    case Qt::Key_Down:
        return Navigation::Down;
    case Qt::Key_Enter:
    case Qt::Key_Return:
        return Navigation::OK;
    case Qt::Key_Back:
    case Qt::Key_Backspace:
        return Navigation::Back;
    case Qt::Key_MediaPlay:
        return Navigation::Play;
    case Qt::Key_MediaPause:
        return Navigation::Pause;
    case Qt::Key_MediaTogglePlayPause:
        return Navigation::PlayPause;
    case Qt::Key_MediaStop:
        return Navigation::Stop;
    case Qt::Key_MediaNext:
        return Navigation::Next;
    case Qt::Key_MediaPrevious:
        return Navigation::Previous;
    case Qt::Key_VolumeUp:
        return Navigation::VolumeUp;
    case Qt::Key_VolumeDown:
        return Navigation::VolumeDown;
    case Qt::Key_VolumeMute:
        return Navigation::Mute;

    default:
        return Navigation::Undefined;
    }
}

Qt::Key Navigation::translateButton(Navigation::Button button)
{
    switch (button)
    {
    case Navigation::Left:
        return Qt::Key_Left;
    case Navigation::Right:
        return Qt::Key_Right;
    case Navigation::Up:
        return Qt::Key_Up;
    case Navigation::Down:
        return Qt::Key_Down;
    case Navigation::OK:
        return Qt::Key_Return;
    case Navigation::Back:
        return Qt::Key_Backspace;
    case Navigation::Play:
        return Qt::Key_MediaPlay;
    case Navigation::Pause:
        return Qt::Key_MediaPause;
    case Navigation::Stop:
        return Qt::Key_MediaStop;
    case Navigation::PlayPause:
        return Qt::Key_MediaTogglePlayPause;
    case Navigation::Next:
        return Qt::Key_MediaNext;
    case Navigation::Previous:
        return Qt::Key_MediaPrevious;
    case Navigation::VolumeDown:
        return Qt::Key_VolumeDown;
    case Navigation::VolumeUp:
        return Qt::Key_VolumeUp;
    case Navigation::Mute:
        return Qt::Key_VolumeMute;
    case Navigation::Record:
        return Qt::Key_MediaRecord;

    default:
        return Qt::Key_unknown;
    }
}

NavigationAttachedType * Navigation::qmlAttachedProperties(QObject *object)
{
    auto attached = new NavigationAttachedType(object);
    object->installEventFilter(attached);
    return attached;
}

bool NavigationAttachedType::keyPressEvent(const QKeyEvent * e)
{
    if (e->modifiers() != Qt::NoModifier)
        return false;

    Navigation::Button button = Navigation::translateKey(static_cast<Qt::Key>(e->key()));
    if (button == Navigation::Button::Undefined)
        return false;

    QuickNavEvent event(button, e->isAutoRepeat());

    switch (event.button())
    {
    case Navigation::Left:
        acceptAndEmit(&NavigationAttachedType::left, &event);
        break;
    case Navigation::Right:
        acceptAndEmit(&NavigationAttachedType::right, &event);
        break;
    case Navigation::Up:
        acceptAndEmit(&NavigationAttachedType::up, &event);
        break;
    case Navigation::Down:
        acceptAndEmit(&NavigationAttachedType::down, &event);
        break;
    case Navigation::OK:
        acceptAndEmit(&NavigationAttachedType::ok, &event);
        break;
    case Navigation::Back:
        acceptAndEmit(&NavigationAttachedType::back, &event);
        break;
    case Navigation::Play:
        acceptAndEmit(&NavigationAttachedType::play, &event);
        break;
    case Navigation::PlayPause:
        acceptAndEmit(&NavigationAttachedType::playPause, &event);
        break;
    case Navigation::Pause:
        acceptAndEmit(&NavigationAttachedType::pause, &event);
        break;
    case Navigation::Stop:
        acceptAndEmit(&NavigationAttachedType::stop, &event);
        break;
    case Navigation::Next:
        acceptAndEmit(&NavigationAttachedType::next, &event);
        break;
    case Navigation::Previous:
        acceptAndEmit(&NavigationAttachedType::previous, &event);
        break;
    case Navigation::Record:
        acceptAndEmit(&NavigationAttachedType::record, &event);
        break;
    case Navigation::VolumeDown:
        acceptAndEmit(&NavigationAttachedType::volumeDown, &event);
        break;
    case Navigation::VolumeUp:
        acceptAndEmit(&NavigationAttachedType::volumeUp, &event);
        break;
    case Navigation::Mute:
        acceptAndEmit(&NavigationAttachedType::mute, &event);
        break;
    default:
        break;
    }

    if (!event.isAccepted())
    {
        emit buttonPressed(&event);
    }

    return event.isAccepted();
}

bool NavigationAttachedType::eventFilter(QObject *, QEvent * event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent * keyEvent = static_cast<QKeyEvent *>(event);
        return keyPressEvent(keyEvent);
    }
    return false;
}

template <typename MemberFunctionPointer>
void NavigationAttachedType::acceptAndEmit(MemberFunctionPointer member, QuickNavEvent * event)
{
    QMetaMethod signal = QMetaMethod::fromSignal(member);
    if (isSignalConnected(signal))
    {
        event->setAccepted(true);
        signal.invoke(this, Qt::DirectConnection, Q_ARG(QuickNavEvent *, event));
    }
}
