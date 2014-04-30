#include "navigationcontroller.h"
#include <QCoreApplication>



NavigationController::NavigationController()
    :   m_lirc(new LircClient)
{ }

NavigationController::~NavigationController()
{
#ifdef Q_OS_WIN32
    Q_ASSERT(QCoreApplication::instance() != nullptr);
    QCoreApplication::instance()->removeNativeEventFilter(&m_filter);
#endif
}

void NavigationController::initialize(int lircDelay)
{
    Q_UNUSED(lircDelay);
#if defined(Q_OS_WIN32)
    Q_ASSERT(QCoreApplication::instance() != nullptr);
    QCoreApplication::instance()->installNativeEventFilter(&m_filter);
#elif defined(Q_OS_LINUX)
    m_lirc->setDelay(lircDelay);
    m_lirc->attach();
#endif
}
