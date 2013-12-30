#include "application.h"
#include <QGuiApplication>


int main(int argc, char *argv[])
{
    // Register Spotinetta types
    Spotinetta::registerTypes();

    QCoreApplication::setApplicationName("Sonetta");
    QCoreApplication::setOrganizationDomain("sonetta.org");
    QCoreApplication::setOrganizationName("Sonetta");

    QScopedPointer<QGuiApplication> application(new QGuiApplication(argc, argv));

    application->setQuitOnLastWindowClosed(false);

    Sonetta::Application sonetta;
    if (sonetta.initialize())
    {
        return application->exec();
    }

    return -1;
}
