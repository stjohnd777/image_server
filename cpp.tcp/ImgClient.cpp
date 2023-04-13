#include <boost/asio.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;
using boost::asio::ip::tcp;


bool isRunning = true;
bool CheckShutdown() {
    // TODO: poll file system for stop file
    return !isRunning;
}


int main() {

#define WIDTH  1280
#define HEIGHT 720
#define DEPTH  1
    std::string download_dir = "/Users/overman/dev/github/_mycode/_cpp/image_server/data/out";
    int cameraId = 0;
    string ipServer = "127.0.0.1";
    int port = 8080;
    size_t READ_EXACTLY = HEIGHT * WIDTH * DEPTH;

    while (!CheckShutdown()) {
        try {
            boost::asio::io_context io_context;

            // Create a TCP socket and connect to the server on port 8080
            tcp::socket socket(io_context);
            socket.connect(tcp::endpoint(boost::asio::ip::address::from_string(ipServer), port));

            // construct request string
            // cameraID|time|cmd1| ... cmd(N)\n
            stringstream ss;
            auto time = std::chrono::system_clock::now();
            ss << cameraId << "|" << time.time_since_epoch().count() << endl;
            std::string request = ss.str();
            boost::asio::write(socket, boost::asio::buffer(request));
            std::cout << "Request sent to server" << std::endl;

            // Read the image data from the server using exactly READ_EXACTLY bytes
            std::vector<char> image_data;
            boost::asio::streambuf response_buf;
            boost::asio::read(socket, response_buf, boost::asio::transfer_exactly(READ_EXACTLY));
            image_data = std::vector<char>(buffers_begin(response_buf.data()), buffers_end(response_buf.data()));

            // validate read length
            size_t bytes_rcv = image_data.size();
            if (bytes_rcv != READ_EXACTLY) {
                cerr << "Invalid Read Length, abort this response" << endl;
                continue;
            }

            // Convert bytes to cv::Mat
            cv::Mat img(HEIGHT, WIDTH, cv::IMREAD_GRAYSCALE);
            memcpy(img.data, &image_data[0], image_data.size());
            imshow("image", img);
            cv::waitKey(0);

            // Save Image to File
            stringstream ss2;
            ss2 << download_dir << "/" << cameraId << time.time_since_epoch().count() << ".jpg";
            imwrite(ss2.str(), img);

        } catch (std::exception &e) {
            std::cerr << "Exception: " << e.what() << std::endl;
            cerr << "Make sure server is running and host ip and port are correct" << endl;
        }
    }
    return 0;
}
