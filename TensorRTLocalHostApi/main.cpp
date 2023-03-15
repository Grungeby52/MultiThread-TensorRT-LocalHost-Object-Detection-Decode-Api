#include <QCoreApplication>
#include <tensorserver.h>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    TensorServer Server;
    Server.StartServer();
    return a.exec();
}
