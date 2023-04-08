#pragma once

#include <boost/asio.hpp>

#include <string>
#include <sstream>
#include <thread>
#include <atomic>
#include <memory>
#include <iostream>
#include <functional>
#include <vector>

#include "Common.h"

using namespace boost;
using namespace std;
using boost::asio::ip::tcp;

class SyncTcpClient {
public:
    SyncTcpClient(const std::string &host, unsigned short port) :
    m_sock(m_ios) {
        m_sock.connect(tcp::endpoint(boost::asio::ip::address::from_string(host), port));
    }

    std::tuple<size_t, const char *> request_reply(double timse, uint16_t camera_id) {
        stringstream ss;
        ss << camera_id << "|" << time << endl;
        sendRequest(ss.str());
        return receiveResponse();
    };

    virtual ~SyncTcpClient() {
        close();
    }

private:

    virtual void sendRequest(const std::string &request) {
        asio::write(m_sock, asio::buffer(request));
    }

    std::tuple<size_t, const char *> receiveResponse() {
        boost::system::error_code error;
        boost::asio::streambuf receive_buffer;

//        boost::asio::streambuf response_buf;
//        boost::asio::read(socket, response_buf, boost::asio::transfer_exactly(WIDTH * HEIGHT * DEPTH) );
//        std::vector<char> image_data = std::vector<char>(buffers_begin(response_buf.data()), buffers_end(response_buf.data()));
//        char * a = = &response_buf[0];
//        char *dst = new char[WIDTH * HEIGHT * DEPTH];
//        memcpy(dst , a, WIDTH * HEIGHT * DEPTH);
//        return make_tuple(WIDTH * HEIGHT * DEPTH, dst);

        char *dst = new char[WIDTH * HEIGHT * DEPTH];
        memset(dst, 0, WIDTH * HEIGHT * DEPTH);
        size_t total = 0;
        while (true) {
            char temp_buffer[WIDTH * HEIGHT * DEPTH];
            boost::system::error_code error;
            size_t len = m_sock.read_some(boost::asio::buffer(temp_buffer), error);

            memcpy(dst + total, temp_buffer, len);
            total += len;

            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                return make_tuple(0, nullptr);
            // throw boost::system::system_error(error);
        }
        return make_tuple(total, dst);
    }

    void close() {
    }

private:
    asio::io_service m_ios;
    asio::ip::tcp::socket m_sock;
};
