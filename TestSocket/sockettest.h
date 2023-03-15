#ifndef SOCKETTEST_H
#define SOCKETTEST_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include <QTcpServer>
#include <QTcpSocket>
#include <QAbstractSocket>
using namespace  cv;
class SocketTest : public QObject
{
    Q_OBJECT
public:
    explicit SocketTest(QObject *parent = 0);

    void Connect();

signals:
    
public slots:

private:
    QTcpSocket *socket;
    VideoCapture CapBarcode_0;
    Mat FrameBarcode_0;
};

#endif // SOCKETTEST_H
