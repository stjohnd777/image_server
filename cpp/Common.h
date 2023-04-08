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
