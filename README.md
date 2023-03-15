# MultiThread-TensorRT-LocalHost-Object-Detection-Decode-Api
This repository was created to run Object Detection Multithread on the GPUs of servers that support TensorRt. In other words, the Object Detection Model runs on a server and the clients send images to the server and return results to them on the server.

For example, a barcode detection model is trained and decode operations of the barcode are also performed.

There are two projects in repo. First project for Server, second project for Client Test. 

You will need to copy the BarcodeDetection model folder into the build folder of the server project.


### 1 - Instalition
    First you need to perform the installations here.
    https://github.com/Grungeby52/Cuda-10.2-Cudnn-8.2.1-TensorRT-8.0.1.6-Install-Ubuntu-18.04/edit/main/README.md
    
### Requirements
- Cmake 3.10.2
- Opencv 4.5.3 + Contribute 4.5.3
- Zbar Library
- Jetson Inference
- Qt5
