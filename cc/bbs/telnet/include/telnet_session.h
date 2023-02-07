#ifndef __TELNET_SESSION_H__
#define __TELNET_SESSION_H__

#include <boost/asio.hpp>

namespace ba = boost::asio;
namespace bs = boost::system;
using boost::asio::ip::tcp;

struct TelnetSession {

    TelnetSession(ba::io_service& io_service);

    tcp::socket& socket() { return s; }

    void start();

    enum { max_length = 10 };

private:
    void handle_read(const bs::error_code& error, size_t bytes_transferred);

    void handle_write(const bs::error_code& error);

    tcp::socket     s;
    char            data[max_length];
};

#endif

