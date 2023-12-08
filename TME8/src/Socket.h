#ifndef SRC_SOCKET_H_
#define SRC_SOCKET_H_

#include <netinet/ip.h>
#include <string>
#include <iosfwd>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

namespace pr {

class Socket {
	int fd;

public :
	Socket():fd(-1){}
	Socket(int fd):fd(fd){}

	// tente de se connecter à l'hôte fourni
	void connect(const std::string & host, int port) {
        struct in_addr ipv4;
        struct addrinfo * adr;
        if (getaddrinfo(host.c_str(), nullptr, nullptr, &adr))
        {
            perror("DNS");
        }
        for (struct addrinfo * rp = adr; rp != NULL; rp->ai_next)
        {
            if (rp->ai_family == AF_INET && rp->ai_socktype) {
                ipv4 = ((struct sockaddr_in *)rp->ai_addr)->sin_addr;
                break;
            }
        }
        freeaddrinfo(adr);
        connect(ipv4, port);
    }

	void connect(in_addr ipv4, int port) {
        fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (fd < 0) {
            perror("socket");
        }

        struct sockaddr_in server;
        server.sin_family = AF_INET;
        server.sin_port = htons(port);
        server.sin_addr = ipv4;
        if (::connect(fd, (struct sockaddr *)&server, sizeof server) < 0) {
            ::close(fd);
            fd = -1;
            perror("connect");
        }
    }

	bool isOpen() const {return fd != -1;}
	int getFD() { return fd ;}

	void close() {
        if (fd != -1) {
            ::close(fd);
            fd = -1;
        }
    }
};

std::ostream & operator<< (std::ostream & os, struct sockaddr_in * addr) {
    struct sockaddr_in {
      sa_family_t sin_family;
      in_port_t sin_port;
      struct in_addr sin_addr;
    };

    struct in_addr {
        uint32_t s_addr;
    };

    char host[1024];
    if (getnameinfo((struct sockaddr *)addr, sizeof (struct sockaddr_in), host, 1024, nullptr, 0, 0)) {
        os << "host : " << host << std::endl;
    }

    std::cout << inet_ntoa(addr->sin_addr) << " : " << ntohs(addr->sin_port);

    return os;
}

}

#endif /* SRC_SOCKET_H_ */
