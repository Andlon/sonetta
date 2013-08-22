#include <QCoreApplication>
#include "sessiontest.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    SessionTest * s = SessionTest::instance();
    if (s->createSession())
        s->login("username", "password");

    return app.exec();
}
