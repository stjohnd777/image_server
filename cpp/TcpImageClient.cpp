#include <iostream>
#include <tuple>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "SyncTcpClient.h"

using namespace std;
using namespace cv;


int main() {

    const std::string host = "192.168.1.7";
    //const std::string host = "127.0.0.1";
    const unsigned short port = 7767;
    int cameraId = 0;
    int height = HEIGHT;
    int width = WIDTH;

    bool isRunning = true;
    stringstream ss;
    ss << "Camera: " << cameraId << ":" << width << "x" << height << ":" << "tcp@" << host << ":" << port;
    string windowName = ss.str();
    cv::Mat img(height, width, cv::IMREAD_GRAYSCALE);


    auto start = std::chrono::system_clock::now();

    while (isRunning) {
        SyncTcpClient client(host, port);

        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> diff = end - start;
        double time = diff.count(); // GPS TIME
        cout << "client request cameraId: " << cameraId << " time:" << time << endl;
        auto tupResponse = client.request_reply(time, cameraId);


        size_t numberBytesReply = get<0>(tupResponse);
        const char* bytesReply = get<1>(tupResponse);
        cout << "client received response bytes" << numberBytesReply << endl;
        if (numberBytesReply == 0) {
            cout << "Zero bytes return " << endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }
        if (numberBytesReply == 1) {
            cout << "Error Code Returned: " << (int)bytesReply[0] << endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }

        memcpy(img.data, bytesReply, numberBytesReply);
        imshow(windowName, img);
        cv::waitKey(0);
        cout << "next round" << endl;

    }

}