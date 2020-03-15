//
//  main.cpp
//  Boost ASIO
//
//  Created by Richard Henry on 28/02/2019.
//  Copyright © 2019 Dogstar Industries Ltd. All rights reserved.
//

#include <iostream>
#include <boost/asio.hpp>

namespace sock {
    
    using namespace boost::asio;
    using ip::tcp;

    std::string read_string(tcp::socket & socket) {
        
        streambuf buf;
        read_until(socket, buf, "\n");
        std::string data = buffer_cast<const char *>(buf.data());
        
        return data;
    }
    
    void send_string(tcp::socket & socket, const std::string& message) {
        
        write(socket, buffer(message + "\n"));
    }
    
    void send_and_recieve_on_socket() {
        
        io_service io_service;
        
		std::cout << "Listening on socket 31337"<< std::endl;

        // Set up sockets
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 31337));
        tcp::socket socket(io_service);
        
        // Wait for a connection
        acceptor.accept(socket);
        
        // Log received message
        std::cout << "Received : " << read_string(socket) << std::endl;
        
        // Send a reply
        send_string(socket, "Here are the gumboots you ordered madam.\n");
    }
}

#include <boost/interprocess/ipc/message_queue.hpp>

namespace {
    
    void message_queue_exchange() {
        
        std::unique_ptr<int> p = std::make_unique<int>(5);
        //std::unique_ptr<boost::interprocess::message_queue> incomingQueue = std::make_unique<boost::interprocess::message_queue>();
        //m_incomingQueue = std::move(std::make_unique<boost::interprocess::message_queue>(boost::interprocess::create_only, &m_userBrowserToDaemonQueueName[0u], k_maxMessageQueued, k_maxMessageSize, perm));

    }
}

int main(int argc, const char * argv[]) {
    
    sock::send_and_recieve_on_socket();
    
    return 0;
}
