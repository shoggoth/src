#include "telnet_server.h"
#include <iostream>
#include <boost/bind/bind.hpp>

using namespace boost::placeholders;

TelnetServer::TelnetServer(ba::io_service& io_service, uint16_t port) : io_service(io_service), acceptor(io_service, tcp::endpoint(tcp::v4(), port)) {
    
    start_accept();
}

void TelnetServer::start_accept() {
    
    std::cout << "TelnetServer::start_accept\n";
    auto new_session = new TelnetSession(io_service);
    acceptor.async_accept(new_session->socket(),
                          boost::bind(&TelnetServer::handle_accept, this, new_session,
                                      ba::placeholders::error));
}

void TelnetServer::handle_accept(TelnetSession *new_session, const bs::error_code& error) {
    
    if (!error) new_session->start(); else delete new_session;
    
    start_accept();
}
