#include <QGuiApplication>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QDebug>

#include "sonataapplication.h"


int main(int argc, char *argv[])
{
    Application sonata(argc, argv);
    Sonata::registerMetaTypes();

    return sonata.start();
}
