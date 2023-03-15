#include "tensorclient.h"

#include <QImage>
#define IS_HEADLESS() (const char*)NULL

TensorClient::TensorClient(QObject *parent) :
    QObject(parent)
{
    QThreadPool::globalInstance()->setMaxThreadCount(5);

    qDebug()<<"Barcode Initilaizing...";
    char *argv[7];
    argv[0]="./ScanPickV4";
    argv[1]="--network=barcodeDetectModel/ssd-mobilenet.onnx";
    argv[2]="--labels=barcodeDetectModel/labels.txt";
    argv[3]="--input-blob=input_0";
    argv[4]="--output-cvg=scores";
    argv[5]="--output-bbox=boxes";
    argv[6]="--threshold=0,30";

    commandLine cmdLine(7, argv, IS_HEADLESS());
    cmdLine.Print();
    net=detectNet::Create(cmdLine);
    overlayFlags = detectNet::OverlayFlagsFromStr(cmdLine.GetString("overlay", "box,labels,conf"));

}

void TensorClient::SetSocket(int Descriptor)
{
    // make a new socket
    socket = new QTcpSocket(this);

    qDebug() << "A new socket created!";

    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    socket->setSocketDescriptor(Descriptor);

    qDebug() << " Client connected at " << Descriptor;
}


// asynchronous - runs separately from the thread we created
void TensorClient::connected()
{
    qDebug() << "Client connected event";
}

// asynchronous
void TensorClient::disconnected()
{
    qDebug() << "Client disconnected";
}

// Our main thread of execution
// This happening via main thread
// Our code running in our thread not in Qthread
// That's why we're getting the thread from the pool

void TensorClient::readyRead()
{
    auto start_time = std::chrono::high_resolution_clock::now();

    data =  socket->readAll();
    socket->waitForReadyRead(100);
    QByteArray imageData = QByteArray::fromBase64(data.toStdString().c_str());
    QImage image;
    image.loadFromData(imageData);
    cv::Mat mat(image.height(), image.width(), CV_8UC4, image.bits(), image.bytesPerLine());
//    cv::Mat mImg2(image.height(), image.width(), CV_8UC3);
//    mat.convertTo(mImg2, CV_8UC3);
    cv::Mat temp;
    if (!mat.empty()) {
        cv::cvtColor(mat, temp,cv::COLOR_BGRA2RGB);

        // Time consumer
        TensorTask *mytask = new TensorTask();
        mytask->setAutoDelete(true);
        connect(mytask, SIGNAL(Result(QString)), this, SLOT(TaskResult(QString)), Qt::QueuedConnection);

        mytask->Frame = temp;
        mytask->net = net;
        mytask->overlayFlags = overlayFlags;
//        qDebug() << "Starting a new task using a thread from the QThreadPool";
        QThreadPool::globalInstance()->start(mytask);
        //QThread::currentThread()->msleep(150);
        auto end_time = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
        elapsed /= 1000000000;
    }

    //qDebug()<<"FPS Server: "<<1/(elapsed);

}

void TensorClient::TaskResult(QString Number)
{
    QByteArray Buffer;
    Buffer.append(Number);
    socket->write(Buffer);
    socket->flush();
    socket->waitForBytesWritten(3000);

}

