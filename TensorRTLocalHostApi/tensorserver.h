#ifndef TENSORSERVER_H
#define TENSORSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QAbstractSocket>
#include "tensorclient.h"
#include <QString>
class TensorServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TensorServer(QObject *parent = 0);
    void StartServer();
protected:
    void incomingConnection(qintptr handle);

signals:

public slots:
};

#endif // TENSORSERVER_H
