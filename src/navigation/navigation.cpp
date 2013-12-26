#include "navigation.h"

#include <QCoreApplication>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QDebug>

#include <QtQuick>

NavigationAttached::NavigationAttached(QObject *parent)
    :   QObject(parent)
{

}

bool NavigationAttached::event(QEvent * event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent * keyEvent = static_cast<QKeyEvent *>(event);

        Navigation::Button key = Navigation::translateKey(static_cast<Qt::Key>(keyEvent->key()));
        NavEvent navEvent(key, keyEvent->isAutoRepeat());
        customEvent(&navEvent);

        event->setAccepted(navEvent.isAccepted());
        return event->isAccepted();
    }

    return QObject::event(event);
}

void NavigationAttached::customEvent(QEvent *event)
{
    if (event->type() == NavEventType)
    {
        NavEvent *nEvent = static_cast<NavEvent *>(event);
        QuickNavEvent qne(nEvent->key(), nEvent->isAutoRepeat());

        emit navigationEvent(&qne);

        nEvent->setAccepted(qne.isAccepted());

        if (!nEvent->isAccepted())
        {
            switch (nEvent->key())
            {
            case Navigation::Left:
                if (isSignalConnected(QMetaMethod::fromSignal(&NavigationAttached::left)))
                {
                    qne.setAccepted(true);
                    emit left(&qne);
                }
                break;
            case Navigation::Right:
                if (isSignalConnected(QMetaMethod::fromSignal(&NavigationAttached::right)))
                {
                    qne.setAccepted(true);
                    emit right(&qne);
                }
                break;
            case Navigation::Up:
                if (isSignalConnected(QMetaMethod::fromSignal(&NavigationAttached::up)))
                {
                    qne.setAccepted(true);
                    emit up(&qne);
                }
                break;
            case Navigation::Down:
                if (isSignalConnected(QMetaMethod::fromSignal(&NavigationAttached::down)))
                {
                    qne.setAccepted(true);
                    emit down(&qne);
                }
                break;
            case Navigation::OK:
                if (isSignalConnected(QMetaMethod::fromSignal(&NavigationAttached::ok)))
                {
                    qne.setAccepted(true);
                    emit ok(&qne);
                }
                break;
            case Navigation::Back:
                if (isSignalConnected(QMetaMethod::fromSignal(&NavigationAttached::back)))
                {
                    qne.setAccepted(true);
                    emit back(&qne);
                }
                break;
            case Navigation::Play:
                if (isSignalConnected(QMetaMethod::fromSignal(&NavigationAttached::play)))
                {
                    qne.setAccepted(true);
                    emit play(&qne);
                }
                break;
            case Navigation::PlayPause:
                if (isSignalConnected(QMetaMethod::fromSignal(&NavigationAttached::playPause)))
                {
                    qne.setAccepted(true);
                    emit playPause(&qne);
                }
                break;
            case Navigation::Pause:
                if (isSignalConnected(QMetaMethod::fromSignal(&NavigationAttached::pause)))
                {
                    qne.setAccepted(true);
                    emit pause(&qne);
                }
                break;
            case Navigation::Stop:
                if (isSignalConnected(QMetaMethod::fromSignal(&NavigationAttached::stop)))
                {
                    qne.setAccepted(true);
                    emit stop(&qne);
                }
                break;
            case Navigation::Next:
                if (isSignalConnected(QMetaMethod::fromSignal(&NavigationAttached::next)))
                {
                    qne.setAccepted(true);
                    emit next(&qne);
                }
                break;
            case Navigation::Previous:
                if (isSignalConnected(QMetaMethod::fromSignal(&NavigationAttached::previous)))
                {
                    qne.setAccepted(true);
                    emit previous(&qne);
                }
                break;

            case Navigation::Record:
                if (isSignalConnected(QMetaMethod::fromSignal(&NavigationAttached::record)))
                {
                    qne.setAccepted(true);
                    emit record(&qne);
                }
                break;
            default:
                break;
            }

            event->setAccepted(qne.isAccepted());
        }
    }
}

void Navigation::dispatchNavigationEvent(Navigation::Button button, bool autoRepeat)
{
    QKeyEvent * keyEvent = new QKeyEvent(QEvent::KeyPress, translateButton(button), Qt::NoModifier, QString(), autoRepeat);
    QCoreApplication::postEvent(QGuiApplication::focusObject(), keyEvent);
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

NavigationAttached * Navigation::qmlAttachedProperties(QObject *object)
{
    return new NavigationAttached(object);
}
