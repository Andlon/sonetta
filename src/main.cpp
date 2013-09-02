#include <QGuiApplication>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QDebug>

#include "application.h"


int main(int argc, char *argv[])
{
    // Register Spotinetta types
    Spotinetta::registerTypes();

    QCoreApplication::setApplicationName("Sonetta");
    QCoreApplication::setOrganizationDomain("sonetta.org");
    QCoreApplication::setOrganizationName("Sonetta");

    Sonetta::Application sonetta(argc, argv);
    return sonetta.run();
}
