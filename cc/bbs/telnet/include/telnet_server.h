#ifndef __TELNET_SERVER_H__
#define __TELNET_SERVER_H__

#include "telnet_session.h"

struct TelnetServer {

    TelnetServer(ba::io_service& io_service, uint16_t port);

private:
    void start_accept();
    void handle_accept(TelnetSession *new_session, const bs::error_code& error);

    ba::io_service  &io_service;
    tcp::acceptor   acceptor;
};

#endif

