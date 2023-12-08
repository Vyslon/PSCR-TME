#ifndef SRC_SERVERSOCKET_H_
#define SRC_SERVERSOCKET_H_

#include "Socket.h"
#include <iostream>

namespace pr {

class ServerSocket {
	int socketfd;

public :
	// Demarre l'ecoute sur le port donne
	ServerSocket(int port) {
        socketfd = socket(AF_INET, SOCK_STREAM, 0);
        if (socketfd < 0) { perror("socket"); }
        struct sockaddr_in adr;
        adr.sin_family = AF_INET;
        adr.sin_port = htons(port);
        adr.sin_addr.s_addr = INADDR_ANY;
        if (bind(socketfd, (struct sockaddr *)&adr, sizeof adr) < 0) { perror("bind"); }
        int i = 1;
        setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof i);

        if (listen(socketfd, 10) < 0) {
            perror("listen");
            // todo : close
        }
    }

	int getFD() { return socketfd;}
	bool isOpen() const {return socketfd != -1;}

	Socket accept() {
        struct sockaddr exp;
        socklen_t sz;
        int sc = ::accept(socketfd, &exp, &sz);
        std::cout << "Connection de " << (sockaddr_in *)&exp;
        return Socket(sc);
    }

	void close();
};

} // ns pr
#endif /* SRC_SERVERSOCKET_H_ */
