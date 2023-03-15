#include "sockettest.h"
#include <QImage>
#include <QBuffer>
SocketTest::SocketTest(QObject *parent) :
    QObject(parent)
{
    CapBarcode_0.open(0,CAP_V4L2);
    if (!CapBarcode_0.isOpened()) {
        qDebug() << "ERROR! Unable to open camera_0\n";
    }
//    CapBarcode_0.set(CAP_PROP_FOURCC,1196444237);
//    CapBarcode_0.set(CAP_PROP_FRAME_WIDTH, 1920);
//    CapBarcode_0.set(CAP_PROP_FRAME_HEIGHT, 1200);
//    CapBarcode_0.set(CAP_PROP_FPS,60);
//    CapBarcode_0.set(CAP_PROP_EXPOSURE,312);
}

void SocketTest::Connect()
{
    socket = new QTcpSocket(this);
    socket->connectToHost(QHostAddress::Any, 7070);

    if(socket->waitForConnected(3000))
    {
        qDebug() << "Connected!";

        while (true) {
            auto start_time = std::chrono::high_resolution_clock::now();

            CapBarcode_0.read(FrameBarcode_0);
            if (FrameBarcode_0.empty()) {
                qDebug()<<"Frame_0 Grapping Error";
            }
            QImage qimage(FrameBarcode_0.data, FrameBarcode_0.cols, FrameBarcode_0.rows, FrameBarcode_0.step, QImage::Format_RGB888);
            qDebug()<<qimage.width()<<"     "<<FrameBarcode_0.type();

            QImage qimage2 = qimage; // your QImage object

            // Convert QImage to QByteArray
            QByteArray byteArray;
            QBuffer buffer(&byteArray);
            buffer.open(QIODevice::WriteOnly);
            qimage2.save(&buffer, "JPEG");

            // Encode the QByteArray to Base64
            QString base64String = byteArray.toBase64();

            // Convert the Base64-encoded string to QByteArray
            QByteArray base64ByteArray = base64String.toUtf8();
            socket->write(base64ByteArray);
            socket->waitForBytesWritten(100);
            socket->waitForReadyRead(100);
            qDebug() << "Reading: " << socket->bytesAvailable();

            qDebug() << socket->readAll();
            auto end_time = std::chrono::high_resolution_clock::now();
            double elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
            elapsed /= 1000000000;
            qDebug()<<"FPS Camera_0: "<<1/(elapsed);
            socket->flush();

        }
    }
    else
    {
        qDebug() << "Not connected!";
    }

}
