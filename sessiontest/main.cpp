#include <QCoreApplication>
#include <QDebug>
#include "sessiontest.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    SessionTest * test = SessionTest::instance();

    if (test->createSession())
    {
        qDebug() << "Successfully created session. Logging in...";
        test->login("username", "password");
    }
    
    return a.exec();
}
