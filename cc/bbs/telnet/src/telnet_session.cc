#include "telnet_server.h"
#include <iostream>
#include <boost/bind.hpp>

TelnetSession::TelnetSession(ba::io_service& io_service) : s(io_service) {}

void TelnetSession::start() {
    
    s.async_read_some(boost::asio::buffer(data, max_length),
        boost::bind(&TelnetSession::handle_read, this,
            ba::placeholders::error, ba::placeholders::bytes_transferred));
}

void TelnetSession::handle_read(const bs::error_code& error, size_t bytes_transferred) {
    
    if (!error) {
        std::cout << "TelnetSession::handle_read " << (int) bytes_transferred << " bytes\n";
        ba::async_write(s,
            ba::buffer(data, bytes_transferred), 
            boost::bind(&TelnetSession::handle_write, this, ba::placeholders::error));
    } else {
        std::cout << "TelnetSession::handle_read error: " << error << std::endl;
        delete this;
    }
}

void TelnetSession::handle_write(const bs::error_code& error) {
    
    if (!error) {
        std::cout << "TelnetSession::handle_write\n";
        s.async_read_some(ba::buffer(data, max_length),
            boost::bind(&TelnetSession::handle_read, this,
                ba::placeholders::error, ba::placeholders::bytes_transferred));
    } else {
        std::cout << "TelnetSession::handle_write error: " << error << std::endl;
        delete this;
    }
}

