#pragma once

#define WIDTH  1280
#define HEIGHT 720
#define DEPTH  1

#include <thread>
#include <tuple>
#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "SyncTcpServer.h"

using namespace std;
using namespace cv;

int countCameras()
{
    cv::VideoCapture temp_camera;
    int maxTested = 10;
    for (int i = 0; i < maxTested; i++) {
        cv::VideoCapture temp_camera(i);
        bool res = (!temp_camera.isOpened());
        temp_camera.release();
        if (res)
        {
            return i;
        }
    }
    return maxTested;
}

/*
std::thread* StartTcpImageServer(std::string ip = "127.0.0.1", unsigned short port = 7767, int cameraId = 0) {

    auto provider = [&](string ip, unsigned short port, int cameraId) {
        bool isRunning = true;
        VideoCapture* pVideoCapture = new VideoCapture(cameraId);
        if (!pVideoCapture->isOpened()) {
            isRunning = false ;
        }
        if (!pVideoCapture->isOpened()) {
            delete pVideoCapture;
            isRunning = false;
        }

        auto srv = new SyncTcpServer(ip, port);
        int service_count = 0;

        while (isRunning) {
            srv->StartListeningAndHandleOneRequest([&](int cameraId, double time) {
                cv::Mat aCameraFrame;
                *(pVideoCapture) >> aCameraFrame;
                cvtColor(aCameraFrame, aCameraFrame, COLOR_BGR2GRAY);
                size_t len = aCameraFrame.rows * aCameraFrame.cols * aCameraFrame.elemSize();
                char* bytesToTransfer = new char[len];
                memset(bytesToTransfer, 0, len);
                memcpy(bytesToTransfer, aCameraFrame.data, len);
                auto tup = make_tuple(len, bytesToTransfer);
                return tup;
                });
            service_count++;
            cout << "server handled request " << service_count << endl;
        }

        delete srv;
        delete pVideoCapture;
    };

    return new thread(provider,ip,port,cameraId);
}
*/
