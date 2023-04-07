#include <boost/asio.hpp>
#include <iostream>
#include <fstream>
#include <boost/asio.hpp>
#include <iostream>
#include <fstream>

using boost::asio::ip::tcp;

int main()
{
    try
    {
        boost::asio::io_context io_context;

        // Create a TCP acceptor to listen for incoming connections on port 8080
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8080));

        while (true)
        {
            // Wait for a client to connect
            tcp::socket socket(io_context);
            acceptor.accept(socket);

            std::cout << "Client connected" << std::endl;

            // Read the request from the client
            boost::asio::streambuf request_buf;
            boost::asio::read_until(socket, request_buf, "\n");
            std::string request = boost::asio::buffer_cast<const char*>(request_buf.data());

            // Open the requested image file and read its contents
            std::ifstream file("/Users/overman/dev/github/_mycode/_cpp/image_server/data/ori.jpg");
            std::vector<char> file_contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

            // Send the image data to the client
            boost::asio::write(socket, boost::asio::buffer(file_contents));

            std::cout << "Image sent to client" << std::endl;
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}