#include "application.h"
#include <QGuiApplication>


int main(int argc, char *argv[])
{
    Spotinetta::registerTypes();

    QCoreApplication::setApplicationName("sonetta");
    QCoreApplication::setOrganizationDomain("sonetta.org");
    QCoreApplication::setOrganizationName("sonetta");

    QScopedPointer<QGuiApplication> application(new QGuiApplication(argc, argv));

    application->setQuitOnLastWindowClosed(false);

    Sonetta::Application sonetta;
    if (sonetta.initialize())
    {
        return application->exec();
    }

    return -1;
}
