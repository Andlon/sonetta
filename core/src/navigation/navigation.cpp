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
    qmlRegisterType<QuickNavigation>("Navigation", 0, 1, "Nav");
    qmlRegisterUncreatableType<Navigation>("Navigation", 0, 1, "Navigation", "CAnnot instantiate Navigation.");
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

void QuickNavigation::keyPressEvent(QKeyEvent * e)
{
    Navigation::Button button = Navigation::translateKey(static_cast<Qt::Key>(e->key()));
    QuickNavEvent event(button, e->isAutoRepeat());

    switch (event.button())
    {
    case Navigation::Left:
        acceptAndEmit(&QuickNavigation::left, &event);
        break;
    case Navigation::Right:
        acceptAndEmit(&QuickNavigation::right, &event);
        break;
    case Navigation::Up:
        acceptAndEmit(&QuickNavigation::up, &event);
        break;
    case Navigation::Down:
        acceptAndEmit(&QuickNavigation::down, &event);
        break;
    case Navigation::OK:
        acceptAndEmit(&QuickNavigation::ok, &event);
        break;
    case Navigation::Back:
        acceptAndEmit(&QuickNavigation::back, &event);
        break;
    case Navigation::Play:
        acceptAndEmit(&QuickNavigation::play, &event);
        break;
    case Navigation::PlayPause:
        acceptAndEmit(&QuickNavigation::playPause, &event);
        break;
    case Navigation::Pause:
        acceptAndEmit(&QuickNavigation::pause, &event);
        break;
    case Navigation::Stop:
        acceptAndEmit(&QuickNavigation::stop, &event);
        break;
    case Navigation::Next:
        acceptAndEmit(&QuickNavigation::next, &event);
        break;
    case Navigation::Previous:
        acceptAndEmit(&QuickNavigation::previous, &event);
        break;
    case Navigation::Record:
        acceptAndEmit(&QuickNavigation::record, &event);
        break;
    case Navigation::VolumeDown:
        acceptAndEmit(&QuickNavigation::volumeDown, &event);
        break;
    case Navigation::VolumeUp:
        acceptAndEmit(&QuickNavigation::volumeUp, &event);
        break;
    case Navigation::Mute:
        acceptAndEmit(&QuickNavigation::mute, &event);
    default:
        break;
    }

    if (!event.isAccepted())
    {
        emit buttonPressed(&event);
    }

    e->setAccepted(event.isAccepted());
}

template <typename MemberFunctionPointer>
void QuickNavigation::acceptAndEmit(MemberFunctionPointer member, QuickNavEvent * event)
{
    QMetaMethod signal = QMetaMethod::fromSignal(member);
    if (isSignalConnected(signal))
    {
        event->setAccepted(true);
        signal.invoke(this, Qt::DirectConnection, Q_ARG(QuickNavEvent *, event));
    }
}
