#include "tensorserver.h"

TensorServer::TensorServer(QObject *parent) : QTcpServer(parent)
{

}

void TensorServer::StartServer()
{
    if(listen(QHostAddress::Any, 7070))
    {
        qDebug() << "Server: started";
    }
    else
    {
        qDebug() << "Server: not started!";
    }
}

void TensorServer::incomingConnection(qintptr handle)
{
    // at the incoming connection, make a client
    TensorClient *client = new TensorClient(this);
    client->SetSocket(handle);
}
