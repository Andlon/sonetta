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

Navigation::Navigation(QObject *parent) :
    QObject(parent), m_filter(new NavigationNativeEventFilter)
{
    QCoreApplication::instance()->installNativeEventFilter(m_filter);

#ifdef Q_OS_LINUX
    m_lirc = new LircRemote;
    m_lirc->attach();
#endif
}

Navigation::~Navigation()
{
    delete m_filter;
}

bool Navigation::dispatchNavigationEvent(NavEvent *event)
{
    if (event->key() == Navigation::Undefined)
        return false;

    QObject * receiver = QGuiApplication::focusObject();

    while (receiver && !event->isAccepted())
    {
        // Send to object
        QCoreApplication::sendEvent(receiver, event);

        if (!event->isAccepted())
        {
            // Send to attached item
            QObject * attachee = qmlAttachedPropertiesObject<Navigation>(receiver, false);

            if (attachee)
                QCoreApplication::sendEvent(attachee, event);

            // Try parent next, visual items if Quick items
            QQuickItem * qItem = qobject_cast<QQuickItem *>(receiver);
            if (qItem && qItem->parentItem())
                receiver = qItem->parentItem();
            else
                receiver = receiver->parent();
        }
    }

    return event->isAccepted();
}

bool Navigation::dispatchKeyEvent(QKeyEvent *keyEvent)
{
    Navigation::Key key = translateKey(static_cast<Qt::Key>(keyEvent->key()));
    NavEvent navEvent(key, keyEvent->isAutoRepeat());
    return dispatchNavigationEvent(&navEvent);
}

Navigation::Key Navigation::translateKey(Qt::Key key)
{
    switch (key)
    {
    case Qt::Key_Left:
        return Navigation::Left;
        break;
    case Qt::Key_Right:
        return Navigation::Right;
        break;
    case Qt::Key_Up:
        return Navigation::Up;
        break;
    case Qt::Key_Down:
        return Navigation::Down;
        break;
    case Qt::Key_Return:
        return Navigation::OK;
        break;
    case Qt::Key_Backspace:
        return Navigation::Back;
        break;
    case Qt::Key_MediaPlay:
        return Navigation::Play;
        break;
    case Qt::Key_MediaPause:
        return Navigation::Pause;
        break;
    case Qt::Key_MediaTogglePlayPause:
        return Navigation::PlayPause;
        break;
    case Qt::Key_MediaStop:
        return Navigation::Stop;
        break;
    case Qt::Key_MediaNext:
        return Navigation::Next;
        break;
    case Qt::Key_MediaPrevious:
        return Navigation::Previous;
        break;

    default:
        return Navigation::Undefined;
    }
}

NavigationAttached * Navigation::qmlAttachedProperties(QObject *object)
{
    return new NavigationAttached(object);
}
