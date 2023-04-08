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

#define WIDTH  1280
#define HEIGHT 720
#define DEPTH  1

int main()
{
    std::string download_dir = "/Users/overman/dev/github/_mycode/_cpp/image_server/data/out";
    int cameraId = 0;
    int height = 720;
    int width = 1280;

    while ( true) {
        try {
            boost::asio::io_context io_context;

            // Create a TCP socket and connect to the server on port 8080
            tcp::socket socket(io_context);
            socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 8080));

            stringstream ss;
            auto time = std::chrono::system_clock::now();
            ss << cameraId << "|" << time.time_since_epoch().count() << endl;
            std::string request = ss.str();
            boost::asio::write(socket, boost::asio::buffer(request));
            std::cout << "Request sent to server" << std::endl;

            // Read the image data from the server
            std::vector<char> image_data;
            boost::asio::streambuf response_buf;
            boost::asio::read(socket, response_buf, boost::asio::transfer_exactly(WIDTH * HEIGHT * DEPTH));
            //boost::asio::read_until(socket, response_buf, "\n");
            image_data = std::vector<char>(buffers_begin(response_buf.data()), buffers_end(response_buf.data()));
            size_t lenrec = image_data.size();

            cv::Mat img(height, width, cv::IMREAD_GRAYSCALE);
            memcpy(img.data, &image_data[0], image_data.size());
            imshow("image", img);
            cv::waitKey(0);

            stringstream ss2;
            ss2 << download_dir << "/" << cameraId << time.time_since_epoch().count() << ".jpg";
            imwrite(ss2.str(), img);
        }
        catch (std::exception &e) {
            std::cerr << "Exception: " << e.what() << std::endl;
        }
    }
    return 0;
}
