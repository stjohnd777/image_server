#include <boost/asio.hpp>
#include <iostream>
#include <fstream>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>


#include <boost/asio.hpp>
#include <iostream>
#include <fstream>

using namespace cv;
using boost::asio::ip::tcp;

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

struct camera_trigger {
    int cameraId;
    enum PROC {
        RAW_CAMERA,
        REMAP,
        THRESHOLDING_BINARY,
        BAND_BINARY,
        BLOB,
        CENTROID,
        SOBEL,
        FAST,
        CANNY,
        MEAN3,
        GAUSSIAN3
    };
    PROC proc;
    bool latest;
    double gpsTime;
};

bool CheckShutdown() {
    // TODO: poll file system for stop file
    return false;
}


int main() {
    try {

        int cameraId = 0;
        int port = 8080;

        bool isCamera = true;
        VideoCapture *pVideoCapture = new VideoCapture(cameraId);
        if (!pVideoCapture->isOpened()) {
            return -1;
        }
        if (!pVideoCapture->isOpened()) {
            delete pVideoCapture;
            return -2;
        }

        boost::asio::io_context io_context;

        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));


        while (!CheckShutdown()) {
            // Wait for a client to connect
            tcp::socket socket(io_context);
            acceptor.accept(socket);

            std::cout << "Client connected" << std::endl;

            // Read the request from the client
            boost::asio::streambuf request_buf;
            boost::asio::read_until(socket, request_buf, "\n");
            std::string request = boost::asio::buffer_cast<const char *>(request_buf.data());

            if (isCamera) {
                cv::Mat aCameraFrame;
                *(pVideoCapture) >> aCameraFrame;
                cvtColor(aCameraFrame, aCameraFrame, COLOR_BGR2GRAY);
                size_t len = aCameraFrame.rows * aCameraFrame.cols * aCameraFrame.elemSize();
                boost::asio::write(socket, boost::asio::buffer(aCameraFrame.data, len));
            } else {
                // Open the requested image file and read its contents
                std::ifstream file("/Users/overman/dev/github/_mycode/_cpp/image_server/data/ori.jpg");
                std::vector<char> file_contents((std::istreambuf_iterator<char>(file)),
                std::istreambuf_iterator<char>());
                // Send the image data to the client
                boost::asio::write(socket, boost::asio::buffer(file_contents));
            }
            std::cout << "Image sent to client" << std::endl;
        }
    }
    catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}