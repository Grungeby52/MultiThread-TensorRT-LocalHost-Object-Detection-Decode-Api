#include <QCoreApplication>
#include <sockettest.h>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    SocketTest test;
    test.Connect();
    return a.exec();
}
