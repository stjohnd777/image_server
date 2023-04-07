#include <boost/asio.hpp>
#include <iostream>
#include <fstream>

using boost::asio::ip::tcp;

int main()
{
    try
    {
        boost::asio::io_context io_context;

        // Create a TCP socket and connect to the server on port 8080
        tcp::socket socket(io_context);
        socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("192.168.1.12"), 8080));

        // Send the request for the image file "image.jpg"
        std::string request = "/Users/overman/dev/github/_mycode/_cpp/image_server/data/ori.jpg\n";
        boost::asio::write(socket, boost::asio::buffer(request));

        std::cout << "Request sent to server" << std::endl;

        // Read the image data from the server
        std::vector<char> image_data;
        boost::asio::streambuf response_buf;
        boost::asio::read_until(socket, response_buf, "\n");
        image_data = std::vector<char>(buffers_begin(response_buf.data()), buffers_end(response_buf.data()));

        // Save the image data to a file
        std::ofstream file("/Users/overman/dev/github/_mycode/_cpp/image_server/data/received_image.jpg", std::ios::out | std::ios::binary);
        file.write(image_data.data(), image_data.size());
        file.close();

        std::cout << "Image received and saved to file" << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
