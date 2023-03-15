
#include "tensortask.h"
#include <QDebug>


TensorTask::TensorTask()
{
//    qDebug() << "Tensor Task Created";

}

// When the thread pool kicks up, and implements Qrunnable
// it's going to hit this run, and it's going to do this time consuming task.
// After it's done, we're going to send the results back to our main thread.
// This runs in the separate thread, and we do not have any control over this thread,
// but Qt does.
// This may just stay in the queue for several ms depending on how busy the server is.


void TensorTask::run()
{
    // time consumer

//    qDebug() << "Task start";
    auto start_time = std::chrono::high_resolution_clock::now();

    detectNet::Detection* detections = NULL;
    uchar3* imgBufferRGB = NULL;

    cudaMalloc((void**)&imgBufferRGB, Frame.cols * sizeof(uchar3) * Frame.rows);
    cudaMemcpy2D((void*)imgBufferRGB, Frame.cols*sizeof(uchar3), (void*)Frame.data, Frame.step,Frame.cols*sizeof(uchar3), Frame.rows, cudaMemcpyHostToDevice);
    int numDetections = net->Detect(imgBufferRGB, Frame.cols, Frame.rows, &detections, overlayFlags);

    Mat tempFrame = Frame;

    if( numDetections > 0 ) {
        // LogVerbose("%i objects detected\n", numDetections);
        for( int n=0; n < numDetections; n++ ) {

            if (detections[n].ClassID==1) {
                detectionAcc = QString::number(detections[n].Confidence);
                //                // Setup a rectangle to define your region of interest
                int left=(int)detections[n].Left;
                int top=(int)detections[n].Top;
                int width=(int)detections[n].Width();
                int height=(int)detections[n].Height();


                cv::Rect myROI(left, top ,width, height);
                //qDebug()<<"Detected Barcode!";
                // Crop the full image to that image contained by the rectangle myROI
                // Note that this doesn't copy the data
                cv::Mat barcode = tempFrame(myROI);
                if ((int)detections[n].Left!=38) {
                    //Decode
                    //imwrite("barcode.jpg",barcode);
                    vector<decodedObject> decodedObjects;
                    barcodeString = decodeBarcode(barcode,decodedObjects,QString("Normal"));
                    emit Result(barcodeString);
                }
            }
        }
    }
    cudaFree(imgBufferRGB);

    auto end_time = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
    elapsed /= 1000000000;
//    qDebug()<<"FPS Detection: "<<1/(elapsed);

//    qDebug() << "Task done";
}

QString TensorTask::decodeBarcode(Mat &im, vector<decodedObject>&decodedObjects, QString threadName)
{
    cv::cvtColor(im, imGray,cv::COLOR_BGR2GRAY);
    // Wrap image data in a zbar image
    Image image(im.cols, im.rows, "Y800", (uchar *)imGray.data, im.cols * im.rows);

    n = scanner.scan(image);
    QString tempObj = "0";

    // Print results
    for(Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol) {

        obj.type = symbol->get_type_name();
        obj.data = symbol->get_data();
        tempObj.append("1/");
        tempObj.append(obj.data.c_str());
        tempObj.append("/45/45/45");
//        barcodeString = tempObj;
        qDebug()<<threadName<<"_"<<"Barcodes: "<<tempObj;
        decodedObjects.push_back(obj);
       // QThread::currentThread()->msleep(10000);
        return tempObj;
    }

    return tempObj;
}
