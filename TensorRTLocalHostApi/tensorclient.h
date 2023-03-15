#ifndef TENSORCLIENT_H
#define TENSORCLIENT_H

#include <QObject>

#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QThreadPool>
#include <QString>
#include <QImage>
#include <opencv2/opencv.hpp>
#include "tensortask.h"


using namespace std;
using namespace cv;
class TensorClient : public QObject
{
    Q_OBJECT
public:
    explicit TensorClient(QObject *parent = 0);
    void SetSocket(int Descriptor);
    QString data;
    detectNet* net;
    uint32_t overlayFlags;

signals:

public slots:
    void connected();
    void disconnected();
    void readyRead();
    void TaskResult(QString Number);

private:
    QTcpSocket *socket;
    TensorTask *mytask;
};

#endif // TENSORCLIENT_H
