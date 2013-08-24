#include <QGuiApplication>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QDebug>

#include "application.h"


int main(int argc, char *argv[])
{
    Sonetta::Application sonetta(argc, argv);

    return sonetta.run();
}
